%% Clean the workspace.
clc 
clear variables
close all

%% Set the simulation parameters.

% Motor parameters (maxon 339156), obtained from its datasheet.
%Lm      = 0.00125; % Phase-to-phase inductance [H]. Since the board is directly performing current/torque control, this can be ignored.
%Rm      = 10.6; % Phase-to-phase resistance [ohm]. Since the board is directly performing current/torque control, this can be ignored.
% k_tau	= 0.0538; % Torque constant [Nm/A]. Since the board is directly performing current/torque control, this can be ignored.
B_m     = 7.90E-07; % Viscous friction of the ball bearings [Nm/(rad/s)].
J_rotor = 1.39e-6; % Rotor inertia [kg.m^2].

% Paddle mechanical parts parameters, obtained from the CAD model.
r_p1	= 0.075; % Radius of the bottom arc of the paddle [m].
r_m     = 0.005; % Motor worm screw radius [m].
J_p     = 2.20E-04; % Paddle inertia [kg*m^2].
J_m     = 0.19E-06; % Worm screw inertia [kg*m^2].
B_p     = 0; % Paddle ball bearing viscous friction [Nm*s/rad]. Will be ignored for this simulation.
m_p     = 0.075; % Paddle mass [kg].
l       = 0.0199; % Distance between the center of rotation and the center of mass of the paddle [m].
r_p2    = 0.065; % Length of the paddle lever (to apply a force with the finger) [m].

% Environment parameters.
g       = 9.81; % Gravity [N/kg].

% Computed constants.
R = r_p1 / r_m; % Reduction ratio of the cable transmission [].
J_eq  = J_p + (J_rotor+J_m) * R^2; % Equivalent total inertia, computed on the paddle side [kg*m^2].
B_eq  = B_p + B_m * R^2; % Equivalent viscous friction, computed on the paddle side [Nm*s/rad].
dry_friction = 0.001; %Nm/s^2

%% Sinusoidal Torque
sim('HapticPaddleSin');

%% Plot the simulation results.
figure()
plt = plot(t(1:3000),phi(1:3000),'LineWidth',2)
hold on
xlabel('Time [s]')
ylabel('Angle [deg]')
title('Angular displacement(degrees)')
saveas(plt,'Simulink_Position.png')
figure
plt = plot(t(1:3000),omega(1:3000),'LineWidth',2)
xlabel('Time [s]')
ylabel('Angular velocity [deg/s]')
title('Angular velocity(degrees/s)')
saveas(plt,'Simulink_Velocity.png')
figure
plt = plot(t(1:3000),omega_dot(1:3000),'LineWidth',2)
xlabel('Time [s]')
ylabel('Angular acceleration [deg/s^2]')
title('Angular acceleration (degrees/s^2)')
saveas(plt,'Simulink_Acceleration.png')

%% Run the Simulink simulation.
sim('HapticPaddle');

%% Plot the simulation results.
figure()
plt = plot(t(1:100),phi(1:100),'LineWidth',1.5)
hold on
plot(t(1:100), ones(1,100)*10,'LineWidth',1.5)
plot(t(1:100), ones(1,100)*max(phi),'-.','LineWidth',1.5)
plot(t(1:100), ones(1,100)*9.85,'-.','LineWidth',1.25)
plot(t(1:100), ones(1,100)*10.15, '-.','LineWidth',1.25)
legend('PID simulation','Target angle','Maximum overshoot 1.3°','Upper error 10.15°','Lower error 9.85°','Location','southeast')
xlabel('Time [s]')
ylabel('Angle [deg]')
saveas(plt,'Simulink_PID.png')
