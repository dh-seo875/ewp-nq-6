# TE1 BAT Type3 SW 변경사항

NQ6 기준 SW에 반영한 TE1 BAT Type3 Hotfix 변경사항을 기록합니다.

### 모터 및 FOC 제어 파라미터

- Speed PI Kp/Ki: `700/500` -> `1200/400`
- Current PI Kp/Ki: `300/500` -> `800/500`
- Start Current 초기값: `150` -> `196`
- Start Current 증가 Step: `20` -> `23`
- Start Current 상한: `1572` -> `1967`
- SVM Deadtime: `60` -> `30`
- Phase Resistance 환산값: `12379` -> `28937`
- Phase Inductance 환산값: `3822` -> `10256`
- Open Loop에서 Closed Loop 전환 시 Speed PI IOut 사전 설정 배율: `8192` -> `32768`

### Open Loop에서 Closed Loop 전환

- Open Loop 속도가 900 rpm에 도달하면 Start Angle과 Flux Angle PLL의 오차를 확인합니다.
- 허용 각도 오차는 `+/-3000`입니다.
- 전류벡터각은 `192~512` 범위에서 `2`씩 보정합니다.
- 각도 한계 도달 후 Settling `20 Tick`과 Timeout `10 Tick`을 적용합니다.
- 900 rpm 도달 후 최대 `50 Tick` 동안 전환되지 않으면 Timeout으로 Closed Loop 전환을 수행합니다.
- 전류벡터각에 따라 `RefCurrD`와 `RefCurrQ`를 생성하여 D/Q축 전류 PI 입력에 반영합니다.
- 정상 전환 시 전류벡터각을 즉시 기준값 `256`으로 설정하여 순수 Q축 제어로 전환합니다.
- Timeout 전환 시 Closed Loop에서 전류벡터각을 기준값 `256`으로 단계적으로 복귀시킵니다.
- 정지 및 재기동 시 `StartSpeedSlope`와 전환 관련 변수를 초기화합니다.

### 운전 범위 및 속도 Step 제어

- 최대 운전속도: `6000 rpm` -> `4600 rpm`
- Limp-home 속도: `6000 rpm` -> `4600 rpm`
- 최소 운전속도: `1000 rpm`
- 정지 상태 기동 및 저속 감속 Step: `15` -> `12`
- 가속 Step: `300` -> `120`

### 고장진단

- Speed Error Case 1 기준: `2000 rpm` -> `1865 rpm`
- Speed Error Case 2 하한: `1625 rpm` -> `1565 rpm`
- Speed Error 복귀 편차: `625 rpm`
- Low Current
  - 진입: `1.56 A` -> `1.0 A`
  - 복귀: `2.34 A` -> `1.5 A`
  - 진단 속도: `3500 rpm` -> `2300 rpm`
- BAT 사양에서 사용하지 않는 Warning Current, Stall Error, Phase Open, LIN Invalid Value 및 Initial 진단 호출을 비활성화했습니다.

### LIN 통신

- Duty 기반 지령을 `20 rpm/bit` 속도지령 방식으로 변경했습니다.
- 수신 Raw 유효 범위는 `50~230`이며 운전 범위는 `1000~4600 rpm`입니다.
- 지령 수신 신호를 `Ewp2_Target_Spd_RPM`으로 변경했습니다.
- 실제속도 응답도 `20 rpm/bit`로 표출합니다.
- BAT 통신 맵에 맞춰 FaultStatus, ProtectMode 및 개별 진단 신호의 Byte/Bit 위치를 변경했습니다.
- LIN 응답 Byte 3에 EEPROM Error Code Index를 표출합니다.
- LIN 응답 프레임의 예약 비트 초기값을 적용했습니다.
- `BEWP_TYPE3_0X38` 값을 `0x0A`로 적용했습니다.
- `VERSION=1` 기준 LIN `Ewp1_Version` 표출값은 `0xA1`입니다.

### 고장 진단 이력 NVM 저장 조건 개선

- 대상: `mhLib/Bsw/Nvm.c`의 `ManageNvm()`.
- 기존에는 저장 요청(`Nvm_errorCode != 0u`)이 있을 때 LIN 성공 플래그가 `1`이거나, 강제 저장 플래그가 `1`이면서 모터가 STOP인 경우에 저장했습니다.
- LIN 성공 플래그는 약 1ms 동안 유지되지만 NVM 태스크는 10ms 주기로 실행되어 성공 표시를 놓칠 수 있었습니다. COMMAND_EWP가 계속 수신되면 강제 저장 조건인 약 251ms 미수신에도 도달하지 못합니다.
- 개선 후에는 LIN 관련 조건을 제거하고, 저장 요청이 있으며 `Emo_Status.MotorState == EMO_MOTOR_STATE_STOP`인 경우 저장합니다. 모터가 STOP이 아니면 저장 요청을 유지합니다.
- 저장 호출 후 `Nvm_errorCode`를 `0`으로 초기화하는 기존 처리는 유지합니다. 반환값에 따른 재시도는 추가하지 않았으며, Flash 쓰기가 정상이라는 전제로 저장 진입 조건을 개선했습니다.
- LIN 통신 중에도 저장할 수 있으므로 Flash 처리 중 통신 누락 가능성은 별도 시험으로 확인해야 합니다.

