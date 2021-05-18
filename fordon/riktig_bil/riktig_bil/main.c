#include <stdio.h>
#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "SPI/spi.h"
#include "SPI/display.h"
#include "I2C/i2cmaster.h"
#include "I2C/portextender.h"
#include "I2C/SRF02.h"
#include "UART/uart.h"
#include "PIEZO/piezo.h"
#include "MOTOR/motor.h"
#include "WS2812/ws2812_config.h"
#include "WS2812/light_ws2812.h"

#define NUM_LEDS 19
#define REMOTE_AX (1<<0)
#define REMOTE_JOY (1<<1)

#define EMERGENCY (1<<0)
#define NO_DEADMAN (1<<1)
#define BOTH_DEADMAN (1<<2)
#define NO_HEARBEAT (1<<3)

#define CLOSE_TO_WALL_F (1<<0)
#define CLOSE_TO_WALL_B (1<<1)

#define RED_LED_PORT PORTC
#define RED_LED_DDR DDRC
#define RED_LED (1<<0)

void spi_master_init(void);
void display_init(void);

static FILE mystdout = FDEV_SETUP_STREAM(display_put_char,
NULL,_FDEV_SETUP_WRITE);

volatile char line_buffer[40];
volatile uint8_t line_ind = 0;

volatile char deadman_switches;

/*
 * Bit 0: Emergency break
 * Bit 1: No deadman switches
 * Bit 2: Both deadman switches
 * Bit 3: No hearbeat
 */
volatile char control;

volatile char close_to_wall;

struct cRGB leds[NUM_LEDS];

int main(void)
{
	stdout = &mystdout;
	display_port_init();
	spi_master_init();
	_delay_ms(4);
	display_init();
	uart_init();
	uart_flush();
	motors_init();
	piezo_init();
	i2c_init();
	portextender_write(0xff);
	PORTC |= (1<<PORTC0);
	DDRC |= (1<<DDC0);
	for (int i = 0; i < NUM_LEDS; i++)
	{
		leds[i].r = 0;
		if (i % 2 == 1)
		{
			leds[i].b = 255;
			leds[i].g = 0;
		}
		else
		{
			leds[i].b = 0;
			leds[i].g = 255;
		}
	}
	ws2812_setleds(leds, NUM_LEDS);
	sei();
	char msg[17] = "1 1 ";
	uint8_t len = 0;
    while (1) 
    {
		//int distance_front = srf02_get_distance(SRF_FRONT);
		//int distance_back = srf02_get_distance(SRF_BACK);
		int distance_front = 20;
		int distance_back = 20;
		char portextender_data = portextender_read();
		
		itoa(distance_front, msg+4, 10);
		len = strlen(msg);
		msg[len] = ' ';
		msg[++len] = '\0';
		itoa(distance_back, msg+len, 10);
		len = strlen(msg);
		msg[len] = ' ';
		msg[++len] = '\0';
		for(uint8_t i = 2; i < 6; i++){
			msg[len++] = ((portextender_data & (1<<i)) > 0)+'0';
		}
		msg[len] = '\0';
		uart_send_line(msg);
		_delay_ms(1000);
    }
}

void timer_init()
{
	OCR2A = 100;
	TCCR2A |= (1<<WGM21);
	TCCR2B |= (1<<CS21);
	TIMSK2 |= (1<<OCIE2A);
}

ISR(INT0_vect)
{
	
}

ISR(INT1_vect)
{
	
}

ISR(TIMER2_COMPA_vect)
{
}

ISR(USART_RX_vect){
	line_buffer[line_ind] = UDR0;
	if(line_buffer[line_ind] == '\n'){
		line_buffer[line_ind] = '\0';
		//Do something
		
		if(line_buffer[2] == '0')
		{
			//Hearbeat
		}
		else if(line_buffer[2] == '1')
		{
			//Textmeddelande
			display_position_cursor(32);
			int i = 3;
			while(line_buffer[i] != '\0')
			{
				display_char(line_buffer[i]);
				i++;
			}
			for (;i < 15; i++)
			{
				display_char(' ');
			}
		}
		else if(line_buffer[2] == '2')
		{
			//Spela låt / tuta
		}
		else if(line_buffer[2] == '3')
		{
			//Sätt hjulfart
			PORTC ^= (1<<PORTC0);
			motors_set_speed(line_buffer[4], line_buffer[5]);
		}
		else if(line_buffer[2] == '4')
		{
			//Deadman set
			if (line_buffer[4] & (1<<1))
			{
				if (line_buffer[4] & (1<<0))
				{
					deadman_switches |= REMOTE_AX;
				}
				else
				{
					deadman_switches &=~ REMOTE_AX;
				}
			}
			else
			{
				if (line_buffer[4] & (1<<0))
				{
					deadman_switches |= REMOTE_JOY;
				}
				else
				{
					deadman_switches &=~ REMOTE_JOY;
				}
			}
			if (deadman_switches == 0b00000011)
			{
				control |= BOTH_DEADMAN;
				control &=~ NO_DEADMAN;
			}
			else if (deadman_switches == 0b00000000)
			{
				control &=~ BOTH_DEADMAN;
				control |= NO_DEADMAN;
			}
			else
			{
				control &=~ BOTH_DEADMAN;
				control &=~ NO_DEADMAN;
			}
		}
		else if(line_buffer[2] == '5')
		{
			//Nödstopp
			control |= EMERGENCY;
		}
		else if(line_buffer[2] == '6')
		{
			//Kvittera nödstopp
			control &=~ EMERGENCY; 
		}
		else if(line_buffer[2] == '7')
		{
			//PING
			uart_send_line("1 2");
		}
		
		
		/*if(line_buffer[2] == 'w' || line_buffer[2] == 'W')
		{
			motors_set_speed(line_buffer[4], line_buffer[5], ((line_buffer[6] & 0b00010000)>>4), (line_buffer[6] & 0b00000001));
		}*/
		line_ind = 0;
		} else{
		line_ind++;
	}
}