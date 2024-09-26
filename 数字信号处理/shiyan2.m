% 读取音频文件
[audio1, fs1] = audioread('D:\dsp\test1.wav');
[audio2, fs2] = audioread('D:\dsp\test2.wav');
[audio3, fs3] = audioread('D:\dsp\test3.wav');
[audio4, fs4] = audioread('D:\dsp\test4.wav');

% 绘制音频信号的时域图
t1 = (0:length(audio1)-1) / fs1;
t2 = (0:length(audio2)-1) / fs2;
t3 = (0:length(audio3)-1) / fs3;
t4 = (0:length(audio4)-1) / fs4;

subplot(2,2,1);
plot(t1, audio1);
title('test1.wav - 时域图');
xlabel('时间 (秒)');
ylabel('幅度');

subplot(2,2, 2);
plot(t2, audio2);
title('test2.wav - 时域图');
xlabel('时间 (秒)');
ylabel('幅度');

subplot(2,2, 3);
plot(t3, audio3);
title('test3.wav - 时域图');
xlabel('时间 (秒)');
ylabel('幅度');

subplot(2,2, 4);
plot(t4, audio4);
title('test4.wav - 时域图');
xlabel('时间 (秒)');
ylabel('幅度');