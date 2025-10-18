param(
  [string]$Configuration = "Debug",
  [string]$Platform = "x64",
  [string]$OutDir = "",
  [switch]$Run
)

if (-not $OutDir -or $OutDir -eq "") {
  $OutDir = "bin\$Platform\$Configuration\"
}

Write-Host "Building solution: Configuration=$Configuration Platform=$Platform OutDir=$OutDir"
$props = "/p:Configuration=$Configuration;Platform=$Platform;OutDir=$OutDir"
$sol = Join-Path $PSScriptRoot "..\Appartment_Booking_System.sln"
msbuild $sol /t:Build $props
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

if ($Run) {
  $exe = Join-Path $PSScriptRoot "..\$OutDir\App.exe"
  if (Test-Path $exe) {
    Write-Host "Running: $exe"
    & $exe
    exit $LASTEXITCODE
  } else {
    Write-Error "Executable not found: $exe"
    exit 1
  }
}
