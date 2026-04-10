[CmdletBinding()]
param (
    [Parameter(Mandatory=$true, Position=0, HelpMessage="(ProjDir)")]
    [string]$ProjDir,

    [string]$EnumNamespace = "winrt::SystemExplorer::Core::Data::Enums",
    [string]$HelperNamespace = "winrt::SystemExplorer::Helpers",
    [string]$StringsHelperNamespace = "winrt::SystemExplorer::Helpers",
    [string]$ReswFilePath = ""
)

if (-not (Test-Path -Path $ProjDir -PathType Container)) {
    Write-Error "Project directory '$ProjDir' not found."
    exit
}

$availableStrings = @()
if ([string]::IsNullOrWhiteSpace($ReswFilePath)) {
    $foundResw = Get-ChildItem -Path $ProjDir -Filter "Resources.resw" -Recurse | Select-Object -First 1
    if ($foundResw) {
        $ReswFilePath = $foundResw.FullName
    }
}

if (-Not [string]::IsNullOrWhiteSpace($ReswFilePath) -and (Test-Path $ReswFilePath)) {
    [xml]$xml = Get-Content $ReswFilePath -Encoding UTF8
    $dataNodes = $xml.SelectNodes("//root/data")
    foreach ($node in $dataNodes) {
        $rawName = $node.name
        $safeName = $rawName -replace '[\.\-\s]', ''
        $availableStrings += $safeName
    }
    Write-Host "Loaded $($availableStrings.Count) resource keys from $ReswFilePath" -ForegroundColor Cyan
} else {
    Write-Host "Resw file not found. Falling back to string literals only." -ForegroundColor Yellow
}

$idlFiles = Get-ChildItem -Path $ProjDir -Filter "*.idl" -File -Recurse

if ($idlFiles.Count -eq 0) {
    Write-Host "No .idl files found in '$ProjDir'." -ForegroundColor Yellow
    exit
}

$allEnums = @()
$pattern = '(?s)\benum\s+([a-zA-Z0-9_]+)\s*\{([^}]+)\}'
$enumRegex = [regex]$pattern

foreach ($file in $idlFiles) {
    $content = [System.IO.File]::ReadAllText($file.FullName)
    $matches = $enumRegex.Matches($content)

    foreach ($match in $matches) {
        $enumName = $match.Groups[1].Value.Trim()
        $enumBody = $match.Groups[2].Value

        $enumValues = @()
        
        $lines = $enumBody -split ','
        foreach ($line in $lines) {
            $cleanLine = $line -replace '//.*', '' -replace '/\*.*?\*/', ''
            $cleanLine = ($cleanLine -split '=')[0].Trim()
            
            if ($cleanLine -match '^[a-zA-Z0-9_]+$') {
                $enumValues += $cleanLine
            }
        }

        if ($enumValues.Count -gt 0) {
            $allEnums += [PSCustomObject]@{
                Name = $enumName
                Values = $enumValues
            }
        }
    }
}

if ($allEnums.Count -eq 0) {
    Write-Host "No enums found in .idl files." -ForegroundColor Yellow
    exit
}

$helpersDir = Join-Path -Path $ProjDir -ChildPath "Helpers"

if (-not (Test-Path -Path $helpersDir)) {
    New-Item -ItemType Directory -Path $helpersDir -Force | Out-Null
}

$hCode = New-Object System.Text.StringBuilder
$hCode.AppendLine("#pragma once") | Out-Null
$hCode.AppendLine("#include <winrt/base.h>") | Out-Null
$hCode.AppendLine() | Out-Null
$hCode.AppendLine("#include <winrt/SystemExplorer.Core.Data.Enums.h>") | Out-Null
$hCode.AppendLine() | Out-Null
$hCode.AppendLine("namespace $HelperNamespace") | Out-Null
$hCode.AppendLine("{") | Out-Null
$hCode.AppendLine("    struct EnumHelper") | Out-Null
$hCode.AppendLine("    {") | Out-Null

foreach ($enum in $allEnums) {
    $hCode.AppendLine("        [[nodiscard]] static winrt::hstring Map(${EnumNamespace}::$($enum.Name) value);") | Out-Null
}

