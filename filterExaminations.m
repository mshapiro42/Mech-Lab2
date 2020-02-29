if exist('s','var') %If the serial port is open
    fclose(s);      %Close the serial port
    delete(s);      %Delete the variable   
end
clear; clc; close all;
s = serial('COM12', 'BaudRate', 9600); %Establish Serial Port
s.ReadAsyncMode = 'continuous';
fopen(s); %Open Serial Port

% 4th order butterworth filter with c/o 150 Hz
[b, a] = butter(4, .4);

inputs = zeros(1,5);
outputs = zeros(1,5);
outputs2 = zeros(1,100);
figure;
plot(outputs2);
axis([0 100 -100 100]);
while (1)
    result = fread(s,1,'float');
    inputs = [result, inputs(1:4)];
    filteredValue = filterValue(inputs, outputs, b, a);
    fprintf('New Output: %.4f\n',filteredValue);
    outputs = [filteredValue, outputs(1:4)];
    outputs2 = [filteredValue, outputs2(1:end-1)];
    plot(outputs2);
    axis([0 100 -100 100]);
    pause(.001);
end


