param(
  [string]$Configuration = "Debug",
  [string]$Platform = "x64",
  [string]$OutDir = ""
)

if (-not $OutDir -or $OutDir -eq "") {
  $OutDir = "bin\tests\$Platform\$Configuration\"
}

Write-Host "Building tests: Configuration=$Configuration Platform=$Platform OutDir=$OutDir"
$props = "/p:Configuration=$Configuration;Platform=$Platform;OutDir=$OutDir"
$proj = Join-Path $PSScriptRoot "..\Tests\Tests.vcxproj"
msbuild $proj /t:Build $props
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

$exe = Join-Path $PSScriptRoot "..\$OutDir\Tests.exe"
if (Test-Path $exe) {
  Write-Host "Running tests: $exe"
  & $exe
  exit $LASTEXITCODE
} else {
  Write-Error "Tests executable not found: $exe"
  exit 1
}
