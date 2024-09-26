
t=linspace(0,10,100);
x=zeros(size(t));
n=linspace(0,10,100);
x(t<=5)=2;
b=0.02;
a=[1,-0.9802];
[H,w]=freqz(b,a);
y=filter(b,a,x);
subplot(3,1,1);
stem(x);
xlabel('n');ylabel('x(n)');
title('输入x(n)的图像');

subplot(3,1,2);
plot(w,abs(H));grid on;
xlabel('w(rad）');ylabel('|H(e^jw)|');
title('数字低通滤波器频率响应');

subplot(3,1,3);
stem(y);
xlabel('n');ylabel('y(n)');
title('输出y(n)的图像');