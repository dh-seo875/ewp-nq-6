# ewp-nq-6

TLE987x 기반 제어기 소프트웨어 프로젝트입니다.

## 저장소 구조

- `01_Doc/`: 설계, 개발, 시험 및 릴리스 문서
- `02_Src/`: 애플리케이션, 드라이버 및 라이브러리 소스
- `ci/`: CI에서 실행하는 PowerShell 스크립트
- `.gitlab/ci/`: GitLab CI Job 설정
- `.gitlab/merge_request_templates/`: Merge Request 템플릿
- `.gitlab/issue_templates/`: Issue 템플릿

## 빌드

빌드 도구와 비대화식 명령을 확정한 후 `ci/build.ps1`의 TODO 구간을 구현합니다.
현재 GitLab의 Build Job은 수동 실행으로 설정되어 있으며, 빌드 명령이 구성되기 전에는 실패하도록 되어 있습니다.

## 개발 절차

1. 최신 `main`에서 `feature/<Issue번호>-<설명>` 또는 `bugfix/<Issue번호>-<설명>` 브랜치를 생성합니다.
2. 변경 사항을 Commit하고 원격 브랜치로 Push합니다.
3. Merge Request를 만들고 빌드 및 시험 결과를 기록합니다.
4. Reviewer 승인과 Pipeline 확인 후 Merge합니다.

## 산출물

빌드 결과는 저장소에 직접 Commit하지 않고 GitLab Job Artifact 또는 Release 자산으로 보관합니다.
