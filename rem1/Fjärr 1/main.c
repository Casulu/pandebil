/*
 * Fjärr 1.c
 *
 * Created: 2021-05-10 14:54:00
 * Author : Rasmus Lyxell
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "uart.h"
#include "util/lcd.h"
#include "util/spi.h"
#include "util/adc.h"
#include "util/summer.h"

#define BTN_DDR DDRD
#define BTN_PORT PORTD
#define BTN_PIN PIND
#define HONKBTN 3
#define EXTRABTN 2
#define DEADMANBTN 4
#define LED_DDR DDRD
#define LED_PORT PORTD
#define LEDG 5
#define LEDR1 6
#define LEDR2 7
#define MAX_MSG_LEN 16
#define FRONT_POS 6
#define BACK_POS 21
#define CL_POS 30
#define CR_POS 31
#define PIR_POS 15

void timer_init();
void clear_heart();
void btn_init();
void led_init();
void display_next_message();
void perform_command(uint8_t topic, uint8_t command, volatile uint8_t* args);
void queue_message(char* msg, bool urgent);
void display_sensors();
void print_lcd_static();
void force_message(char* msg);
int uart_put_char(char c, FILE* stream);
int lcd_put_char(char c, FILE* stream);


static FILE uartstdout = FDEV_SETUP_STREAM(uart_put_char, NULL, 0);
static FILE lcdstdout = FDEV_SETUP_STREAM(lcd_put_char, NULL, 0);
volatile uint8_t uart_linebuf[40];
volatile uint8_t uart_bufind = 0;
volatile uint8_t heart_hundreths = 0;
volatile uint8_t message_hundreths = 0;
volatile uint8_t message_seconds = 0;
volatile char message_buf[5][MAX_MSG_LEN];
volatile uint8_t message_buf_ind = 0;
volatile uint8_t message_disp_ind = 0;
volatile uint8_t message_counter = 0;
volatile bool message_displaying = false;
volatile char sensor_buf[16];
volatile bool sensors_received = false;


int main(void)
{
	int16_t last_x = 0;
	int16_t last_y = 0;
	char itoabuf[15] = "03";
	
	spi_init();
	_delay_ms(100);
	lcd_init();
	adc_init();
	uart_init();
	uart_flush();
	led_init();
	btn_init();
	Summer_Init();
	timer_init();
	print_lcd_static();
	stdout = &uartstdout;
	sei();
  
    while (1) 
    {
		if(!message_displaying) display_next_message();
		if(sensors_received){
			 display_sensors();
			 sensors_received = false;
		}
		if(!(BTN_PIN & 1<<DEADMANBTN)){
			if(!(LED_PORT & (1<<LEDG))){
				uart_send_line("041");
				LED_PORT |= 1<<LEDG;
			}
			int16_t adjusted_x = 511 - read_avg_adc(1, 25);
			int16_t adjusted_y = 507 - read_avg_adc(2, 25); 
			if(adjusted_x > 127){
				adjusted_x = 127;
			}
			else if(adjusted_x < -127) adjusted_x = -127;
			if(adjusted_y > 127) {
				adjusted_y = 127;
			}
			else if(adjusted_y < -127) adjusted_y = -127;
			if(adjusted_x != last_x || adjusted_y != last_y){
				itoa(adjusted_x-adjusted_y, itoabuf+2, 10);
				uint8_t len = strlen(itoabuf);
				itoabuf[len++] = ' ';
				itoabuf[len] = '\0';
				itoa(adjusted_x+adjusted_y, itoabuf+len, 10);
				uart_send_line(itoabuf);
			}
			last_x = adjusted_x;
			last_y = adjusted_y;
		} else{
			if(LED_PORT & (1<<LEDG)){
				uart_send_line("040");
				LED_PORT &= ~(1<<LEDG);
			}
		}
		_delay_ms(100);
    }
}

int uart_put_char(char c, FILE* stream){
	uart_send_byte(c);
	return 0;
}

int lcd_put_char(char c, FILE* stream){
	write_lcd_char(c);
	return 0;
}

void print_lcd_static(){
	set_cursor_pos(0);
	write_lcd_string("Fram: __cm PIR:_");
	set_cursor_pos(16);
	write_lcd_string("Bak: __cm  C: __");
}

ISR(TIMER2_COMPA_vect){
	if(++heart_hundreths > 50){
		LED_PORT |= 1<<LEDR2;
		heart_hundreths = 0;
	}
	if(message_displaying && ++message_hundreths > 100){
		if(++message_seconds > 5){
			message_seconds = 0;
			message_displaying = false;
			clear_line(2);
		}
		message_hundreths = 0;
	}
}

void timer_init(){
	/* Start 100Hz system timer with TC2 for counting time */
	OCR2A = 77;
	TCCR2A = 1<<WGM21;
	TCCR2B = 7;
	TIMSK2 = 1<<OCIE2A;
}

