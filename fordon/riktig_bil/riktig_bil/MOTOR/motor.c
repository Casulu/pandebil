#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>
#include "motor.h"
#include "drive_register.h"

bool motor_forward_lock;
bool motor_backward_lock;
bool motor_lock;
int8_t motor_speed_l;
int8_t motor_speed_r;

bool motor_startup_l;
bool motor_startup_r;
uint8_t timer;

/*
 * Initializes timer0 with fast pwm and the rest of what the motors need.
 */
void motors_init()
{
	MOTOR_PWM_DDR |= MOTOR_L_PWM | MOTOR_R_PWM;
	
	MOTOR_L_F_DDR |= MOTOR_L_FORWARD;
	MOTOR_L_B_DDR |= MOTOR_L_BACKWARD;
	
	MOTOR_L_F_PORT |= MOTOR_L_FORWARD;
	MOTOR_L_B_PORT |= MOTOR_L_BACKWARD;
	
	MOTOR_R_F_DDR |= MOTOR_R_FORWARD;
	MOTOR_R_B_DDR |= MOTOR_R_BACKWARD;
	
	MOTOR_R_F_PORT |= MOTOR_R_FORWARD;
	MOTOR_R_B_PORT |= MOTOR_R_BACKWARD;
	
	TCCR0A |= ((1<<COM0A1) | (1<<COM0B1) | (1<<WGM01) | (1<<WGM00));
	TCCR0B |= (1<<CS00);
	
	OCR0A = 0;
	OCR0B = 0;
	motor_speed_l = 0;
	motor_speed_r = 0;
	
	motor_forward_lock = false;
	motor_backward_lock = false;
	motor_lock = false;
}

/*
 * Handles the startup process for the vehicle in lower speeds. Should be called on a 1kHz frequency.
 */
void motor_tick()
{
	if (motor_startup_l == true && timer == 0)
	{
		MOTOR_L_SPEED = (abs(motor_speed_l) * 2);
		motor_startup_l = false;
	}
	
	if (motor_startup_r == true && timer == 0)
	{
		MOTOR_R_SPEED = (abs(motor_speed_r) * 2);
		motor_startup_r = false;
	}
	
	if ((motor_startup_l == true || motor_startup_r == true) && timer > 0)
	{
		timer--;
	}
}

/*
 * Assigns given speed to the motors.
 */
void motors_set_speed(int8_t speed_l, int8_t speed_r)
{
	if (drive_register_registers() == true)
	{
		//If left motor is lock
		if ((speed_l < 0 && motor_backward_lock == true) || (speed_l > 0 && motor_forward_lock == true) || (motor_lock == true))
		{
			speed_l = 0;
		}
		
		//If right motor is locked
		if ((speed_r < 0 && motor_backward_lock == true) || (speed_r > 0 && motor_forward_lock == true) || (motor_lock == true))
		{
			speed_r = 0;
		}
		
		//Lower speeds is problematic and will not be used
		if (abs(speed_l) < 10)
		{
			speed_l = 0;
		}
		
		//Lower speeds is problematic and will not be used.
		if (abs(speed_r) < 10)
		{
			speed_r = 0;
		}
		
		//Registers how the motors moves.
		drive_register_add_entry();
		drive_register_register_entry(speed_l, speed_r);
	}
	
	if (speed_l > 0)
	{
		//Left motor forward
		MOTOR_L_F_PORT |= MOTOR_L_FORWARD;
		MOTOR_L_B_PORT &=~ MOTOR_L_BACKWARD;
	}
	else if (speed_l < 0)
	{
		//Left motor backward
		MOTOR_L_F_PORT &=~ MOTOR_L_FORWARD;
		MOTOR_L_B_PORT |= MOTOR_L_BACKWARD;
	}
	else
	{
		//Left motor lock
		//Active breaking
		MOTOR_L_F_PORT |= MOTOR_L_FORWARD;
		MOTOR_L_B_PORT |= MOTOR_L_BACKWARD;	
		speed_l = 127;
	}
	
	if (speed_r > 0)
	{
		//Right motor forwards
		MOTOR_R_F_PORT |= MOTOR_R_FORWARD;
		MOTOR_R_B_PORT &=~ MOTOR_R_BACKWARD;
	}
	else if (speed_r < 0)
	{
		//Right motor backward
		MOTOR_R_F_PORT &=~ MOTOR_R_FORWARD;
		MOTOR_R_B_PORT |= MOTOR_R_BACKWARD;
	}
	else
	{
		//Right motor lock
		//Active breaking
		MOTOR_R_F_PORT |= MOTOR_R_FORWARD;
		MOTOR_R_B_PORT |= MOTOR_R_BACKWARD;
		speed_r = 127;
	}
	
	//If speed is low enough to need a startup process
	if (MOTOR_L_SPEED == 0 && abs(speed_l) < 30 && abs(speed_l) > 0)
	{
		//Set to have max speed for 20ms
		MOTOR_L_SPEED = 0xff;
		motor_speed_l = (abs(speed_l) * 2);
		motor_startup_l = true;
		timer = 20;
	}
	else
	{
		//Set speed on left motor
		MOTOR_L_SPEED = (abs(speed_l) * 2);
	}
	
	//If speed is low enough to need a startup process
	if (MOTOR_R_SPEED == 0 && (abs(speed_r) < 30) && abs(speed_r) > 0)
	{
		//Set to have max speed for 20ms
		MOTOR_R_SPEED = 0xff;
		motor_speed_r = (abs(speed_r) * 2);
		motor_startup_r = true;
		timer = 20;
	}
	else
	{
		//Set speed on right motor
		MOTOR_R_SPEED = (abs(speed_r) * 2);
	}
}

/*
 * Sets the forward moving lock.
 */
void motor_forward_set_lock(bool lock)
{
	if (motor_forward_lock == false && lock == true)
	{
		motor_forward_lock = lock;
		motors_set_speed(0, 0);
	}
	else
	{
		motor_forward_lock = lock;
	}
}

/*
 * Sets the backward moving lock.
 */
void motor_backward_set_lock(bool lock)
{
	if (motor_backward_lock == false && lock == true)
	{
		motor_backward_lock = lock;
		motors_set_speed(0, 0);
	}
	else
	{
		motor_backward_lock = lock;
	}
}

/*
 * Sets the general lock.
 */
void motor_set_lock(bool lock)
{
	if (motor_lock == false && lock == true)
	{
		motor_lock = lock;
		motors_set_speed(0, 0);
	}
	else
	{
		motor_lock = lock;
	}
}