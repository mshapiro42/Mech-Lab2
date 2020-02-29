if exist('s','var') %If the serial port is open
    fclose(s);      %Close the serial port
    delete(s);      %Delete the variable   
end
clear; clc;
s = serial('COM12', 'BaudRate', 9600); %Establish Serial Port
s.ReadAsyncMode = 'continuous';
fopen(s); %Open Serial Port

% 4th order butterworth filter with c/o 150 Hz
[b, a] = butter(4, .4);

inputs = 0*ones(1,5);
outputs = 0*ones(1,5);

while (1)
    result = fread(s,1,'float');
    outputs = filterValue(results, inputs, outputs, b, a);
    fprintf('New Output: %.4f\n',outputs(1));
end


