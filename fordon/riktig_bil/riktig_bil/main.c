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
#define DEADMAN_STOP (1<<1)
#define NO_HEARBEAT (1<<2)

#define CLOSE_TO_WALL_F (1<<0)
#define CLOSE_TO_WALL_B (1<<1)

#define RED_LED_PORT PORTC
#define RED_LED_DDR DDRC
#define RED_LED (1<<0)

void spi_master_init(void);
void display_init(void);
void perform_command(uint8_t topic, uint8_t command, volatile uint8_t* args);
void timer_init();

static FILE mystdout = FDEV_SETUP_STREAM(display_put_char,
NULL,_FDEV_SETUP_WRITE);

volatile uint8_t uart_linebuf[40];
volatile uint8_t line_ind = 0;

volatile char deadman_switches;

volatile int heartbeat_timer;

/*
 * Bit 0: Emergency break
 * Bit 1: Deadman stop
 * Bit 2: No hearbeat
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
	timer_init();
	portextender_write(0xff);
	PORTC |= (1<<PORTC0);
	DDRC |= (1<<DDC0);
	leds[0].b = 0;
	leds[0].g = 0;
	leds[0].r = 255;
	leds[1].b = 0;
	leds[1].g = 255;
	leds[1].r = 255;
	leds[2].b = 0;
	leds[2].g = 255;
	leds[2].r = 255;
	leds[3].b = 0;
	leds[3].g = 0;
	leds[3].r = 255;
	leds[4].b = 0;
	leds[4].g = 0;
	leds[4].r = 255;
	leds[5].b = 0;
	leds[5].g = 255;
	leds[5].r = 255;
	leds[6].b = 0;
	leds[6].g = 255;
	leds[6].r = 255;
	leds[7].b = 0;
	leds[7].g = 0;
	leds[7].r = 255;
	for (int i = 8; i < NUM_LEDS; i++)
	{
		if (i % 2 == 1)
		{
			leds[i].b = 0;
			leds[i].g = 255;
			leds[i].r = 255;
		}
		else
		{
			leds[i].b = 255;
			leds[i].g = 0;
			leds[i].r = 0;
		}
	}
	ws2812_setleds(leds, NUM_LEDS);
	sei();
	portextender_port_out(YELLOW_LED);
	portextender_port_out(GREEN_LED);
	char msg[17] = "11";
	uint8_t len = 0;
    while (1) 
    {
		//int distance_front = srf02_get_distance(SRF_FRONT);
		//int distance_back = srf02_get_distance(SRF_BACK);
		int distance_front = 20;
		int distance_back = 20;
		char portextender_data = portextender_read();
		
		itoa(distance_front, msg+2, 10);
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

void perform_command(uint8_t topic, uint8_t command, volatile uint8_t* args)
{
	switch(command){
		case '0':
		//Hearbeat
		PORTC ^= (1<<PORTC0);
		heartbeat_timer = 30000;
		break;
		case '1':
		//Textmeddelande
		display_position_cursor(32);
		printf("%s", args);
		break;
		case '2':
		//Spela låt / tuta
		break;
		case '3':
		if (args[0] == '\0')
			return;
		//Sätt hjulfart
		uint8_t i = 0;
		while(args[++i] != ' ');
		args[i++] = '\0';
		motors_set_speed(atoi((char*)args), atoi((char*)(args+i)));
		break;
		case '4':
		//Deadman set
		switch(args[0])
		{
			case '0':
				deadman_switches &=~ REMOTE_AX;
			break;
			case '1':
				deadman_switches |= REMOTE_AX;
			break;
			case '2':
				deadman_switches &=~ REMOTE_JOY;
			break;
			case '3':
				deadman_switches |= REMOTE_JOY;
			break;
		}
		switch(deadman_switches)
		{
			case 0:
			control |= DEADMAN_STOP;
			break;
			case 3:
			control |= DEADMAN_STOP;
			break;
			default:
			control &=~ DEADMAN_STOP;
			break;
		}
		break;
		case '5':
		//Nödstopp
		control |= EMERGENCY;
		break;
		case '6':
		//Kvittera nödstopp
		control &=~ EMERGENCY;
		break;
		case '7':
		//PING
		uart_send_line("12"); /*PONG*/
		break;
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
	if (heartbeat_timer == 0)
	{
		control |= NO_HEARBEAT;
	}
	else
	{
		heartbeat_timer--;
	}
	if (heartbeat_timer%100 == 0)
	{
		display_clear();
		printf("%d", (heartbeat_timer / 1000));
	}
	song_play();
}

ISR(USART_RX_vect){
	uart_linebuf[line_ind] = UDR0;
	if(uart_linebuf[line_ind] == '\n'){
		uart_linebuf[line_ind] = '\0';
		//Do something
		perform_command(uart_linebuf[0], uart_linebuf[1], uart_linebuf+2);
		line_ind = 0;
		} else{
		line_ind++;
	}
}