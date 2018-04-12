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
dry_friction = 0.0013; %Nm/s^2

%% Run the Simulink simulation.
sim('HapticPaddle');

%% Plot the simulation results.
figure()
subplot(311)
plot(t(1:300),phi(1:300))
title('Angle(degrees)')
subplot(312)
plot(t,omega)
title('Angular velocity(degrees/s)')
subplot(313)
plot(t,phi)
title('Angular acceleration (degrees/s^2)')