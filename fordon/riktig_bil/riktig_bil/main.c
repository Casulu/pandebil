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
#include "MOTOR/drive_register.h"
#include "WS2812/ws2812_config.h"
#include "WS2812/light_ws2812.h"

#define NUM_LEDS 19
#define REMOTE_AX (1<<0)
#define REMOTE_JOY (1<<1)

#define EMERGENCY (1<<0)
#define DEADMAN_STOP (1<<1)
#define NO_HEARBEAT (1<<2)

#define RED_LED_PORT PORTC
#define RED_LED_DDR DDRC
#define RED_LED (1<<0)

void spi_master_init(void);
void display_init(void);
void perform_command(uint8_t topic, uint8_t command, volatile uint8_t* args);
void timer_init();
void ledstrip_init();

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
volatile uint8_t control;

int main(void)
{
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
	motors_init();
	piezo_init();
	i2c_init();
	timer_init();
	portextender_set_data(0xff);
	drive_register_init();
	RED_LED_PORT &=~ RED_LED;
	RED_LED_DDR |= RED_LED;
	ledstrip_init();
	sei();
	char msg[17] = "11";
	uint8_t len = 0;
	uint16_t distance_front = 0;
	uint16_t distance_back = 0;
	srf02_trigger(SRF_FRONT);
	srf02_trigger(SRF_BACK);
	srf02_timer_reset();
    while (1) 
    {
	    //Write set data to portextender
	    portextender_write();
		
	    //Every 70ms recieve data from the range sensor and send another sound wave.
		if (srf02_timer_alarm() == true)
		{
			distance_front = srf02_read(SRF_FRONT);
			distance_back = srf02_read(SRF_BACK);
			srf02_trigger(SRF_FRONT);
			srf02_trigger(SRF_BACK);
			srf02_timer_reset();
		}
		
		//Set if vehicle is close to the wall infront it or not
		if (distance_front <= (15 + 12) && distance_front > 0) //Sensor is about 11.5?12 cm from edge, if 0 sensor did not detect anything
		{
			motor_forward_set_lock(true);
		}
		else
		{
			motor_forward_set_lock(false);
		}
		
		//Set if vehicle is close to the wall behind it or not
		if (distance_back <= (15 + 12) && distance_back > 0) //Sensor is about 11.5?12 cm from edge, if 0 sensor did not detect anything
		{
			motor_backward_set_lock(true);
		}
		else
		{
			motor_backward_set_lock(false);
		}
		
		uint8_t portextender_data = portextender_read();
		
		//Send sensor data every quarter second.
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
			sensor_timer = 250;
		}
		
		display_buff(); //Displayes the content of a display buffer
    }
}

/*
 * Sets a initiali color on the ledstip.
 */
void ledstrip_init()
{
	struct cRGB leds[NUM_LEDS];
	
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
}

/*
 * Initializes a 1kHz CTC timer.
 */
void timer_init()
{
	OCR2A = 125;
	TCCR2A |= (1<<WGM21);
	TCCR2B |= (1<<CS22);
	TIMSK2 |= (1<<OCIE2A);
}

/*
 * Preforms a command from MQTT.
 */
void perform_command(uint8_t topic, uint8_t command, volatile uint8_t* args)
{
	switch(command){
		case '0':
		//Hearbeat received
		heartbeat_timer = 3000;
		break;
		case '1':
		//Textmessage recieved
		display_add_to_buff((char*)args, 16, 32); //Display textmessage
		break;
		case '2':
		//Play song / horn received
		switch(args[0])
		{
			case '0':
			song_set(get_horn_song());
			song_start();
			break;
			case '1':
			song_stop();
			break;
			case '2':
			song_set(get_cantina_band_song());
			song_start();
			break;
			case '3':
			song_set(get_imperial_march_song());
			song_start();
			break;
			case '4':
			song_set(get_soviet_anthem_song());
			song_start();
			break;
			case '5':
			song_set(get_allstars_song());
			song_start();
			break;
			case '6':
			song_set(get_mii_song());
			song_start();
			break;
			case '7':
			song_set(get_halo_theme_song());
			song_start();
			break;
			case '8':	
			song_set(get_mario_song());
			song_start();
			break;
			case '9':
			song_set(get_mario_galaxy_song());
			song_start();
			break;
			case 'A':
			song_set(get_pokemon_center_song());
			song_start();
			break;
			case 'B':
			song_set(get_petalburg_song());
			song_start();
			break;
		}
		break;
		case '3':
		//Set wheel speed command recieved
		if (args[0] == '\0')
			return;
		if (drive_register_registers() == true)
		{
			uint8_t i = 0;
			while(args[++i] != ' ');
			args[i++] = '\0';
			motors_set_speed(atoi((char*)args), atoi((char*)(args+i))); //Set given wheelspeed
		}
		break;
		case '4':
		//Deadman set/clear
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
		//Set led to indicate if a dead man is set or not
		switch(deadman_switches)
		{
			case 0:
			control |= DEADMAN_STOP;
			portextender_port_in(GREEN_LED);
			break;
			case 3:
			control |= DEADMAN_STOP;
			portextender_port_in(GREEN_LED);
			break;
			default:
			control &=~ DEADMAN_STOP;
			portextender_port_out(GREEN_LED);
			break;
		}
		break;
		case '5':
		//Emergency break
		control |= EMERGENCY;
		//Set led for emergency break
		RED_LED_PORT |= RED_LED;
		break;
		case '6':
		//Clear emergency break
		control &=~ EMERGENCY;
		//Turn off emergency break led
		RED_LED_PORT &=~ RED_LED;
		break;
		case '7':
		//PING recieved
		uart_send_line("12"); //Respond with PONG
		break;
		case '8':
		//Return home command received
			switch(args[0])
			{
				case '0':
				//Activate return to home
				if (drive_register_registers() == true)
				{
					drive_register_backtrack();
				}
				break;
				case '1':
				//Reset drive register
				drive_register_init();
				break;
			}
		break;
	}
	
	//Set if car should break because of emergency break.
	if (control != 0)
	{
		motor_set_lock(true);
		drive_register_init(); //reset drive register
	}
	else
	{
		motor_set_lock(false);
	}
}

/*
 * Timer2 compare interrupts. Frequency is 1kHz.
 * Used for things that is time sensitive.
 */
ISR(TIMER2_COMPA_vect)
{
	if (heartbeat_timer == 0)
	{
		control |= NO_HEARBEAT;
	}
	else
	{
		control &=~ NO_HEARBEAT;
		heartbeat_timer--;
	}
	
	//Set if car should break because of emergency break.
	if (control != 0)
	{
		motor_set_lock(true);
		drive_register_init(); //Reset drive register
	}
	else
	{
		motor_set_lock(false);
	}
	
	//If a song is playing
	if (song_playing() == true)
	{
		song_play(); //Continue song
	}
	
	if (sensor_timer > 0)
	{
		sensor_timer--;
	}
	
	srf02_timer_tick();
	drive_register_tick();
	if (drive_register_registers() == false)
	{
		portextender_port_out(YELLOW_LED);
	}
	else
	{
		portextender_port_in(YELLOW_LED);
	}
	motor_tick();
}

/*
 * UART receive interrupt. Used when receiving MQTT commands. 
 */
ISR(USART_RX_vect){
	uart_linebuf[line_ind] = UDR0;
	if(uart_linebuf[line_ind] == '\n')
	{
		uart_linebuf[line_ind] = '\0';
		perform_command(uart_linebuf[0], uart_linebuf[1], uart_linebuf+2);
		line_ind = 0;
	}
	else
	{
	line_ind++;
	}
}