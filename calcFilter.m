clear; clc;
sample = 1000; %Hz
nyquist = sample / 2; %Hz
cutoff = 150; %Hz
Wn = cutoff / nyquist;
[b, a] = butter(4, Wn);
fprintf('Cutoff Frequency, %.0f Hz, is %.2f of nyquist, %.0f Hz.\n',cutoff, Wn,nyquist);
fprintf('Filter is 4th order\n');
fprintf('B coefficients:\n');
fprintf('%.9f ',b);
fprintf('\nA coefficients:\n');
fprintf('%.9f ',a);
fprintf('\n');
freqs(b, a);