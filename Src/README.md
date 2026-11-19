# TE1 PE SW 변경사항

NQ6 기준 SW에 반영한 TE1 PE 사양 변경사항을 기록합니다.

TE1 PE 사양에 맞춰 전류 센싱, 진단 기준, 모터 제어 파라미터와 Open Loop에서 Closed Loop로 전환하는 동작을 변경했습니다.

### 전류 센싱 및 진단 기준

- Iq 환산 계수: `100 / 197` -> `100 / 131`
- Open Phase 판정 범위: `+/-7` -> `+/-5`
- Warning Current
  - 진입: `14.5 A` -> `18.0 A`
  - 복귀: `13.5 A` -> `16.5 A`
- Over Current: `16.0 A` -> `20.0 A`
- Low Current
  - 진입: `1.56 A` -> `2.04 A`
  - 복귀: `2.34 A` -> `3.06 A`
  - 진단 속도: `3500 rpm` -> `3250 rpm`

### 모터 및 FOC 제어 파라미터

- Start Current: `196` -> `130`
- Start Current 증가 Step: `20` -> `15`
- Start Current 상한: `1572` -> `1311`
- Speed PI Kp/Ki: `700/500` -> `500/300`
- Open Loop에서 Closed Loop 전환 시 Speed PI IOut 사전 설정 배율: `8192` -> `32768`
- Current PI Kp/Ki: `300/500` -> `600/300`
- SVM Deadtime: `60` -> `30`
- Phase Resistance 환산값: `12379` -> `9102`
- Phase Inductance 환산값: `3822` -> `5500`

### Open Loop에서 Closed Loop 전환

- Open Loop 속도가 900 rpm에 도달하면 Start Angle과 Flux Angle PLL의 오차를 확인합니다.
- 허용 각도 오차는 `+/-3000`입니다.
- 전류 벡터 각도는 `192~512` 범위에서 `2`씩 보정합니다.
- 각도 한계 도달 후 Settling `20 Tick`과 Timeout `10 Tick`을 적용합니다.
- 900 rpm 도달 후 최대 `50 Tick` 동안 전환되지 않으면 Timeout으로 Closed Loop 전환을 수행합니다.
- 전류 벡터 각도에 따라 `RefCurrD`와 `RefCurrQ`를 생성하여 D/Q축 전류 PI 입력에 반영합니다.
- Timeout으로 전환하면 Closed Loop에서 전류 벡터 각도를 기준값 `256`으로 단계적으로 복귀시킵니다.
- 재기동 시 이전 기동 상태가 남지 않도록 `StartSpeedSlope`와 전환 관련 변수를 초기화합니다.

### 운전 범위 및 LIN 버전

- 최대 운전속도: `6000 rpm` -> `5500 rpm`
- Limp-home 속도: `6000 rpm` -> `5500 rpm`
- 센서 RPM 상한: `6000 rpm` -> `5500 rpm`
- SW Version: `1` -> `10`
- LIN `Ewp1_Version`: `(BEWP_TYPE3_0X38 << 4) | VERSION` = `0xEA`

### 변경 파일

- `emo/Emo.c`
- `emo/Emo_RAM.c`
- `emo/Emo_RAM.h`
- `mhLib/Swc/Diag_cfg.h`
- `mhLib/Swc/Manage_cfg.h`
- `mhLib/Swc/Sensor.c`
- `mhLib/Swc/Sensor.h`
- `mhLib/Swc/Sensor_cfg.h`
- `mhLib/Variant_cfg.h`
