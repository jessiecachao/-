% 读取音频文件
[audio, fs] = audioread('D:\dsp\test4.wav');
% 设置参数
windowSize =100;  % 窗口大小
overlap = 0.5;  % 重叠比例
% 定义窗函数
windowFunctions = {@boxcar, @triang, @hanning, @hamming, @blackman};
windowNames = {'boxcar', 'triang','hanning' 'hamming', 'blackman'};

% 绘制频谱图

    for j = 1:length(windowFunctions)
        % 应用窗函数
        window = windowFunctions{j}(windowSize);
        [H,w]=freqz(window,1,1024,'whole');
% 寻找频谱中的主要频率
        [~, idx] = max(abs(H));
        mainFreq = fs(idx);
    end
% 音符识别
noteFreqs = [220, 246.94, 261.63, 293.66, 329.63, 349.23, 392];
noteNames = {'la', 'ci', 'do', '2(rui)', '3(mi)', '4(fa)', '5(so)'};

noteIndex = zeros(size(mainFreq));
for i = 1:length(noteFreqs)
    [~, index] = min(abs(mainFreq - noteFreqs(i)));
    noteIndex(index) = i;
end

% 输出音符识别结果
disp('音符识别结果:');
for i = 1:length(noteIndex)
    if noteIndex(i) > 0
        disp(['音符 ', num2str(i), ' - ', noteNames{noteIndex(i)}]);
    end
end