$hCode.AppendLine() | Out-Null
$hCode.AppendLine("        template <typename EnumT>") | Out-Null
$hCode.AppendLine("        [[nodiscard]] static EnumT Map(winrt::hstring const& value);") | Out-Null
$hCode.AppendLine() | Out-Null
$hCode.AppendLine("    private:") | Out-Null
$hCode.AppendLine("        EnumHelper() = default;") | Out-Null
$hCode.AppendLine("    };") | Out-Null
$hCode.AppendLine() | Out-Null

foreach ($enum in $allEnums) {
    $fqn = "${EnumNamespace}::$($enum.Name)"
    $hCode.AppendLine("    template <>") | Out-Null
    $hCode.AppendLine("    $fqn EnumHelper::Map<$fqn>(winrt::hstring const& value);") | Out-Null
}
$hCode.AppendLine("}") | Out-Null

$cppCode = New-Object System.Text.StringBuilder
$cppCode.AppendLine("#include `"pch.h`"") | Out-Null
$cppCode.AppendLine("#include `"EnumHelper.h`"") | Out-Null

if ($availableStrings.Count -gt 0) {
    $winrtHeader = ($StringsHelperNamespace -replace '^winrt::', '') -replace '::', '.'
    $cppCode.AppendLine("#include <winrt/${winrtHeader}.h>") | Out-Null
}

$cppCode.AppendLine("#include <stdexcept>") | Out-Null
$cppCode.AppendLine() | Out-Null
$cppCode.AppendLine("namespace $HelperNamespace") | Out-Null
$cppCode.AppendLine("{") | Out-Null

foreach ($enum in $allEnums) {
    $name = $enum.Name
    $fqn = "${EnumNamespace}::${name}"
    
    $cppCode.AppendLine("    winrt::hstring EnumHelper::Map($fqn value)") | Out-Null
    $cppCode.AppendLine("    {") | Out-Null
    $cppCode.AppendLine("        switch (value)") | Out-Null
    $cppCode.AppendLine("        {") | Out-Null
    foreach ($val in $enum.Values) {
        if ($availableStrings -ccontains $val) {
            $cppCode.AppendLine("            case ${fqn}::${val}: return ${StringsHelperNamespace}::StringsHelper::${val}();") | Out-Null
        } else {
            $cppCode.AppendLine("            case ${fqn}::${val}: return L`"$val`";") | Out-Null
        }
    }
    $cppCode.AppendLine("            default: throw winrt::hresult_invalid_argument(L`"Invalid value for enum $name`");") | Out-Null
    $cppCode.AppendLine("        }") | Out-Null
    $cppCode.AppendLine("    }") | Out-Null
    $cppCode.AppendLine() | Out-Null
    
    $cppCode.AppendLine("    template <>") | Out-Null
    $cppCode.AppendLine("    $fqn EnumHelper::Map<$fqn>(winrt::hstring const& value)") | Out-Null
    $cppCode.AppendLine("    {") | Out-Null
    foreach ($val in $enum.Values) {
        if ($availableStrings -ccontains $val) {
            $cppCode.AppendLine("        if (value == ${StringsHelperNamespace}::StringsHelper::${val}()) return ${fqn}::${val};") | Out-Null
        } else {
            $cppCode.AppendLine("        if (value == L`"$val`") return ${fqn}::${val};") | Out-Null
        }
    }
    $cppCode.AppendLine("        throw winrt::hresult_invalid_argument(L`"Invalid string mapping for enum $name`");") | Out-Null
    $cppCode.AppendLine("    }") | Out-Null
    $cppCode.AppendLine() | Out-Null
}

$cppCode.AppendLine("}") | Out-Null

$headerPath = Join-Path -Path $helpersDir -ChildPath "EnumHelper.h"
$cppPath = Join-Path -Path $helpersDir -ChildPath "EnumHelper.cpp"

[System.IO.File]::WriteAllText($headerPath, $hCode.ToString(), [System.Text.Encoding]::UTF8)
[System.IO.File]::WriteAllText($cppPath, $cppCode.ToString(), [System.Text.Encoding]::UTF8)

Write-Host "Processed enums: $($allEnums.Count)" -ForegroundColor Green
Write-Host "Generated header: $headerPath" -ForegroundColor Green
Write-Host "Generated source: $cppPath" -ForegroundColor Green