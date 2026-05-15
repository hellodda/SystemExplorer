param (
    [Parameter(Mandatory=$true)]
    [string]$JsonPath,
    [string]$OutputDir = "NIL",
    [string]$Namespace = "winrt::SystemExplorer::Core::Data::Models::Json"
)

if (!(Test-Path $JsonPath)) { 
    Write-Error "JSON file not found at: $JsonPath"
    exit 1 
}

if (!(Test-Path $OutputDir)) { 
    New-Item -ItemType Directory -Path $OutputDir | Out-Null
}

function Get-PascalCase ([string]$name) {
    if ([string]::IsNullOrWhiteSpace($name)) { return "Root" }
    return ($name -split '[_-]' | ForEach-Object { 
        if ($_.Length -gt 0) {
            $first = $_.Substring(0,1).ToUpper()
            $rest = ""
            if ($_.Length -gt 1) {
                $rest = $_.Substring(1)
            }
            $first + $rest
        }
    }) -join ''
}

function Get-CppType ($value, $name) {
    if ($value -is [PSCustomObject]) { return Get-PascalCase $name }
    if ($value -is [int] -or $value -is [long]) { return "int" }
    if ($value -is [double] -or $value -is [float]) { return "double" }
    if ($value -is [bool]) { return "bool" }
    return "winrt::hstring"
}

$GeneratedClasses = @{}

function Process-Object ($obj, $className) {
    if ($GeneratedClasses.ContainsKey($className)) { return }
    
    $props = @()
    foreach ($p in $obj.psobject.Properties) {
        $cppType = Get-CppType $p.Value $p.Name
        $isObject = $p.Value -is [PSCustomObject]
        
        $props += [PSCustomObject]@{
            RawName = $p.Name
            PascalName = Get-PascalCase $p.Name
            Type = $cppType
            IsObject = $isObject
        }
        
        if ($isObject) { Process-Object $p.Value $cppType }
    }
    
    $GeneratedClasses[$className] = $props
    Write-ClassFiles $className $props
}

function Write-ClassFiles ($className, $props) {
    $hPath = Join-Path $OutputDir "$className.h"
    $cppPath = Join-Path $OutputDir "$className.cpp"

    $h = @"
#pragma once
#include <winrt/Windows.Data.Json.h>
$( ($props | Where-Object { $_.IsObject } | ForEach-Object { "#include `"$($_.Type).h`"" }) -join "`n" )

namespace $Namespace 
{
    class $className 
    {
    public:
        $className() = default;

        void FromJson(winrt::Windows::Data::Json::JsonObject const& json);
        [[nodiscard]] winrt::Windows::Data::Json::JsonObject ToJson() const;


"@
    foreach ($p in $props) {
        $h += "        [[nodiscard]] $($p.Type) $($p.PascalName)() const noexcept;`n"
        $h += "        void $($p.PascalName)($($p.Type) const& value) noexcept;`n`n"
    }

    $h += "    private:`n"
    foreach ($p in $props) {
        $mVar = if ($p.Type -eq "bool" -and !($p.RawName.StartsWith("is"))) { "is" + $p.PascalName + "_" } else { $p.RawName + "_" }
        $def = switch($p.Type) { "int" {"0"} "double" {"0.0"} "bool" {"false"} "winrt::hstring" {"L`"`""} Default {"{}"} }
        $h += "        $($p.Type) $mVar = $def;`n"
    }
    $h += "    };`n}"

    $cpp = @"
#include "pch.h"
#include "$className.h"

namespace $Namespace
{
    void $className`::FromJson(winrt::Windows::Data::Json::JsonObject const& json)
    {
        if (!json) return;
"@
    foreach ($p in $props) {
        $mVar = if ($p.Type -eq "bool" -and !($p.RawName.StartsWith("is"))) { "is" + $p.PascalName + "_" } else { $p.RawName + "_" }
        if ($p.IsObject) {
            $cpp += "`n        if (json.HasKey(L`"$($p.RawName)`")) $mVar.FromJson(json.GetNamedObject(L`"$($p.RawName)`"));"
        } else {
            $getter = switch($p.Type) { "int" {"GetNamedNumber"} "double" {"GetNamedNumber"} "bool" {"GetNamedBoolean"} Default {"GetNamedString"} }
            $cast = if ($p.Type -eq "int") { "static_cast<int>" } else { "" }
            $cpp += "`n        if (json.HasKey(L`"$($p.RawName)`")) $mVar = $cast(json.$getter(L`"$($p.RawName)`"));"
        }
    }

    $cpp += "`n    }`n`n    winrt::Windows::Data::Json::JsonObject $className`::ToJson() const`n    {`n        winrt::Windows::Data::Json::JsonObject json;"
    foreach ($p in $props) {
        $mVar = if ($p.Type -eq "bool" -and !($p.RawName.StartsWith("is"))) { "is" + $p.PascalName + "_" } else { $p.RawName + "_" }
        if ($p.IsObject) {
            $cpp += "`n        json.Insert(L`"$($p.RawName)`", $mVar.ToJson());"
        } else {
            $creator = switch($p.Type) { "int" {"CreateNumberValue"} "double" {"CreateNumberValue"} "bool" {"CreateBooleanValue"} Default {"CreateStringValue"} }
            $cpp += "`n        json.Insert(L`"$($p.RawName)`", winrt::Windows::Data::Json::JsonValue::$creator($mVar));"
        }
    }
    $cpp += "`n        return json;`n    }`n"

    foreach ($p in $props) {
        $mVar = if ($p.Type -eq "bool" -and !($p.RawName.StartsWith("is"))) { "is" + $p.PascalName + "_" } else { $p.RawName + "_" }
        $cpp += "`n    $($p.Type) $className`::$($p.PascalName)() const noexcept { return $mVar; }"
        $cpp += "`n    void $className`::$($p.PascalName)($($p.Type) const& value) noexcept { $mVar = value; }`n"
    }
    $cpp += "}"
    $h | Out-File $hPath -Encoding utf8
    $cpp | Out-File $cppPath -Encoding utf8
}
$FileNameOnly = [System.IO.Path]::GetFileNameWithoutExtension($JsonPath)
$RootClassName = Get-PascalCase $FileNameOnly

$rootJson = Get-Content $JsonPath -Raw | ConvertFrom-Json
Process-Object $rootJson $RootClassName

Write-Host "Success: $($GeneratedClasses.Count) classes generated."
Write-Host "Root class: $RootClassName"