#### 기존 개선 검증 시 디버깅 실측 결과

아래는 기존 개선 검증의 관측값이며, 현 BAT 사양에서 새로 수행한 시험 결과는 아닙니다. Sleep 전환 직전의 누적값을 비교했으며, 대기 시간은 NVM 태스크의 10ms 주기를 기준으로 환산했습니다.

| 평가 조건 | 저장 시도 횟수 | 조건 미충족 대기 횟수 | 마지막 저장 진입 경로 | 쓰기 반환값 |
| --- | ---: | ---: | --- | --- |
| 개선 전 미저장 시험 2회 | 각 0 | 각 500 (약 5초) | 미진입 | 미호출 |
| 개선 전 저장 시험 | 1 | 113 (약 1.13초) | LIN 성공 조건 | 0 |
| 개선 후 시험 2회 | 각 1 | 각 0 | 모터 STOP 조건 | 각 0 |

- 개선 전 미저장 시험에서는 저장 요청이 남은 상태로 Sleep 전환에 도달했습니다.
- 개선 후 시험에서는 조건 대기 없이 저장을 호출하고 Sleep 전환 전에 저장 요청을 처리했습니다. 위 결과는 해당 시험의 관측값이며 전체 조건의 저장 보장을 의미하지 않습니다.
- 검증에 사용한 디버깅 변수는 최종 코드에서 제거했습니다.

### 구속(Stall) 진단 이력 코드 추가

- 대상: `mhLib/Bsw/Nvm.c`의 `convertErrorCode()`.
- 기존에는 Stall 진단 결과가 Fault Byte1의 bit 4(`0x10`)로 전달돼도 변환 case가 없어 `0`을 반환했으며, 해당 Stall 이력을 배열에 추가하지 못했습니다.
- `case 0x110`을 추가하여 Stall 단독 신규 발생을 이력 코드 `14`(`0x0E`)로 변환합니다. 기존 이력 배열 구성과 조회 응답 경로를 사용합니다.
- 실제 Flash 저장에는 기존 저장 요청 및 모터 STOP 조건이 적용됩니다. 이 변경 자체가 저장 요청을 생성하지는 않습니다.
- 진단기에서는 주행 사이클 표시 비트를 기존 방식으로 분리한 뒤 코드 `14`를 Stall로 표시하도록 매핑해야 합니다.
- 다른 Fault와 동시에 신규 발생한 조합값은 단일 `case 0x110`으로 처리하지 않습니다. Stall 저장 및 진단기 표출의 실기 검증은 별도로 필요합니다.
- 현 BAT 사양은 Stall 진단 호출을 비활성화하므로, 변환 case 추가만으로 Stall 진단이 활성화되지는 않습니다.

### CI 파이프라인

- `feature/...`, `feature-...` 브랜치와 함께 `hotfix/...`, `hotfix-...` 브랜치의 push에서도 파이프라인이 실행되도록 변경했습니다.

### 변경 파일

- `.gitlab-ci.yml`
- `Src/app/linmain.c`
- `Src/emo/Emo.c`
- `Src/emo/Emo_RAM.c`
- `Src/emo/Emo_RAM.h`
- `Src/lin_driver/genLinConfig.c`
- `Src/lin_driver/genLinConfig.h`
- `Src/lin_driver/lin_main.c`
- `Src/mhLib/Rte/RteApp.c`
- `Src/mhLib/Rte/RteApp.h`
- `Src/mhLib/Rte/RteHal.c`
- `Src/mhLib/Rte/RteHal.h`
- `Src/mhLib/Swc/Diag.c`
- `Src/mhLib/Swc/Diag_cfg.h`
- `Src/mhLib/Swc/Manage.c`
- `Src/mhLib/Swc/Manage.h`
- `Src/mhLib/Swc/Manage_cfg.h`
- `Src/mhLib/Swc/Sensor.c`
- `Src/mhLib/Swc/Sensor.h`
- `Src/mhLib/Variant_cfg.h`
- `Src/mhLib/Bsw/Nvm.c`
