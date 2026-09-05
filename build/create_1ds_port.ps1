```powershell
$ErrorActionPreference = "Stop"

$projectDir = Resolve-Path "..\SystemExplorer" -ErrorAction SilentlyContinue

if (-not $projectDir -or -not (Test-Path "$projectDir\vcpkg.json")) {
    if (Test-Path ".\vcpkg.json") {
        $projectDir = (Get-Location).Path
    }
    else {
        Write-Error "vcpkg.json not found. Run the script from the build directory or from the directory containing vcpkg.json."
        return
    }
}

Write-Host "[INFO] Project directory found: $projectDir" -ForegroundColor Cyan

$customPortsDir = Join-Path $projectDir "vcpkg-custom-ports\1ds-cpp-sdk"
New-Item -ItemType Directory -Path $customPortsDir -Force | Out-Null

$portVcpkgJson = @'
{
  "name": "1ds-cpp-sdk",
  "version": "1.0.0",
  "description": "Microsoft 1DS C++ Telemetry SDK",
  "homepage": "https://github.com/microsoft/cpp_client_telemetry",
  "dependencies": [
    "zlib",
    "sqlite3",
    "curl",
    "nlohmann-json",
    "vcpkg-cmake",
    "vcpkg-cmake-config"
  ]
}
'@

Write-Host "[INFO] Downloading source archive and calculating SHA512..." -ForegroundColor Cyan

$tempZip = [System.IO.Path]::GetTempFileName()

try {
    Invoke-WebRequest `
        -Uri "https://github.com/microsoft/cpp_client_telemetry/archive/refs/heads/main.zip" `
        -OutFile $tempZip

    $sha512 = [System.Security.Cryptography.SHA512]::Create()

    try {
        $hashBytes = $sha512.ComputeHash(
            [System.IO.File]::ReadAllBytes($tempZip)
        )

        $actualSha512 = ($hashBytes | ForEach-Object {
            $_.ToString("x2")
        }) -join ""
    }
    finally {
        $sha512.Dispose()
    }
}
finally {
    if (Test-Path $tempZip) {
        Remove-Item $tempZip -Force
    }
}

Write-Host "[OK] SHA512 calculated: $actualSha512" -ForegroundColor Green

$portfileCmake = @"
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO microsoft/cpp_client_telemetry
    REF main
    HEAD_REF main
    SHA512 $actualSha512
)

vcpkg_cmake_configure(
    SOURCE_PATH "\${SOURCE_PATH}"
    OPTIONS
        -DMATSDK_BUILD_UNIT_TESTS=OFF
        -DMATSDK_BUILD_FUNC_TESTS=OFF
        -D1DS_BUILD_TESTS=OFF
        -DMATSDK_BUILD_HEADERS=ON
        -DMATSDK_BUILD_LIBRARY=ON
        -DMATSDK_BUILD_PACKAGE=OFF
        -DMATSDK_SQLITE_PROVIDER=SYSTEM
        -DMATSDK_ZLIB_PROVIDER=SYSTEM
)

vcpkg_cmake_install()

file(REMOVE_RECURSE "\${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "\${CURRENT_PACKAGES_DIR}/debug/share")

file(INSTALL "\${SOURCE_PATH}/LICENSE"
     DESTINATION "\${CURRENT_PACKAGES_DIR}/share/\${PORT}"
     RENAME copyright)
"@

[System.IO.File]::WriteAllText(
    "$customPortsDir\vcpkg.json",
    $portVcpkgJson
)

[System.IO.File]::WriteAllText(
    "$customPortsDir\portfile.cmake",
    $portfileCmake
)

Write-Host "[OK] Custom port files created successfully." -ForegroundColor Green

$configPath = Join-Path $projectDir "vcpkg-configuration.json"
$overlayRelPath = "./vcpkg-custom-ports/1ds-cpp-sdk"

if (Test-Path $configPath) {
    $configObj = Get-Content $configPath -Raw | ConvertFrom-Json

    if (-not $configObj.psobject.Properties['overlay-ports']) {
        $configObj | Add-Member `
            -MemberType NoteProperty `
            -Name "overlay-ports" `
            -Value @()
    }

    $currentOverlay = @($configObj.'overlay-ports')

    if ($currentOverlay -notcontains $overlayRelPath) {
        $configObj.'overlay-ports' = [array]$currentOverlay + $overlayRelPath
    }
}
else {
    $configObj = [PSCustomObject]@{
        "default-registry" = [PSCustomObject]@{
            "kind" = "builtin"
        }
        "overlay-ports" = @($overlayRelPath)
    }
}

[System.IO.File]::WriteAllText(
    $configPath,
    ($configObj | ConvertTo-Json -Depth 10)
)

Write-Host "[OK] vcpkg-configuration.json updated." -ForegroundColor Green

$manifestPath = Join-Path $projectDir "vcpkg.json"
$manifestObj = Get-Content $manifestPath -Raw | ConvertFrom-Json

if (-not $manifestObj.psobject.Properties['dependencies']) {
    $manifestObj | Add-Member `
        -MemberType NoteProperty `
        -Name "dependencies" `
        -Value @()
}

$depName = "1ds-cpp-sdk"
$depExists = $false

foreach ($dep in @($manifestObj.dependencies)) {
    if ($dep -is [string] -and $dep -eq $depName) {
        $depExists = $true
        break
    }

    if ($dep -is [PSCustomObject] -and $dep.name -eq $depName) {
        $depExists = $true
        break
    }
}

if (-not $depExists) {
    $manifestObj.dependencies = [array]$manifestObj.dependencies + $depName
}

[System.IO.File]::WriteAllText(
    $manifestPath,
    ($manifestObj | ConvertTo-Json -Depth 10)
)

Write-Host "[OK] Main vcpkg.json updated." -ForegroundColor Green
Write-Host ""
Write-Host "[SUCCESS] 1DS C++ SDK vcpkg overlay is configured successfully." -ForegroundColor Green
```
