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
volatile float32_t hapt_paddleAccelFilt; // Filtered paddle acceleration [deg/s].
volatile float32_t hapt_motorTorque; // Motor torque [N.m].

const float32_t filterCutoffFreqSpeed = 100.0f; // Cutoff frequency for the speed/acceleration filter [Hz].
const float32_t filterCutoffFreqAccel = 10.0f; // Cutoff frequency for the speed/acceleration filter [Hz].

volatile bool enable_PID;
volatile float32_t Target_Angle; // Target andle for PID control [deg].
volatile float32_t error_accumulator; //integral of the error for the I in the PID controller
volatile float32_t Kp; //Parameter for PID
volatile float32_t Kd; //Parameter for PID
volatile float32_t Ki; //Parameter for PID

void hapt_Update(void);
float32_t hapt_LowPassFilter(float32_t previousFilteredValue, float32_t input,float32_t dt, float32_t tau);
float32_t PID_Regulator1(float32_t error, float32_t previous_error, float32_t dt);

/**
* @brief Initializes the haptic controller.
*/
void hapt_Init(void)
{
	//Initializing variables
	enable_PID = false;
	Kp = 0.0f;
	Ki = 0.0f;
	Kd = 0.0f;
	hapt_timestamp = 0;
	hapt_motorTorque = 0.0f;
	hapt_paddlePrevPos = 0.0f;
	hapt_paddlePrevSpeed = 0.0f;
	hapt_paddlePosFilt = 0.0f;
	hapt_paddleSpeedFilt = 0.0f;
	hapt_paddleAccelFilt = 0.0f;
	Target_Angle = 0.0f;
	error_accumulator = 0.0f;

	// Make the timers call the update function periodically.
	cbt_SetHapticControllerTimer(hapt_Update, DEFAULT_HAPTIC_CONTROLLER_PERIOD);

	// Share some variables with the computer.
	comm_monitorUint32Func("timestep [us]", cbt_GetHapticControllerPeriod, cbt_SetHapticControllerPeriod);
	comm_monitorFloat("motor_torque [N.m]", (float32_t*)&hapt_motorTorque, READWRITE);
	comm_monitorFloat("encoder_paddle_pos [deg]", (float32_t*)&hapt_encoderPaddleAngle, READONLY);
	//comm_monitorFloat("paddle_pos [deg]", (float32_t*)&hapt_paddleAngle, READONLY);
	//comm_monitorFloat("paddle_speed [deg/s]", (float32_t*)&hapt_paddleSpeed, READONLY);
	//comm_monitorFloat("paddle_accel [deg/s^2]", (float32_t*)&hapt_paddleAccel, READONLY);
	comm_monitorFloat("paddle_pos_filt [deg]", (float32_t*)&hapt_paddlePosFilt, READONLY);
	//comm_monitorFloat("paddle_speed_filt [deg/s]", (float32_t*)&hapt_paddleSpeedFilt, READONLY);
	//comm_monitorFloat("paddle_accel_filt [deg/s^2]", (float32_t*)&hapt_paddleAccelFilt, READONLY);
	comm_monitorFloat("hall_voltage [V]", (float32_t*)&hapt_hallVoltage, READONLY);
	comm_monitorBool("enable PID", (bool*)&enable_PID, WRITEONLY);
	comm_monitorFloat("Target_Angle [deg]", (float32_t*)&Target_Angle, READWRITE);
	comm_monitorFloat("Kp ", (float32_t*)&Kp, READWRITE);
	comm_monitorFloat("Ki ", (float32_t*)&Ki, READWRITE);
	comm_monitorFloat("Kd ", (float32_t*)&Kd, READWRITE);
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

	// Get the Hall sensor voltage.
	hapt_hallVoltage = hall_GetVoltage();

	// Get the encoder position.
	motorShaftAngle = enc_GetPosition();
	hapt_encoderPaddleAngle = motorShaftAngle / REDUCTION_RATIO;

	// Compute the paddle angle from the Hall sensor voltage or the encoder which is then used for the PID.
	hapt_paddleAngle = hapt_hallVoltage * (-33.0f)+81.9f;
	//hapt_paddleAngle = hapt_encoderPaddleAngle;

	//Filter the position value
	hapt_paddlePosFilt = hapt_LowPassFilter(hapt_paddlePosFilt,hapt_paddleAngle, dt,filterCutoffFreqSpeed);

	// Compute the speed and accleration from the paddle angle.
	hapt_paddleSpeed = (hapt_paddlePosFilt - hapt_paddlePrevPos) / dt;
	hapt_paddleAccel = (hapt_paddleSpeed - hapt_paddlePrevSpeed) / dt;

	// Filter the speed and acceleration.
	hapt_paddleSpeedFilt = hapt_LowPassFilter(hapt_paddleSpeedFilt,hapt_paddleSpeed, dt,filterCutoffFreqSpeed);
	hapt_paddleAccelFilt = hapt_LowPassFilter(hapt_paddleAccelFilt, hapt_paddleAccel, dt, filterCutoffFreqAccel);

	// Compute the motor torque, and apply it.
	hapt_motorTorque = 0.0f;

	//Sinusoidal motor torque
	//hapt_motorTorque = 0.0014*sin(2*3.14*hapt_timestamp*1e-6);

	//Reinitialize error_accumulator for I control
	if(!enable_PID)
	{
		error_accumulator = 0;
	}

	//Calculate motor torque with PID
	if(enable_PID)
	{
		hapt_motorTorque = PID_Regulator1(Target_Angle-hapt_paddlePosFilt,Target_Angle-hapt_paddlePrevPos,dt)/REDUCTION_RATIO;
	}

	torq_SetTorque(hapt_motorTorque);

	//Store the previous position and speed for derivative purposes
	hapt_paddlePrevPos = hapt_paddlePosFilt;
	hapt_paddlePrevSpeed = hapt_paddleSpeed;

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

	error_accumulator += error;
	return -(error*Kp + Kd*(error-previous_error)/dt+Ki*error_accumulator);

}
