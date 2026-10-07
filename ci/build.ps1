[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'

Write-Host 'Firmware build started.'

# TODO: 프로젝트에서 사용하는 컴파일러/IDE와 비대화식 빌드 명령을 확정한 후
# 아래 예외를 실제 빌드 명령으로 교체하십시오.
throw 'CI build command is not configured. Update ci/build.ps1 before enabling automatic builds.'
