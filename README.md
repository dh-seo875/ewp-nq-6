# EWP NQ6

TLE987x 기반 제어기 소프트웨어 프로젝트입니다.

## 저장소 구조

- `Doc/`: 설계, 개발, 시험 및 릴리스 문서
- `Src/`: 애플리케이션, 드라이버, 라이브러리 및 Keil 프로젝트
- `ci/`: GitLab CI에서 실행하는 검증, 빌드 및 산출물 확인 스크립트
- `.gitlab-ci.yml`: GitLab CI/CD 파이프라인 설정
- `.gitlab/merge_request_templates/`: Merge Request 템플릿
- `.gitlab/issue_templates/`: Issue 템플릿

## 개발 및 빌드 환경

- 운영체제: Windows
- 빌드 도구: Keil MDK-ARM v5.25
- Keil 기본 실행 경로: `C:\Keil_v5\UV4\UV4.exe`
- 필수 Device Family Pack: TLE987x
- GitLab Runner 실행 방식: Windows Shell Executor
- GitLab Runner 태그: `windows-keil-v525`

Keil 설치 경로가 기본값과 다른 경우 CI 환경변수 `KEIL_UV4_PATH`에 실제 `UV4.exe` 경로를 설정합니다.

## Keil 프로젝트 정보

- 프로젝트 파일: `Src/iEWP.uvprojx`
- 빌드 Target: `iEWP`
- Object 출력 경로: `Src/Objects/`
- Listing 출력 경로: `Src/Listings/`

## GitLab CI/CD

### 실행 조건

현재 파이프라인은 다음 조건을 모두 만족하는 경우 자동으로 실행됩니다.

- Git Push로 생성된 파이프라인
- 브랜치 이름이 `feature/` 또는 `feature-`로 시작하는 경우

현재 설정에서는 Merge Request 파이프라인과 `main`, `bugfix`, `hotfix`, `release` 브랜치 Push에 대해 자동으로 실행되지 않습니다.

### 파이프라인 단계

파이프라인은 다음 순서로 실행됩니다.

1. `runner_verify`
   - Runner 및 Git 실행 환경 확인
   - Keil 실행 파일 확인
   - 프로젝트와 CI 스크립트 필수 경로 확인
2. `build_firmware`
   - Keil 전체 재빌드 수행
   - 빌드 로그 출력
   - AXF, HEX, MAP 파일 생성 및 복사
3. `artifact_test`
   - AXF, HEX, MAP 파일 존재 여부 확인
   - 각 산출물이 비어 있지 않은지 확인

`artifact_test`는 펌웨어 기능시험이나 단위시험이 아니라 빌드 산출물의 생성 여부를 확인하는 단계입니다.

### 성공 및 실패 기준

- Keil 종료 코드 `0`: 빌드 성공
- Keil 종료 코드 `1`: Warning이 있는 빌드 성공으로 처리
- Keil 종료 코드 `2` 이상: 빌드 실패
- 필수 산출물이 없거나 파일 크기가 0인 경우: 파이프라인 실패

## 빌드 산출물

빌드가 완료되면 다음 파일을 GitLab Job Artifact로 보관합니다.

- `build/iEWP.axf`
- `build/iEWP.hex`
- `build/iEWP.map`
- `build/keil-build.log`

Artifact 보관 기간은 90일입니다. CI 컴퓨터의 작업 폴더는 다음 Job에서 정리되거나 덮어써질 수 있으므로 배포용 파일은 GitLab Artifact에서 내려받습니다.

### Artifact 다운로드

1. GitLab 프로젝트에서 **빌드 > 파이프라인**으로 이동합니다.
2. 성공한 파이프라인을 선택합니다.
3. `build_firmware` Job을 선택합니다.
4. Job Artifact 전체 또는 필요한 `iEWP.hex` 파일을 내려받습니다.

## 브랜치 및 Merge Request 절차

1. 최신 Target Branch를 반영한 작업 브랜치를 생성합니다.
2. 변경사항을 Commit하고 원격 작업 브랜치로 Push합니다.
3. Merge Request를 생성하고 변경 내용과 빌드 및 시험 결과를 기록합니다.
4. Reviewer 승인과 Pipeline 성공을 확인한 후 Merge합니다.

Target Branch의 기본 기준은 다음과 같습니다.

- `feature/*`, `feature-*`, `bugfix/*` → `main`
- `hotfix/*` → 수정 대상 `release/*`
- `hotfix/*` 변경사항은 필요에 따라 `main`에도 반영

## 형상관리 정책

- `Src/iEWP.uvprojx`는 형상관리 대상입니다.
- `Src/iEWP.uvoptx`와 `Src/iEWP.uvguix.USER`는 현재 프로젝트 정책에 따라 형상관리합니다.
- `*.o`, `*.crf`, `*.d`, `*.map`, `*.lst` 등의 빌드 중간 산출물은 Commit하지 않습니다.
- 빌드 결과는 저장소에 직접 Commit하지 않고 GitLab Job Artifact 또는 Release 자산으로 보관합니다.
- 변경 내용이 사용법, 빌드 환경 또는 저장소 구조에 영향을 주는 경우 README나 관련 문서를 함께 수정합니다.

## 보안 주의사항

- 비밀번호, GitLab Runner Token, Access Token 및 개인키를 저장소에 Commit하지 않습니다.
- 민감한 값은 GitLab CI/CD 변수나 별도의 보안 저장소에서 관리합니다.
- 로그, 화면 캡처 및 설정 파일을 공유하기 전에 인증정보가 포함됐는지 확인합니다.

## 문제 해결

- Job이 대기 상태인 경우 `windows-keil-v525` 태그를 가진 Runner가 온라인인지 확인합니다.
- 환경 검증이 실패하면 Keil 실행 경로, TLE987x Pack 설치 여부 및 `Src/iEWP.uvprojx` 존재 여부를 확인합니다.
- Keil 설치 경로가 다르면 GitLab CI/CD 변수 `KEIL_UV4_PATH`를 설정합니다.
- 빌드는 성공했지만 산출물 검증이 실패하면 `Src/Objects/`, `Src/Listings/` 및 `keil-build.log`를 확인합니다.
- 동일 Runner 인증정보를 여러 컴퓨터에서 사용하면 Job이 서로 다른 환경에 배정될 수 있으므로 EWP Runner는 지정된 CI 컴퓨터에서만 실행합니다.
