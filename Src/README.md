상전류 파형 개선을 위한 변경점 적용
- Speed PI Gain 변경
  P Gain : 700 -> 400
  I Gain : 600 -> 500
- Current PI Gain 변경
  P Gain : 2000 -> 300
  I Gain : 618 -> 500
- Mintime 값 ActSpeed에 가변되도록 로직 적용(EWP는 속도에 따른 부하 변화가 발생하여 속도에 연동)
  Actspeed 3000RPM 미만 : 100
  Actspeed 3000RPM 이상 : 160