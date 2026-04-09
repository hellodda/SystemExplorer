param(
    [Parameter(Mandatory=$true)]
    [string]$ReswFilePath,       # Путь к файлу Resources.resw

    [Parameter(Mandatory=$true)]
    [string]$OutputDir,          # Папка, куда сохранять сгенерированные файлы

    [string]$ClassName = "StringsHelper",
    [string]$IdlNamespace = "SystemExplorer.Helpers",
    [string]$GeneratedIncludePrefix = "Helpers" # Для #include "Helpers/StringsHelper.g.cpp"
)

# Вычисляем C++ namespace из IDL namespace
$CppNamespace = "winrt::" + ($IdlNamespace -replace '\.', '::')

# Проверяем наличие resw файла
if (-Not (Test-Path $ReswFilePath)) {
    Write-Error "Файл ресурсов не найден: $ReswFilePath"
    exit 1
}

# Создаем выходную директорию, если её нет
if (-Not (Test-Path $OutputDir)) {
    New-Item -ItemType Directory -Path $OutputDir | Out-Null
}

# Читаем .resw как XML
[xml]$xml = Get-Content $ReswFilePath -Encoding UTF8
$dataNodes = $xml.SelectNodes("//root/data")

$idlMethods = @()
$headerMethods = @()
$cppMethods = @()

foreach ($node in $dataNodes) {
    $rawName = $node.name
    
    # Преобразуем имя: просто удаляем точки (например, Button.Text -> ButtonText)
    # Также убираем пробелы и тире на всякий случай, чтобы точно получить валидный C++ идентификатор
    $safeName = $rawName -replace '[\.\-\s]', ''

    # Генерируем строки для каждого файла
    $idlMethods += "        static String $safeName { get; };"
    $headerMethods += "        static hstring $safeName();"
    
    $cppMethods += @"
    hstring $ClassName`::$safeName()
    {
        return loader_.GetString(L"$rawName");
    }
"@
}

$idlMethodsString = $idlMethods -join "`n"
$headerMethodsString = $headerMethods -join "`n"
$cppMethodsString = $cppMethods -join "`n`n"

# ==========================================
# 1. Генерация IDL
# ==========================================
$idlContent = @"
namespace $IdlNamespace
{
    [default_interface]
    runtimeclass $ClassName
    {
        $ClassName();

$idlMethodsString
    }
}
"@

# ==========================================
# 2. Генерация Header (.h)
# ==========================================
$headerContent = @"
#pragma once
#include "$GeneratedIncludePrefix/$ClassName.g.h"

namespace $CppNamespace`::implementation
{
    using namespace winrt::Microsoft::Windows::ApplicationModel::Resources;

    struct $ClassName : ${ClassName}T<$ClassName>
    {
        $ClassName() = default;

$headerMethodsString

    private:
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

# ==========================================
# 3. Генерация Implementation (.cpp)
# ==========================================
$cppContent = @"
#include "pch.h"
#include "$ClassName.h"

#if __has_include("$GeneratedIncludePrefix/$ClassName.g.cpp")
#include "$GeneratedIncludePrefix/$ClassName.g.cpp"
#endif

namespace $CppNamespace`::implementation
{
    // Инициализация статического члена
    winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader $ClassName`::loader_;

$cppMethodsString
}
"@

# ==========================================
# Сохранение файлов
# ==========================================
$idlPath = Join-Path $OutputDir "$ClassName.idl"
$headerPath = Join-Path $OutputDir "$ClassName.h"
$cppPath = Join-Path $OutputDir "$ClassName.cpp"

# Записываем с BOM UTF-8, как это любит Visual Studio
Set-Content -Path $idlPath -Value $idlContent -Encoding UTF8
Set-Content -Path $headerPath -Value $headerContent -Encoding UTF8
Set-Content -Path $cppPath -Value $cppContent -Encoding UTF8

Write-Host "Успешно сгенерирован $ClassName в $OutputDir" -ForegroundColor Green