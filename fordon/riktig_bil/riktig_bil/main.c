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
#include "PIEZO/song.h"
#include "PIEZO/song_library.h"
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

volatile uint16_t heartbeat_timer;

volatile uint16_t sensor_timer;

/*
 * Bit 0: Emergency break
 * Bit 1: Deadman stop
 * Bit 2: No hearbeat
 */
volatile char control;

volatile char close_to_wall;

struct cRGB leds[NUM_LEDS];
volatile char song_timer;

int main(void)
{
	song_timer = 0;
	heartbeat_timer = 0;
	sensor_timer = 0;
	control = 0;
	control |= DEADMAN_STOP;
	stdout = &mystdout;
	display_port_init();
	spi_master_init();
	_delay_ms(500);
	display_init();
	uart_init();
	uart_flush();
	motors_init();
	piezo_init();
	i2c_init();
	timer_init();
	portextender_write(0xff);
	RED_LED_PORT &=~ RED_LED;
	RED_LED_DDR |= RED_LED;
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
	char msg[17] = "11";
	uint8_t len = 0;
	int distance_front = 0;
	int distance_back = 0;
	//srf02_trigger(SRF_FRONT);
	//srf02_trigger(SRF_BACK);
	srf02_timer_reset();
    while (1) 
    {
		if (srf02_timer_alarm() == true)
		{
			/*distance_front = srf02_read(SRF_FRONT);
			distance_back = srf02_read(SRF_BACK);
			srf02_trigger(SRF_FRONT);
			srf02_trigger(SRF_BACK);*/
			distance_front = 17;
			distance_back = 17;
			srf02_timer_reset();
		}
		distance_front = 17;
		distance_back = 17;
		if (distance_front <= (15 + 12) && distance_front > 0)
		{
			close_to_wall |= CLOSE_TO_WALL_F;
			leds[0].b = 255;
			leds[0].g = 0;
			leds[0].r = 0;
			leds[1].b = 255;
			leds[1].g = 0;
			leds[1].r = 0;
			leds[2].b = 255;
			leds[2].g = 0;
			leds[2].r = 0;
			leds[3].b = 255;
			leds[3].g = 0;
			leds[3].r = 0;
		}
		else
		{
			close_to_wall &=~ CLOSE_TO_WALL_F;
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
		}
		
		if (distance_back <= (15 + 12) && distance_back > 0)
		{
			close_to_wall |= CLOSE_TO_WALL_B;
			leds[4].b = 255;
			leds[4].g = 0;
			leds[4].r = 0;
			leds[5].b = 255;
			leds[5].g = 0;
			leds[5].r = 0;
			leds[6].b = 255;
			leds[6].g = 0;
			leds[6].r = 0;
			leds[7].b = 255;
			leds[7].g = 0;
			leds[7].r = 0;
		}
		else
		{
			close_to_wall &=~ CLOSE_TO_WALL_B;
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
		}
		ws2812_setleds(leds, NUM_LEDS);
		
		char portextender_data = portextender_read();
		if (sensor_timer == 0)
		{
			for(uint8_t i = 2; i < 6; i++){
				msg[i] = ((portextender_data & (1<<i)) > 0)+'0';
			}
			msg[6] = ' ';
			itoa(distance_front, msg+7, 10);
			len = strlen(msg);
			msg[len] = ' ';
			msg[++len] = '\0';
			itoa(distance_back, msg+len, 10);
			len = strlen(msg);
			msg[len] = '\0';
			uart_send_line(msg);
			sensor_timer = 2500;
		}
		
		if (control != 0)
		{
			motor_set_lock(true);
		}
		else
		{
			motor_set_lock(false);
		}
		
		if(close_to_wall & CLOSE_TO_WALL_F)
		{
			motor_forward_set_lock(true);
		}
		else
		{
			motor_forward_set_lock(false);
		}
		
		if(close_to_wall & CLOSE_TO_WALL_B)
		{
			motor_backward_set_lock(true);
		}
		else
		{
			motor_backward_set_lock(false);
		}
		display_buff();
    }
}

void perform_command(uint8_t topic, uint8_t command, volatile uint8_t* args)
{
	switch(command){
		case '0':
		//Hearbeat
		heartbeat_timer = 30000;
		break;
		case '1':
		//Textmeddelande
		display_add_to_buff((char*)args, 16, 32);
		break;
		case '2':
		//Spela låt / tuta
		switch(args[0])
		{
			case '0':
			set_imperial_march();
			song_start();
			break;
			case '1':
			set_halo_theme();
			song_start();
			break;
			case '2':
			set_soviet_anthem();
			song_start();
			break;
			case '3':
			set_mario();
			song_start();
			break;
			case '4':
			set_allstars();
			song_start();
			break;
			case '5':
			set_mii();
			song_start();
			break;
			case '6':
			set_cantina_band();
			song_start();
			break;
			case '7':
			song_stop();
			break;
		}
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
			portextender_port_in(GREEN_LED);
			portextender_port_in(YELLOW_LED);
			break;
			case 3:
			control |= DEADMAN_STOP;
			portextender_port_in(GREEN_LED);
			portextender_port_out(YELLOW_LED);
			break;
			default:
			control &=~ DEADMAN_STOP;
			portextender_port_out(GREEN_LED);
			portextender_port_in(YELLOW_LED);
			break;
		}
		break;
		case '5':
		//Nödstopp
		control |= EMERGENCY;
		RED_LED_PORT |= RED_LED;
		break;
		case '6':
		//Kvittera nödstopp
		control &=~ EMERGENCY;
		RED_LED_PORT &=~ RED_LED;
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
		//control |= NO_HEARBEAT;
	}
	else
	{
		control &=~ NO_HEARBEAT;
		heartbeat_timer--;
	}
	
	if (song_playing() == true)
	{
		song_timer++;
	}
	else
	{
		song_timer = 0;
	}
	
	if (song_timer == 10)
	{
		song_play();
		song_timer = 0;
	}
	
	if (sensor_timer > 0)
	{
		sensor_timer--;
	}
	
	srf02_timer_tick();
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