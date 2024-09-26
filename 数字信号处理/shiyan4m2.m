M = 60;
alpha=(M-1)/2;
k=0:M-1;
wk= (2*pi/M)*k;
Hrs = [ones(1,10),0.594,0.109,zeros(1,7),0.109,0.594,ones(1,19),0.594,0.109,zeros(1,7),0.109,0.594,ones(1,9)];
Hdr = [1,1,0,1,0,1]; 
wdl = [0,0.35,0.35,0.65,0.65,1];
k1 = 0:floor((M-1)/2);
k2 = floor((M-1)/2)+1:M-1;
angH = [-alpha*(2*pi)/M*k1, alpha*(2*pi)/M*(M-k2)];
H = Hrs.*exp(1i*angH);
h = real(ifft(H,M));
[Ha,w]=freqz(h,1);
subplot(2,1,1); stem(k,h); 
title('脉冲响应'); xlabel('n'); ylabel('h(n)');grid

subplot(2,1,2);plot(w/pi,20*log10(abs(Ha))); 
 grid
title('幅度响应'); xlabel('频率(单位:Pi)');ylabel('分贝');