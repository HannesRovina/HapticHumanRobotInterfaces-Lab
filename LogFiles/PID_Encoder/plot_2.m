%% Import data from text file.
% Script for importing data from the following text file:
%
%    /home/manu/Dropbox/EPFL Spring 2018/Haptic HRI/HapticHumanRobotInterfaces-Lab/LogFiles/PID_Hall/p05i0d0003.csv
%
% To extend the code to different selected data or a different text file,
% generate a function instead of a script.

% Auto-generated by MATLAB on 2018/04/14 17:48:42

%% Initialize variables.
filename = 'C:\Users\Hannes\Desktop\Master\HapticHumanRobotInterfaces\LogFiles\PID_Encoder\p05i0d0003.csv';
delimiter = ';';
startRow = 2;

%% Format for each line of text:
%   column1: double (%f)
%	column2: double (%f)
%   column3: double (%f)
%	column4: double (%f)
%   column5: double (%f)
%	column6: double (%f)
%   column7: double (%f)
%	column8: double (%f)
%   column9: double (%f)
%	column10: double (%f)
%   column11: double (%f)
%	column12: double (%f)
%   column13: double (%f)
%	column14: double (%f)
%   column15: double (%f)
%	column16: double (%f)
%   column17: double (%f)
%	column18: double (%f)
%   column19: double (%f)
%	column20: double (%f)
% For more information, see the TEXTSCAN documentation.
formatSpec = '%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%[^\n\r]';

%% Open the text file.
fileID = fopen(filename,'r');

%% Read columns of data according to the format.
% This call is based on the structure of the file used to generate this
% code. If an error occurs for a different file, try regenerating the code
% from the Import Tool.
dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'TextType', 'string', 'EmptyValue', NaN, 'HeaderLines' ,startRow-1, 'ReturnOnError', false, 'EndOfLine', '\r\n');

%% Close the text file.
fclose(fileID);

%% Post processing for unimportable data.
% No unimportable data rules were applied during the import, so no post
% processing code is included. To generate code which works for
% unimportable data, select unimportable cells in a file and regenerate the
% script.

%% Create output variable
p05i0d0003 = table(dataArray{1:end-1}, 'VariableNames', {'timestamps','actual_currentA','target_currentA','timestepus','motor_torqueNm','encoder_paddle_posdeg','paddle_pos_filtdeg','hall_voltageV','enablePID','Target_Angledeg','Kp','Ki','Kd','enableEncoder_Angle','rise_times','overshoot_angledeg','led_00010','led_10010','led_20010','led_30010'});

%% Clear temporary variables
clearvars filename delimiter startRow formatSpec fileID dataArray ans;

%% Generate plots

time = p05i0d0003.timestamps(6721:end)-867.23;
theta = p05i0d0003.paddle_pos_filtdeg(6721:end);

figure()
plt = plot(time,theta,'LineWidth',2)
hold on
plot(time, ones(size(time))*10,'LineWidth',1.5)
plot(time, ones(size(time))*max(theta),'-.','LineWidth',1.5)
plot(time, ones(size(time))*9.8,'-.','LineWidth',1.25)
plot(time, ones(size(time))*10.2, '-.','LineWidth',1.25)
legend('PID simulation','Target angle','Maximum Overshoot 4.69�','Upper error 10.2�','Lower error 9.8�','Location','southeast')
xlabel('Time [s]')
ylabel('Angle [deg]')
xlim([-0.5 3])
ylim([-0.5 15])
saveas(plt,'Simulink_PID_encoder2.png')

t_init = find(theta>theta(1),1);
rise_time = time(find(theta>10,1))-time(t_init);
disp(rise_time*1000)

t_settle = find(abs(flipud(theta)-10)>0.2,1);
settling_time = time(length(time)-t_settle)-time(t_init);
disp(settling_time*1000)