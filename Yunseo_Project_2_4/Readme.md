  요구사항 분석 중, 바닥에 따라 달라진 gain값이 필요한 상황이 개발 중에 발생할 것으로 보여졌다. 
  시스템 테스팅에 쓰일 바닥이 최소 4가지였으므로 각각의 상황에 원하는 rise time과 overshoot에 맞는 gain값을 trial and error로 구하는 방식은 개발 시간 측면에서 비효율적이라고 판단하였다.
  따라서 전달함수를 알아내고 그에 맞는 gain값을 구하는 방식이 가능하다면 더욱 효율적일 것으로 보고, 개발 프로세스를 향상하기 위한 SW를 개발하였다.
  본 개발용 SW는 모터 시스템의 물리적 구조에 구애받지 않고, 모터 시스템에 입력을 준 후 엔코더로 받은 출력 값을 측정하고, 입력값과 출력값을 통해 시스템의 전달함수를 추정하고 검증한다.
  또한 추정한 전달함수를 통해 PID 시스템의 적절한 gain값을 정하고 설정된 gain값이 반영된 controller와 전달함수를 통해 step response를 시뮬레이션을 통해 알아내고 그 시스템의 rise time과 overshoot를 그래프를 통해 볼 수 있다.

![image](https://github.com/HAMES-4P/Module_Dev/assets/94425657/28decba5-644a-4f87-a788-2f40b3c18a75)
 
  Matlab 상으로 개발한 모델을 쓰기 위해 ArduinoMega2560 보드를 사용하였다.
  우리가 요구사항에 맞게 개발할 시스템 또한 Arduino moter shield를 사용하기에 규격상으로도 맞는 개발 환경이다.
  step input을 모터의 PWM 핀에 인가하고 모터의 엔코더에 측정되는 출력값을 scope를 통해 Matlab workspace로 저장한다. 
 
 (2번 이미지)
 
 입력값과 출력값을 Matlab 전달함수 추정함수를 통해 구하고 추정된 전달함수의 Bode plot을 통해 일반적인 모터의 전달함수 형태와 맞는지 확인한다.

 (3번 이미지) 

 추정된 전달함수에 측정한 입력값을 넣어서 추정 전달함수의 출력값을 구하고 실제 측정된 출력값과 비교하여 검증한다.

 (4번 이미지) 

 PID의 gain을 tuning 할 수 있도록 하는 매트랩 함수를 통해 각 gain들을 얻어내고,

 (5번 이미지) 

 위와 같이 gain들로 이루어진 controller에 의한 step response를 구하고 그 controller가 원하는 rise time과 overshoot를 만족하는지 확인한다. 만일 만족하지 않는다면 다시 tuning을 한다.
