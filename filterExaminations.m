% if exist('s','var') %If the serial port is open
%     fclose(s);      %Close the serial port
%     delete(s);      %Delete the variable
% end
% clear; clc; %close all;
% s = serial('COM12', 'BaudRate', 9600); %Establish Serial Port
% s.ReadAsyncMode = 'continuous';
% fopen(s); %Open Serial Port

cla reset;
x = [ones(1,50), zeros(1,50)];
order = 4;
% 4th order butterworth filter with c/o 150 Hz
[b, a] = butter(order, .3);

inputs = zeros(1,order+1);
inputs2 = zeros(1,200);
outputs = zeros(1,order+1);
outputs2 = zeros(1,200);
outputs3 = zeros(1,200);
%figure;
%hplotdata = plot(1:200,outputs2,'r',1:200,outputs3,'b',1:200,inputs2,'g');
hplotdata = plot(1:200,outputs2,'b',1:200,inputs2,'r');
hold on;
axis([0 200 -.5 105]);
%legend('myFunc','built-in filter','raw input');
legend('myFilter','raw input');
i = 1;
while (1)
    result = 50*x(i) + 25 + 4*rand(1) - 2;
    %result = fread(s,1,'float');
%     while (~(result>0 && result<1024))
%         result = fread(s,1,'float');
%     end
    disp(result);
    inputs = [result, inputs(1:end-1)];
    inputs2 = [result, inputs2(1:end-1)];
    filteredValue = filterValue(inputs, outputs, b, a);
    %fprintf('New Output: %.4f\n',filteredValue);
    
    outputs = [filteredValue, outputs(1:end-1)];
    outputs2 = [filteredValue, outputs2(1:end-1)];
    outputs3 = filter(b,a,outputs2);
    delete(hplotdata);
    %hplotdata = plot(1:200,outputs2,'r',1:200,outputs3,'b',1:200,inputs2,'g');
    hplotdata = plot(1:200,outputs2,'b',1:200,inputs2,'r');
    pause(.01);
    i = mod(i, 100) + 1;
end


