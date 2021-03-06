#include "haptic_controller.h"
#include "communication.h"
#include "drivers/adc.h"
#include "drivers/incr_encoder.h"
#include "drivers/hall.h"
#include "drivers/callback_timers.h"
#include "lib/utils.h"
#include "torque_regulator.h"


#define DEFAULT_HAPTIC_CONTROLLER_PERIOD 350 // Default control loop period [us].

volatile uint32_t hapt_timestamp; // Time base of the controller, also used to timestamp the samples sent by streaming [us].
volatile float32_t hapt_hallVoltage; // Hall sensor output voltage [V].
volatile float32_t hapt_encoderPaddleAngle; // Paddle angle measured by the incremental encoder [deg].
volatile float32_t hapt_paddleAngle; // Paddle angle measured by the calibrated Hall sensor [deg].
volatile float32_t hapt_paddleSpeed; // Paddle speed measured by the calibrated Hall sensor [deg/s].
volatile float32_t hapt_paddleAccel; // Paddle acceleration measured by the calibrated Hall sensor [deg/s^2].
volatile float32_t hapt_paddlePrevPos; // Previous paddle angle [deg].
volatile float32_t hapt_paddlePosFilt; // Filtered paddle position [deg].
volatile float32_t hapt_paddlePrevSpeed; // Previous paddle speed [deg/s].
volatile float32_t hapt_paddleSpeedFilt; // Filtered paddle speed [deg/s].
volatile float32_t hapt_paddleAccelFilt; // Filtered paddle acceleration [deg/s^s].
volatile float32_t hapt_paddlePrevAccel; // Previous paddle acceleration [deg/s^s].
volatile float32_t hapt_motorTorque; // Motor torque [N.m].
volatile float32_t wall_Stiffness; // wall stiffness [N.m/deg].
volatile float32_t wall_Damping; // wall damping [N.m/(deg/s)].

volatile float32_t filterCutoffFreqPosition = 250.0f; // Cutoff frequency for the speed/acceleration filter [Hz].
volatile float32_t filterCutoffFreqSpeed = 25.0f; // Cutoff frequency for the speed/acceleration filter [Hz].
const float32_t filterCutoffFreqAccel = 5.0f; // Cutoff frequency for the speed/acceleration filter [Hz].

volatile bool enable_PID;
volatile bool enable_EncoderAngle; //Change between hall or encoder angle
volatile bool enable_DryFrictionCompensation; //Add dry friction compensation
volatile bool enable_ViscousFrictionCompensation; //Add viscous friction compensation
volatile bool enable_GravityCompensation; //Add gravity compensation
volatile float32_t Target_Angle; // Target andle for PID control [deg].
volatile float32_t error_accumulator; //integral of the error for the I in the PID controller
volatile float32_t Kp; //Parameter for PID
volatile float32_t Kd; //Parameter for PID
volatile float32_t Ki; //Parameter for PID
volatile float32_t overshoot_Angle; //Record overshoot angle
volatile float32_t rise_time; // Record rise time
volatile uint32_t first_trigger;
void hapt_Update(void);
float32_t hapt_LowPassFilter(float32_t previousFilteredValue, float32_t input,float32_t dt, float32_t tau);
float32_t PID_Regulator1(float32_t error, float32_t previous_error, float32_t dt);
float32_t Compensation(float32_t position, float32_t velocity);
float32_t Wall(float32_t position, float32_t velocity);

