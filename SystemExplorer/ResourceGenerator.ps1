param(
    [Parameter(Mandatory=$true)][string]$ReswFilePath,
    [Parameter(Mandatory=$true)][string]$OutputDir,
    [string]$ClassName = "StringsHelper",
    [string]$IdlNamespace = "SystemExplorer.Helpers",
    [string]$GeneratedIncludePrefix = "Helpers"
)

$CppNamespace = "winrt::" + ($IdlNamespace -replace '\.', '::')

if (-Not (Test-Path $ReswFilePath)) {
    Write-Error "File not found: $ReswFilePath"
    exit 1
}

if (-Not (Test-Path $OutputDir)) {
    New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
}

[xml]$xml = Get-Content $ReswFilePath -Encoding UTF8
$dataNodes = $xml.SelectNodes("//root/data")

$idlMethods = [System.Collections.Generic.List[string]]::new()
$headerMethods = [System.Collections.Generic.List[string]]::new()
$cppMethods = [System.Collections.Generic.List[string]]::new()

foreach ($node in $dataNodes) {
    $rawName = $node.name
    $safeName = $rawName -replace '[\.\-\s]', ''

    $idlMethods.Add("        static String $safeName { get; };")
    $headerMethods.Add("       [[nodiscard]] static hstring $safeName();")
    
    $cppMethods.Add(@"
    hstring $ClassName`::$safeName()
    {
        return loader_.GetString(L"$rawName");
    }
"@)
}

$idlMethodsString = $idlMethods -join "`n"
$headerMethodsString = $headerMethods -join "`n"
$cppMethodsString = $cppMethods -join "`n`n"

$idlContent = @"
namespace $IdlNamespace
{
    [default_interface]
    runtimeclass $ClassName
    {
$idlMethodsString
    }
}
"@

$headerContent = @"
#pragma once
#include "$GeneratedIncludePrefix/$ClassName.g.h"
#include <winrt/Microsoft.Windows.ApplicationModel.Resources.h>

namespace $CppNamespace`::implementation
{
    using namespace winrt::Microsoft::Windows::ApplicationModel::Resources;

    struct $ClassName : ${ClassName}T<$ClassName>
    {
$headerMethodsString

    private:
        $ClassName() = default;

        static ResourceLoader loader_;
    };
}

namespace $CppNamespace`::factory_implementation
{
    struct $ClassName : ${ClassName}T<$ClassName, implementation::$ClassName>
    {
    };
}
"@

$cppContent = @"
#include "pch.h"
#include "$ClassName.h"

#if __has_include("$GeneratedIncludePrefix/$ClassName.g.cpp")
#include "$GeneratedIncludePrefix/$ClassName.g.cpp"
#endif

namespace $CppNamespace`::implementation
{
    winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader $ClassName`::loader_{};

$cppMethodsString
}
"@

$idlPath = Join-Path $OutputDir "$ClassName.idl"
$headerPath = Join-Path $OutputDir "$ClassName.h"
$cppPath = Join-Path $OutputDir "$ClassName.cpp"

Set-Content -Path $idlPath -Value $idlContent -Encoding UTF8
Set-Content -Path $headerPath -Value $headerContent -Encoding UTF8
Set-Content -Path $cppPath -Value $cppContent -Encoding UTF8

Write-Host "Successfully generated $ClassName in $OutputDir" -ForegroundColor Green