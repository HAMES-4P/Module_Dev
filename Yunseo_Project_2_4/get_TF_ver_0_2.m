y2 = output_encoder(:, 2); 
u2 = input_step(:, 2);

%step response 측정 모델의 설정대로 15초 기준
y2(1:15000, 1) = y2(15000, 1);
y2(:, 1) = y2(:, 1) - y2(15000, 1);

t = (0:length(u2)-1) * 0.001;

Ts = 0.001; % 샘플링 주파수
data = iddata(y2, u2, Ts);

% tfest 함수를 사용하여 전달 함수 추정
order = 2; % 전달 함수의 차수
sys_estimate = tfest(data, order);

% 추정된 전달 함수의 Bode Plot 시각화
figure;
bode(sys_estimate);

% 결과 출력
disp('Estimated Transfer Function:');
disp(sys_estimate);

y_estimate = lsim(sys_estimate, u2, t);

[controller, info] = pidtune(sys_estimate, 'PID', 1)

% 폐쇄루프 시스템 생성
closedLoopSys = feedback(sys_estimate * controller, 1);

% 스텝 응답 얻기
t_plot = 0:0.01:5; % 시간 범위
[y_plot, t_plot] = step(closedLoopSys, t_plot);

% y2와 y_estimate를 함께 플로팅
figure;

plot(t, y2, '-b', 'LineWidth', 2);
hold on;
plot(t, y_estimate, '--r', 'LineWidth', 2);
hold off;

xlabel('Time');
ylabel('y');
title('Actual Output (y2) vs Estimated Output (y\_estimate)');

legend('Actual Output (y2)', 'Estimated Output (y\_estimate)');

% controller와 system의 step response 출력 => overshoot, rise time 보기 가능
figure;
plot(t_plot, y_plot);
title('Step Response of the Closed-loop System');
xlabel('Time');
ylabel('Output');