volatile float32_t strain_measure; // Strain gauge measurement
volatile float32_t strain_measureFilt; // Filtered strain measurement [V].
volatile float32_t measured_torque;
volatile float32_t target_velocity;
volatile float32_t mass;
volatile float32_t inertia;
volatile float32_t MKB_option;
volatile float32_t target_acceleration;
volatile float32_t prev_target_velocity;
volatile float32_t prev_Target_Angle;
volatile bool calibrate;
volatile bool enable_MKB;
volatile float32_t torque_offset;
/**
* @brief Initializes the haptic controller.
*/
void hapt_Init(void)
{
	//Initializing variables
	enable_PID = false;
	enable_EncoderAngle = false;
	enable_DryFrictionCompensation = false;
	enable_ViscousFrictionCompensation = false;
	enable_GravityCompensation = false;

	Kp = 0.1f;
	Ki = 0.0f;
	Kd = 0.0001f;
	hapt_timestamp = 0;
	hapt_motorTorque = 0.0f;
	hapt_paddlePrevPos = 0.0f;
	hapt_paddlePrevSpeed = 0.0f;
	hapt_paddlePosFilt = 0.0f;
	hapt_paddleSpeedFilt = 0.0f;
	hapt_paddleAccelFilt = 0.0f;
	Target_Angle = 0.0f;
	error_accumulator = 0.0f;
	first_trigger = 0;
	rise_time = 0.0f;
	overshoot_Angle = 0.0f;
	wall_Stiffness = 0.01f;
	wall_Damping = 0.01f;
	strain_measure = 0.0f;
	strain_measureFilt= 0.0f;
	measured_torque = 0.0f;
	target_velocity = 0.0f;
	MKB_option = 0;
	mass = 1;
	target_acceleration = 0.0f;
	prev_target_velocity = 0.0f;
	prev_Target_Angle = 0.0f;
	calibrate = 0;
	enable_MKB = 0;
	torque_offset = 0.0f;
	// Make the timers call the update function periodically.
	cbt_SetHapticControllerTimer(hapt_Update, DEFAULT_HAPTIC_CONTROLLER_PERIOD);

	// Share some variables with the computer.
	//comm_monitorUint32Func("timestep [us]", cbt_GetHapticControllerPeriod, cbt_SetHapticControllerPeriod);
	comm_monitorFloat("motor_torque [N.m]", (float32_t*)&hapt_motorTorque, READWRITE);
	//comm_monitorFloat("encoder_paddle_pos [deg]", (float32_t*)&hapt_encoderPaddleAngle, READONLY);
	//comm_monitorFloat("paddle_pos [deg]", (float32_t*)&hapt_paddleAngle, READONLY);
	//comm_monitorFloat("paddle_speed [deg/s]", (float32_t*)&hapt_paddleSpeed, READONLY);
	//comm_monitorFloat("paddle_accel [deg/s^2]", (float32_t*)&hapt_paddleAccel, READONLY);
	comm_monitorFloat("paddle_pos_filt [deg]", (float32_t*)&hapt_paddlePosFilt, READONLY);
	comm_monitorFloat("target_angle [deg]", (float32_t*)&Target_Angle, READONLY);
	//comm_monitorFloat("paddle_speed_filt [deg/s]", (float32_t*)&hapt_paddleSpeedFilt, READONLY);
	//comm_monitorFloat("target_speed [deg/s]", (float32_t*)&target_velocity, READONLY);
	//comm_monitorFloat("paddle_accel_filt [deg/s^2]", (float32_t*)&hapt_paddleAccelFilt, READONLY);
	//comm_monitorFloat("target_acceleration [deg/s^2]", (float32_t*)&target_acceleration, READONLY);
	//comm_monitorFloat("Cutoff frequency [Hz]", (float32_t*)&filterCutoffFreqSpeed, READWRITE);
	comm_monitorFloat("Stiffness [N.m/deg]", (float32_t*)&wall_Stiffness, READWRITE);
	comm_monitorFloat("Damping [N.m/(deg/s)]", (float32_t*)&wall_Damping, READWRITE);
	comm_monitorFloat("Mass [kg]", (float32_t*)&mass, READWRITE);
	//comm_monitorFloat("paddle_accel_filt [deg/s^2]", (float32_t*)&hapt_paddleAccelFilt, READONLY);
	//comm_monitorFloat("hall_voltage [V]", (float32_t*)&hapt_hallVoltage, READONLY);
	comm_monitorBool("enable PID", (bool*)&enable_PID, WRITEONLY);
	comm_monitorFloat("enable acc/pos/vel/kb admittance control", (float32_t*)&MKB_option, WRITEONLY);
	comm_monitorBool("enable_MKB", (bool*)&enable_MKB, WRITEONLY);
	//comm_monitorFloat("Target_Angle [deg]", (float32_t*)&Target_Angle, READWRITE);
	//comm_monitorFloat("Kp ", (float32_t*)&Kp, READWRITE);
	//comm_monitorFloat("Ki ", (float32_t*)&Ki, READWRITE);
	//comm_monitorFloat("Kd ", (float32_t*)&Kd, READWRITE);
	comm_monitorBool("calibrate", (bool*)&calibrate, WRITEONLY);

	//comm_monitorFloat("rise_time [s]", (float32_t*)&rise_time, READONLY);
	//comm_monitorFloat("overshoot_angle [deg]", (float32_t*)&overshoot_Angle, READONLY);
	//comm_monitorBool("enable Dry Friction Compensation", (bool*)&enable_DryFrictionCompensation, WRITEONLY);
	//comm_monitorBool("enable Viscous Friction Compensation", (bool*)&enable_ViscousFrictionCompensation, WRITEONLY);
	//comm_monitorBool("enable Gravity Compensation", (bool*)&enable_GravityCompensation, WRITEONLY);
	comm_monitorFloat("strain_measurement [V]", (float32_t*)&strain_measure, READONLY);
	comm_monitorFloat("Torque measurement [Nm]", (float32_t*)&measured_torque, READONLY);


}
/**
* @brief Updates the haptic controller state.
*/
void hapt_Update()
{

	float32_t motorShaftAngle; // [deg].
	// Compute the dt (uncomment if you need it).
	float32_t dt = ((float32_t)cbt_GetHapticControllerPeriod()) / 1000000.0f; // [s].

	// Increment the timestamp.
	hapt_timestamp += cbt_GetHapticControllerPeriod();

	strain_measure = adc_GetChannelVoltage(ADC_CHANNEL_6);
	strain_measureFilt = hapt_LowPassFilter(strain_measureFilt,strain_measure,dt,filterCutoffFreqAccel);


	measured_torque = (strain_measureFilt*17.84-29.5)*0.065;
	if(calibrate){
		torque_offset = measured_torque;
		calibrate = 0;
		prev_target_velocity = 0.0f;
		prev_Target_Angle = 0.0f;
		Target_Angle = 0.0f;
		target_velocity = 0.0f;
	}

	measured_torque -= torque_offset;
	inertia = mass*0.075*0.075;
	//Target_Angle = measured_torque/wall_Stiffness;
	//target_velocity = measured_torque/wall_Damping;
	//target_acceleration = measured_torque/inertia;
	// Get the Hall sensor voltage.
	hapt_hallVoltage = hall_GetVoltage();

	// Get the encoder position.
	motorShaftAngle = enc_GetPosition();
	hapt_encoderPaddleAngle = motorShaftAngle / REDUCTION_RATIO;

	// Compute the paddle angle from the Hall sensor voltage or the encoder which is then used for the PID.
	/*
	if(!enable_EncoderAngle)
	{
		hapt_paddleAngle = hapt_hallVoltage * (33.0f)-81.9f;
	}
	else*/ hapt_paddleAngle = hapt_encoderPaddleAngle;

	//Filter the position value
	hapt_paddlePosFilt = hapt_LowPassFilter(hapt_paddlePosFilt,hapt_paddleAngle, dt,filterCutoffFreqPosition);

	// Compute the speed and accleration from the paddle angle.
	hapt_paddleSpeed = (hapt_paddlePosFilt - hapt_paddlePrevPos) / dt;
	hapt_paddleSpeedFilt = hapt_LowPassFilter(hapt_paddleSpeedFilt,hapt_paddleSpeed, dt,filterCutoffFreqSpeed);
	hapt_paddleAccel = (hapt_paddleSpeedFilt - hapt_paddlePrevSpeed) / dt;
	hapt_paddleAccelFilt = hapt_LowPassFilter(hapt_paddleAccelFilt, hapt_paddleAccel, dt, filterCutoffFreqAccel);

	// Compute the motor torque, and apply it.
	hapt_motorTorque = 0.0f;

	//Sinusoidal motor torque
	//hapt_motorTorque = 0.0011*sin(2*3.14*hapt_timestamp*1e-6);

	//Reinitialize error_accumulator for I control
	if(!enable_PID)
	{
		error_accumulator = 0;
		first_trigger = 0;
	}

	//Calculate motor torque with PID
	if(enable_PID)
	{
		//hapt_motorTorque = 0.0011*sin(2*3.14*hapt_timestamp*1e-6);
		if(MKB_option==3)
		{
			target_velocity = measured_torque/wall_Damping;
			Kp = 0.005;
			Kd = 0.0;
			Ki = 0.0;
			hapt_motorTorque = PID_Regulator1(target_velocity-hapt_paddleSpeedFilt,target_velocity-hapt_paddlePrevSpeed,dt)/REDUCTION_RATIO;
		}
		else if(MKB_option == 2){
			Target_Angle = measured_torque/wall_Stiffness;
			Kp = 0.1;
			Kd = 0.0001;
			//Ki = 0.0;
			hapt_motorTorque = PID_Regulator1(Target_Angle-hapt_paddlePosFilt,prev_Target_Angle-hapt_paddlePrevPos,dt)/REDUCTION_RATIO;
		}
		else if(MKB_option == 1){
			target_acceleration = measured_torque/inertia;
			//Kp = 0.0001;
			Ki = 0.0;
			Kd = 0.0;
			hapt_motorTorque = PID_Regulator1(target_acceleration-hapt_paddleAccelFilt,target_acceleration-hapt_paddlePrevAccel,dt)/REDUCTION_RATIO;
		}
		else if(MKB_option == 4){
				Kp = 0.1;
				Ki = 0.0;
				Kd = 0.0001;
				if(enable_MKB)
				{
				target_velocity = prev_target_velocity + (measured_torque-wall_Damping*prev_target_velocity-wall_Stiffness*prev_Target_Angle)*dt/inertia;
				Target_Angle = prev_Target_Angle + target_velocity*dt;
				}
				else Target_Angle = prev_Target_Angle + (measured_torque-wall_Stiffness*prev_Target_Angle)*dt/wall_Damping;
				if(Target_Angle > 45)
				{
					Target_Angle = 45;
					prev_target_velocity = 0.0f;
				}
				if(Target_Angle <-45)
				{
					Target_Angle = -45;
					prev_target_velocity = 0.0f;
				}

				hapt_motorTorque = PID_Regulator1(Target_Angle-hapt_paddlePosFilt,prev_Target_Angle-hapt_paddlePrevPos,dt)/REDUCTION_RATIO;

		}
		else if(MKB_option == 5){
						Kp = 0.05;
						Ki = 0.0;
						Kd = 0.0008;
						target_velocity = (measured_torque-wall_Stiffness*prev_Target_Angle)/wall_Damping;
						Target_Angle = prev_Target_Angle + target_velocity*dt;
						//Target_Angle = prev_Target_Angle + (measured_torque-wall_Stiffness*prev_Target_Angle)*dt/wall_Damping;
						hapt_motorTorque = PID_Regulator1(Target_Angle-hapt_paddlePosFilt,Target_Angle-hapt_paddlePrevPos,dt)/REDUCTION_RATIO;

						Kp = 0.005;
						Kd = 0.0;
						Ki = 0.0;
						hapt_motorTorque += PID_Regulator1(target_velocity-hapt_paddleSpeedFilt,target_velocity-hapt_paddlePrevSpeed,dt)/REDUCTION_RATIO;

						//prev_target_velocity = target_velocity;
						prev_Target_Angle = Target_Angle;
				}
		else if(MKB_option == 6){
								Kp = 0.05;
								Ki = 0.0;
								Kd = 0.0008;
								target_velocity = measured_torque/(2*wall_Damping);
								Target_Angle =  measured_torque/(2*wall_Stiffness);
								//Target_Angle = prev_Target_Angle + (measured_torque-wall_Stiffness*prev_Target_Angle)*dt/wall_Damping;
								hapt_motorTorque = PID_Regulator1(Target_Angle-hapt_paddlePosFilt,Target_Angle-hapt_paddlePrevPos,dt)/REDUCTION_RATIO;

								Kp = 0.005;
								Kd = 0.0;
								Ki = 0.0;
								hapt_motorTorque += PID_Regulator1(target_velocity-hapt_paddleSpeedFilt,target_velocity-hapt_paddlePrevSpeed,dt)/REDUCTION_RATIO;

								//prev_target_velocity = target_velocity;
								//prev_Target_Angle = Target_Angle;
						}
		if(first_trigger==0)
		{
			first_trigger = 1;
			rise_time = 0;
		}
		if (first_trigger == 1)
		{
			rise_time += dt;
			if(hapt_paddlePosFilt>Target_Angle)
			{
				first_trigger = 2;
			}
		}
		if (hapt_paddlePosFilt>overshoot_Angle)
		{
			overshoot_Angle = hapt_paddlePosFilt;
		}
	}


	//Compensation of dry-, viscous friction and gravity
	//hapt_motorTorque += Compensation(hapt_paddlePosFilt, hapt_paddleSpeedFilt);

	//Adding a virtual wall
	//hapt_motorTorque += Wall(hapt_paddlePosFilt, hapt_paddleSpeedFilt);


	torq_SetTorque(hapt_motorTorque);

	//Store the previous position and speed for derivative purposes
	hapt_paddlePrevPos = hapt_paddlePosFilt;
	hapt_paddlePrevSpeed = hapt_paddleSpeedFilt;
	hapt_paddlePrevAccel = hapt_paddleAccelFilt;
	prev_target_velocity = target_velocity;
	prev_Target_Angle = Target_Angle;
}


