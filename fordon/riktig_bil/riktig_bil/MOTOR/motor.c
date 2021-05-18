#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>
#include "motor.h"

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
	
	EIMSK |= (1<<INT0) | (1<<INT1);
	EICRA |= (1<<ISC01) | (1<<ISC11);
}

void motors_set_speed(signed char speed_l, signed char speed_r)
{
	if (speed_l == 0)
	{
		MOTOR_L_F_PORT &=~ MOTOR_L_FORWARD;
		MOTOR_L_B_PORT &=~ MOTOR_L_BACKWARD;
	}
	else if (speed_l > 0)
	{
		MOTOR_L_F_PORT |= MOTOR_L_FORWARD;
		MOTOR_L_B_PORT &=~ MOTOR_L_BACKWARD;
	}
	else
	{
		MOTOR_L_F_PORT &=~ MOTOR_L_FORWARD;
		MOTOR_L_B_PORT |= MOTOR_L_BACKWARD;
	}
	
	if (speed_r == 0)
	{
		MOTOR_R_F_PORT &=~ MOTOR_R_FORWARD;
		MOTOR_R_B_PORT &=~ MOTOR_R_BACKWARD;
	}
	else if (speed_r > 0)
	{
		MOTOR_R_F_PORT |= MOTOR_R_FORWARD;
		MOTOR_R_B_PORT &=~ MOTOR_R_BACKWARD;
	}
	else
	{
		MOTOR_R_F_PORT &=~ MOTOR_R_FORWARD;
		MOTOR_R_B_PORT |= MOTOR_R_BACKWARD;
	}
	MOTOR_L_SPEED = (abs(speed_l) * 2);
	MOTOR_R_SPEED = (abs(speed_r) * 2);
}