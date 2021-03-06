#ifndef MOTOR_H_
#define MOTOR_H_

#define MOTOR_L_SPEED OCR0B
#define MOTOR_R_SPEED OCR0A
#define MOTOR_L_FORWARD (1<<6)
#define MOTOR_L_BACKWARD (1<<7)
#define MOTOR_L_F_PORT PORTB
#define MOTOR_L_B_PORT PORTB
#define MOTOR_R_FORWARD (1<<0)
#define MOTOR_R_BACKWARD (1<<7)
#define MOTOR_R_F_PORT PORTB
#define MOTOR_R_B_PORT PORTD
#define MOTOR_L_F_DDR DDRB
#define MOTOR_L_B_DDR DDRB
#define MOTOR_R_F_DDR DDRB
#define MOTOR_R_B_DDR DDRD
#define MOTOR_PWM_DDR DDRD
#define MOTOR_L_PWM (1<<5)
#define MOTOR_R_PWM (1<<6)
/*#define MOTOR_ENC_PORT DDRD
#define MOTOR_L_ENC 1<<2
#define MOTOR_R_ENC 1<<3*/

void motors_init();
void motors_set_speed(int8_t speed_r, int8_t speed_l);
void motor_forward_set_lock(bool lock);
void motor_backward_set_lock(bool lock);
void motor_set_lock(bool lock);
void motor_tick();

#endif /* MOTOR_H_ */