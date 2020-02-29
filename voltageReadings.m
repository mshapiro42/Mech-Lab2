clear;clc;close all;

y = 0:15:345;
% x1 = zeros(1,length(y));
% x2 = zeros(1,length(y));
x = [406,420,449,489,551,623,703,741,764,784,794,812,830,849,877,920,115,187,246,304,334,356,372,388]; %402];
% x1 = [144,160,179,202.500000000000,226,239,256.500000000000,290,308,323.750000000000,337,348.500000000000,361,381.500000000000,402,426,10.5000000000000,40,49,69,90.5000000000000,107,119,132,144.500000000000];
% x2 = [144.500000000000,160,179,191.500000000000,212.500000000000,237,259,288,310,325.500000000000,337,349.500000000000,362,381.500000000000,399,426,5.50000000000000,41,48,69.5000000000000,91,107,120,132,145];
% x = [824, 840, 862, 895, 132, 214, 274, 316, 343, 362, 377, 393, 405,...
%     424, 452, 491, 556, 637, 699, 738, 761, 778, 791, 801, 817];
% x = [403, 417, 445, 468, 0, 0, 0, 747, 784, 800, 808, 816, 828, 841, 856,...
%     888, 120, 191, 264, 316, 346, 365, 381, 392, 403];
%y(x==0)=[];
%x(x==0) = [];
%y(1:13) = y(1:13) + 360*ones(1,13);
%y = [y(14:end),y(1:13)] - 360;
%x = [x(14:end),x(1:13)];
% y(1:4) = y(1:4) + 360;
% y = [y(5:end),y(1:4)]+120;
% x = [x(5:end),x(1:4)];
% for i=1:length(y)
%     s = string(y(i)) + ': ';
%     x1(i) = input(s);
% end
% for i=1:length(y)
%     s = string(y(i)) + ': ';
%     x2(i) = input(s);
% end

y(1:16) = y(1:16)+360;
y = y - 240;
y = [y(17:end), y(1:16)];
x = [x(17:end), x(1:16)];
plot(x, y,'r*');
%%
close all;
plot(x,y,'*');
hold on;
axis([0, 1024,0, 360]);
yticks(0:45:360);
%yticklabels({'0','1\pi/4','2\pi/4','3\pi/4','4\pi/4','5\pi/4','6\pi/4','7\pi/4','8\pi/4'})
p=polyfit(x,y,6);
plot(x,polyval(p, x));
legend('readings','6th order estimate','Location','NorthWest');
xlabel('8 bit Float ADC');
ylabel('Angular position (degrees)');
title('Angular position vs voltage readings');
% 
%%
while(1)
    i = input('Enter a reading: ');
    if (i == 2000) 
        break;
    end
    temp = polyval(p,i);
    fprintf('Your angle is %.2f degrees \n',temp);
    plot(i,temp,'k*');
end

%%
pos1 = input('Enter a position: ');
pos2 = input('Enter a position 2: ');
pos1 = abs(polyval(p, pos1));
pos2 = abs(polyval(p, pos2));
angDif = pos2 - pos1;
if angDif < 0
    angDif = angDif + 427;
end
angVel = angDif * .002381 * 1000; % rev/sec
fprintf('Velocity: %.3f rev/sec\n',angVel);