[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version 2.0

try {
    Write-Output 'GitLab Windows Runner connection successful'
    Write-Output "Computer = $env:COMPUTERNAME"
    Write-Output "User = $env:USERNAME"
    Write-Output "Project = $env:CI_PROJECT_PATH"
    Write-Output "Branch = $env:CI_COMMIT_REF_NAME"
    Write-Output "Commit = $env:CI_COMMIT_SHORT_SHA"
    Write-Output "Working directory = $(Get-Location)"
    Write-Output "PowerShell version = $($PSVersionTable.PSVersion)"

    git --version
    if ($LASTEXITCODE -ne 0) {
        throw "Git command failed with exit code $LASTEXITCODE"
    }

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

    Write-Output "File OK: $keilExecutable"
    foreach ($path in $required) {
        Write-Output "Path OK: $path"
    }

    Write-Output 'Runner verification passed.'
    exit 0
}
catch {
    Write-Error $_
    exit 1
}
