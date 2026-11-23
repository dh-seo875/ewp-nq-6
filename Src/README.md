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
