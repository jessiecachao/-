[x, Fs] = audioread('D:\dsp\test1.wav');
% 读取音频文件


% 计算音频长度和时间
len = length(x);
duration = len/Fs;
time = linspace(0, duration, len);

% 进行快速傅里叶变换（FFT）
N = 2^nextpow2(len); % 使用2的幂作为FFT长度，以提高计算效率
w1 = boxcar(len);
 X1 = fft(x.*w1, N);
 w2 = triang(len);
 X2 = fft(x.*w2, N);
 w3 = hanning(len);
 X3 = fft(x.*w3, N);
w4 = hamming(len);
 X4 = fft(x.*w4, N);
 w5 = blackman(len);
 X5 = fft(x.*w5, N);
%X = fft(x, N); % X是频域表示

% 计算频率轴
f = Fs*(0:(N/2))/N; % 单边频谱

% 绘制频谱图
subplot(3,2,1);
plot(f, 2*abs(X1(1:N/2+1))/N);
title('频谱分析');
xlabel('频率 (Hz)');
ylabel('振幅');
xlim([0, Fs/2]);

subplot(3,2,2);
plot(f, 2*abs(X2(1:N/2+1))/N);
title('频谱分析');
xlabel('频率 (Hz)');
ylabel('振幅');
xlim([0, Fs/2]);

subplot(3,2,3);
plot(f, 2*abs(X3(1:N/2+1))/N);
title('频谱分析');
xlabel('频率 (Hz)');
ylabel('振幅');
xlim([0, Fs/2]);

subplot(3,2,4);
plot(f, 2*abs(X4(1:N/2+1))/N);
title('频谱分析');
xlabel('频率 (Hz)');
ylabel('振幅');
xlim([0, Fs/2]);

subplot(3,2,5);
plot(f, 2*abs(X5(1:N/2+1))/N);
title('频谱分析');
xlabel('频率 (Hz)');
ylabel('振幅');
% 设置坐标轴范围
xlim([0, Fs/2]);

% 可选：设置谱分析参数
% 例如，使用窗函数对信号进行加窗，减小频谱泄漏
% window = hamming(len);
% X = fft(x.*window, N);

% 可选：对谱进行平滑处理，提高可读性
% 例如，使用移动平均滤波器
% smooth_factor = 10; % 平滑因子
% window_size = ceil(length(X)/smooth_factor);
% b = (1/window_size)*ones(1, window_size);
% a = 1;
% X_smoothed = filter(b, a, abs(X));

% 可选：绘制平滑后的频谱图
% figure;
% plot(f, 2*X_smoothed(1:N/2+1)/N);
% title('平滑后的频谱分析');
% xlabel('频率 (Hz)');
% ylabel('振幅');
% xlim([0, Fs/2]);