/**
* @brief Filters a signal with an exponential smoothing method.
* @param previousFilteredValue the previous filtered value.
* @param input the filter input (the current sample of the signal to filter).
* @param dt the time elapsed since the last call of this function [s].
* @param cutoffFreq the frequency period of the filter [s].
* @return the new output of the filter.
* @remark Formula from: https://en.wikipedia.org/wiki/Exponential_smoothing
*/
float32_t hapt_LowPassFilter(float32_t previousFilteredValue, float32_t input, float32_t dt, float32_t cutoffFreq)
{
	float32_t tau = 1.0f / (2.0f * PI * cutoffFreq); // Rise time (time to reach 63% of the steady-state value).
	float32_t alpha = dt / tau; // Smoothing factor.
	return alpha * input + (1.0f - alpha) * previousFilteredValue;
}


/**
* @brief Calculates the PID output for a given error.
* @param error the current error value.
* @param previous_error the previous error value
* @param dt the time elapsed since the last call of this function [s].
* @return the calculated motor torque.
*/
float32_t PID_Regulator1(float32_t error, float32_t previous_error, float32_t dt)
{

	error_accumulator += error*dt;
	return (error*Kp + Kd*(error-previous_error)/dt+Ki*error_accumulator);

}

float32_t Compensation(float32_t position, float32_t velocity)
{
	float32_t MD = 0.075;
	float32_t rg = 0.01988;
	float32_t g = 9.81;
	float32_t dryfriction = 0.0014;
	float32_t viscousfriction = 8.33e-7;
	float32_t CompensationTorque = 0.0;
	int sign = 1;
	if(enable_DryFrictionCompensation){
		if(velocity < 0)
		{
			sign = -1;
			dryfriction = 0.0012;
		}
		if(velocity>10 || velocity < -10)
		{
			CompensationTorque += sign*dryfriction;
		}
		else
		{
			CompensationTorque += velocity/10*dryfriction;
		}
	}
	if(enable_ViscousFrictionCompensation)
		CompensationTorque += viscousfriction*velocity;
	if(enable_GravityCompensation)
		CompensationTorque += MD*g*rg*sin(3.14/180*position)/REDUCTION_RATIO;
	return CompensationTorque;
}

float32_t Wall(float32_t position, float32_t velocity)
{
	float32_t wall_Torque = 0.0f;
	if(position >= 15)
	{
		wall_Torque -= wall_Stiffness*(position-15);
		wall_Torque += -velocity*wall_Damping;
	}
	else if (position <= -15)
	{
		wall_Torque -= wall_Stiffness*(position+15);
		wall_Torque += -velocity*wall_Damping;
	}

	return wall_Torque/REDUCTION_RATIO;
}
