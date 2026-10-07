[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version 2.0

try {
    $artifactDirectory = Join-Path $env:CI_PROJECT_DIR 'build'
    $artifactNames = @(
        'iEWP.axf',
        'iEWP.hex',
        'iEWP.map'
    )

    Write-Output "Artifact directory = $artifactDirectory"

    foreach ($artifactName in $artifactNames) {
        $artifactPath = Join-Path $artifactDirectory $artifactName

        if (-not (Test-Path -LiteralPath $artifactPath -PathType Leaf)) {
            throw "Build artifact not found: $artifactPath"
        }

        $artifact = Get-Item -LiteralPath $artifactPath
        if ($artifact.Length -le 0) {
            throw "Build artifact is empty: $artifactPath"
        }

        Write-Output ("Artifact OK: {0} ({1} bytes)" -f $artifact.Name, $artifact.Length)
    }

    Write-Output 'iEWP artifact test passed.'
    exit 0
}
catch {
    Write-Error $_
    exit 1
}
