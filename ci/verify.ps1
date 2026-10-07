[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version 2.0

try {
    Write-Host "Runner computer: $env:COMPUTERNAME"
    Write-Host "Runner user: $env:USERNAME"
    Write-Host "Project: $env:CI_PROJECT_PATH"
    Write-Host "Branch: $env:CI_COMMIT_REF_NAME"
    Write-Host "Commit: $env:CI_COMMIT_SHORT_SHA"

    $required = @(
        'README.md',
        'Doc',
        'Src',
        'Src/iEWP.uvprojx',
        'ci/build.bat',
        'ci/build.ps1',
        'ci/verify.ps1'
    )

    $missing = $required | Where-Object { -not (Test-Path -LiteralPath $_) }
    if ($missing) {
        throw "Missing required paths: $($missing -join ', ')"
    }

    $keilExecutable = if ($env:KEIL_UV4_PATH) {
        $env:KEIL_UV4_PATH
    }
    else {
        'C:\Keil_v5\UV4\UV4.exe'
    }

    if (-not (Test-Path -LiteralPath $keilExecutable -PathType Leaf)) {
        throw "Keil executable not found: $keilExecutable"
    }

    Write-Host 'Repository and Keil environment verification passed.'
    exit 0
}
catch {
    Write-Error $_
    exit 1
}
