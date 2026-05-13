[CmdletBinding()]
param (
    [Parameter(Mandatory=$true, Position=0, HelpMessage="Путь к директории проекта (ProjDir)")]
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

$availableStrings = [System.Collections.Generic.HashSet[string]]::new([StringComparer]::Ordinal)

if ([string]::IsNullOrWhiteSpace($ReswFilePath)) {
    $foundResw = Get-ChildItem -Path $ProjDir -Filter "Resources.resw" -Recurse | Select-Object -First 1
    if ($foundResw) {
        $ReswFilePath = $foundResw.FullName
    }
}

if (-Not [string]::IsNullOrWhiteSpace($ReswFilePath) -and (Test-Path $ReswFilePath)) {
    try {
        [xml]$xml = [System.IO.File]::ReadAllText($ReswFilePath, [System.Text.Encoding]::UTF8)
        $dataNodes = $xml.SelectNodes("//root/data")
        foreach ($node in $dataNodes) {
            $safeName = $node.name -replace '[\.\-\s]', ''
            [void]$availableStrings.Add($safeName)
        }
        Write-Host "Loaded $($availableStrings.Count) resource keys from $ReswFilePath" -ForegroundColor Cyan
    } catch {
        Write-Warning "Failed to parse XML in $ReswFilePath. Details: $_"
    }
} else {
    Write-Host "Resw file not found. Falling back to string literals only." -ForegroundColor Yellow
}

$idlFiles = Get-ChildItem -Path $ProjDir -Filter "*.idl" -File -Recurse
if ($idlFiles.Count -eq 0) {
    Write-Host "No .idl files found in '$ProjDir'." -ForegroundColor Yellow
    exit
}

$allEnums = [System.Collections.Generic.List[PSCustomObject]]::new()
$enumRegex = [regex]::new('(?s)\benum\s+([a-zA-Z0-9_]+)\s*\{([^}]+)\}')

foreach ($file in $idlFiles) {
    $content = [System.IO.File]::ReadAllText($file.FullName)
    $matches = $enumRegex.Matches($content)

    foreach ($match in $matches) {
        $enumName = $match.Groups[1].Value.Trim()
        $enumBody = $match.Groups[2].Value

        $enumValues = [System.Collections.Generic.List[string]]::new()
        
        $lines = $enumBody -split ','
        foreach ($line in $lines) {
            $cleanLine = $line -replace '//.*', '' -replace '/\*.*?\*/', ''
            $cleanLine = ($cleanLine -split '=')[0].Trim()
            
            if ($cleanLine -match '^[a-zA-Z0-9_]+$') {
                [void]$enumValues.Add($cleanLine)
            }
        }

        if ($enumValues.Count -gt 0) {
            $allEnums.Add([PSCustomObject]@{
                Name   = $enumName
                Values = $enumValues.ToArray()
            })
        }
    }
}

if ($allEnums.Count -eq 0) {
    Write-Host "No enums found in .idl files." -ForegroundColor Yellow
    exit
}

$helpersDir = Join-Path -Path $ProjDir -ChildPath "Helpers"
if (-not (Test-Path -Path $helpersDir)) {
    [void](New-Item -ItemType Directory -Path $helpersDir -Force)
}

$hCode = [System.Text.StringBuilder]::new()
[void]$hCode.AppendLine(@"
#pragma once
#include <winrt/base.h>
#include <winrt/SystemExplorer.Core.Data.Enums.h>
#include <type_traits>
#include <string_view>

namespace $HelperNamespace
{
    struct EnumHelper
    {
        EnumHelper() = delete;
"@)

foreach ($enum in $allEnums) {
    [void]$hCode.AppendLine("        [[nodiscard]] static winrt::hstring Map(${EnumNamespace}::$($enum.Name) value);")
}

[void]$hCode.AppendLine(@"

        template <typename EnumT>
            requires std::is_enum_v<EnumT>
        [[nodiscard]] static EnumT Map(winrt::hstring const& value);
    };
"@)

foreach ($enum in $allEnums) {
    $fqn = "${EnumNamespace}::$($enum.Name)"
    [void]$hCode.AppendLine("    template <>")
    [void]$hCode.AppendLine("    $fqn EnumHelper::Map<$fqn>(winrt::hstring const& value);")
}
[void]$hCode.AppendLine("}")

$cppCode = [System.Text.StringBuilder]::new()
[void]$cppCode.AppendLine(@"
#include "pch.h"
#include "EnumHelper.h"
#include <stdexcept>
"@)

if ($availableStrings.Count -gt 0) {
    $winrtHeader = ($StringsHelperNamespace -replace '^winrt::', '') -replace '::', '.'
    [void]$cppCode.AppendLine("#include <winrt/${winrtHeader}.h>")
}

[void]$cppCode.AppendLine("`nnamespace $HelperNamespace`n{")

foreach ($enum in $allEnums) {
    $name = $enum.Name
    $fqn = "${EnumNamespace}::${name}"
    
    [void]$cppCode.AppendLine("    winrt::hstring EnumHelper::Map($fqn value)`n    {`n        switch (value)`n        {")
    foreach ($val in $enum.Values) {
        if ($availableStrings.Contains($val)) {
            [void]$cppCode.AppendLine("            case ${fqn}::${val}: return ${StringsHelperNamespace}::StringsHelper::${val}();")
        } else {
            [void]$cppCode.AppendLine("            case ${fqn}::${val}: return L`"$val`";")
        }
    }
    [void]$cppCode.AppendLine("        }`n        throw winrt::hresult_invalid_argument(L`"Invalid value for enum $name`");`n    }`n")
    
    [void]$cppCode.AppendLine("    template <>`n    $fqn EnumHelper::Map<$fqn>(winrt::hstring const& value)`n    {")
    
    $hasStrings = $false
    foreach ($val in $enum.Values) {
        if ($availableStrings.Contains($val)) {
            [void]$cppCode.AppendLine("        if (value == ${StringsHelperNamespace}::StringsHelper::${val}()) return ${fqn}::${val};")
        } else {
            [void]$cppCode.AppendLine("        if (value == std::wstring_view{L`"$val`"}) return ${fqn}::${val};")
        }
    }
    [void]$cppCode.AppendLine("        throw winrt::hresult_invalid_argument(L`"Invalid string mapping for enum $name`");`n    }`n")
}

[void]$cppCode.AppendLine("}")

$headerPath = Join-Path -Path $helpersDir -ChildPath "EnumHelper.h"
$cppPath    = Join-Path -Path $helpersDir -ChildPath "EnumHelper.cpp"

[System.IO.File]::WriteAllText($headerPath, $hCode.ToString(), [System.Text.Encoding]::UTF8)
[System.IO.File]::WriteAllText($cppPath, $cppCode.ToString(), [System.Text.Encoding]::UTF8)

Write-Host "Processed enums: $($allEnums.Count)" -ForegroundColor Green
Write-Host "Generated header: $headerPath" -ForegroundColor Green
Write-Host "Generated source: $cppPath" -ForegroundColor Green