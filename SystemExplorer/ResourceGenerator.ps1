[CmdletBinding()]
param(
    [Parameter(Mandatory=$true)]
    [ValidateScript({Test-Path $_ -PathType Leaf})]
    [string]$ReswFilePath,

    [Parameter(Mandatory=$true)]
    [string]$OutputDir,

    [string]$ClassName = "StringsHelper",
    [string]$IdlNamespace = "SystemExplorer.Helpers",
    [string]$GeneratedIncludePrefix = "Helpers"
)

$ErrorActionPreference = "Stop"

$CppNamespace = "winrt::" + ($IdlNamespace -replace '\.', '::')

if (-not (Test-Path -Path $OutputDir)) {
    New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
}

[xml]$xml = Get-Content -Path $ReswFilePath -Encoding UTF8
$dataNodes = $xml.SelectNodes("//root/data")

if ($null -eq $dataNodes -or $dataNodes.Count -eq 0) {
    Write-Warning "Resource file is empty or does not contain //root/data nodes: $ReswFilePath"
    return
}

$idlMethods = [System.Text.StringBuilder]::new()
$headerMethods = [System.Text.StringBuilder]::new()
$cppMethods = [System.Text.StringBuilder]::new()

foreach ($node in $dataNodes) {
    $rawName = $node.name
    $safeName = $rawName -replace '[\.\-\s]', ''

    [void]$idlMethods.AppendLine("        static String $safeName { get; };")
    [void]$headerMethods.AppendLine("        [[nodiscard]] static hstring $safeName();")
    
    $cppMethod = @"
    hstring ${ClassName}::${safeName}()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"$rawName");
    }
"@
    [void]$cppMethods.AppendLine($cppMethod)
    [void]$cppMethods.AppendLine()
}

$idlContent = @"
namespace $IdlNamespace
{
    [default_interface]
    runtimeclass $ClassName
    {
$($idlMethods.ToString().TrimEnd())
    }
}
"@

$headerContent = @"
#pragma once
#include "$GeneratedIncludePrefix/$ClassName.g.h"
#include "../macro.h"
#include <winrt/Microsoft.Windows.ApplicationModel.Resources.h>

namespace $CppNamespace`::implementation
{
    using namespace winrt::Microsoft::Windows::ApplicationModel::Resources;

    struct $ClassName : ${ClassName}T<$ClassName>
    {
$($headerMethods.ToString().TrimEnd())

    private:
        $ClassName() = default;
    };
}
FACTORY($CppNamespace, $ClassName)
"@

$cppContent = @"
#include "pch.h"
#include "$ClassName.h"

#if __has_include("$GeneratedIncludePrefix/$ClassName.g.cpp")
#include "$GeneratedIncludePrefix/$ClassName.g.cpp"
#endif

namespace $CppNamespace`::implementation
{
$($cppMethods.ToString().TrimEnd())
}
"@

$idlPath = Join-Path $OutputDir "$ClassName.idl"
$headerPath = Join-Path $OutputDir "$ClassName.h"
$cppPath = Join-Path $OutputDir "$ClassName.cpp"

Set-Content -Path $idlPath -Value $idlContent -Encoding UTF8
Set-Content -Path $headerPath -Value $headerContent -Encoding UTF8
Set-Content -Path $cppPath -Value $cppContent -Encoding UTF8

Write-Host "Successfully generated $ClassName in $OutputDir" -ForegroundColor Green