void clear_heart(){
	LED_PORT &= ~(1<<LEDR2);
}

void perform_command(uint8_t topic, uint8_t command, volatile uint8_t* args){
	switch(topic){
		case '1': //From car
			switch(command){
				case '0':
					queue_message("Stopp!", true);
					break;
				case '1':
					strcpy((char*)sensor_buf, (char*)args);
					sensors_received = true;
					break;
				case '2':
					queue_message("PONG", false);
			}
			break;
		case '2': //To remote
			switch(command){
				case '0':
					clear_heart();
					break;
				case '1':
					queue_message((char*)args, false);
					break;
				case '2':
					Summer_PlayMelody(MELODY_HONK);
					break;
			}
			break;
	}
}

void force_message(char* msg){
	cli();
	message_displaying = true;
	message_hundreths = 0;
	message_seconds = 0;
	clear_line(2);
	set_cursor_pos(32);
	write_lcd_string(msg);
	sei();
}

void queue_message(char* msg, bool urgent){
	if(urgent){
		force_message(msg);
	} else{
		/*if(message_counter == 5){
			force_message((char*)(message_buf)[message_disp_ind++]);
			if(message_disp_ind > 4) message_disp_ind = 0;
			message_counter--;
			message_buf_ind++;
		}*/
		strcpy((char*)(message_buf)[message_buf_ind++], msg);
		if(++message_counter > 5) message_counter = 5;
		if(message_buf_ind > 4) message_buf_ind = 0;
	}
}

void display_next_message(){
	if(message_counter > 0){
		set_cursor_pos(32);
		write_lcd_string((char*)(message_buf)[message_disp_ind++]);
		message_displaying = true;
		if(message_disp_ind > 4) message_disp_ind = 0;
		message_counter--;	
	}
}

void display_sensors(){
	char* args = (char*)sensor_buf;
	uint8_t b = 0;
	while(args[++b] != ' ');
	args[b++] = '\0';
	uint8_t k = b;
	while(args[++k] != ' ');
	args[k++] = '\0';
	set_cursor_pos(FRONT_POS);
	write_lcd_string(args);
	set_cursor_pos(BACK_POS);
	write_lcd_string(args+b);
	set_cursor_pos(CR_POS);
	write_lcd_char((args[k] == '0') ? 'R' : '_');
	set_cursor_pos(CL_POS);
	write_lcd_char((args[k+1] == '0') ? 'L' : '_');
	set_cursor_pos(PIR_POS);
	write_lcd_char((args[k+3] == '0') ? 'X' : '_');
}

ISR(USART_RX_vect){
	uart_linebuf[uart_bufind] = UDR0;
	if(uart_linebuf[uart_bufind] == '\n'){
		uart_linebuf[uart_bufind] = '\0';
		//Do something
		perform_command(uart_linebuf[0], uart_linebuf[1], uart_linebuf+2);
		//Done something
		uart_bufind = 0;
	} else{
		uart_bufind++;
	}
}

void btn_init(){
	BTN_DDR &= ~((1<<HONKBTN)|(1<<DEADMANBTN)|(1<<EXTRABTN));
	BTN_PORT |= (1<<HONKBTN)|(1<<DEADMANBTN)|(1<<EXTRABTN);
	EICRA = (2<<ISC00)|(2<<ISC10);
	EIMSK = 3;
}

//Extra
ISR(INT0_vect){
}

//Honk
ISR(INT1_vect){
	if(!(BTN_PIN & 1<<DEADMANBTN)){
		uart_send_line("02m");
	} else{
		uart_send_line("32");
	}
}

void led_init(){
	LED_DDR |= (1<<LEDG)|(1<<LEDR1)|(1<<LEDR2);
	LED_PORT &= ~((1<<LEDG)|(1<<LEDR1));
	LED_PORT |= 1<<LEDR2;
}

