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
#include "util/messages.h"

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
#define FRONT_POS 5
#define BACK_POS 20
#define CL_POS 30
#define CR_POS 31
#define PIR_POS 15
#define MSGBUF_SIZE 8
#define MSGBUF_BITMASK MSGBUF_SIZE-1

#define mask_circ_ind(x) ((x) & (MSGBUF_BITMASK))
#define SETBIT(x, y) ((x) |= (1<<(y)))		//Sets bit y on port x
#define CLEARBIT(x, y) ((x) &= ~(1<<(y)))	//Clears bit y on port x

void timer_init();
void clear_heart();
void btn_init();
void led_init();
void perform_command(uint8_t topic, uint8_t command, volatile uint8_t* args);
void display_sensors();
void print_lcd_static();
int uart_put_char(char c, FILE* stream);
int lcd_put_char(char c, FILE* stream);


static FILE uartstdout = FDEV_SETUP_STREAM(uart_put_char, NULL, 0);
static FILE lcdstdout = FDEV_SETUP_STREAM(lcd_put_char, NULL, 0);
volatile uint8_t uart_linebuf[20];
volatile uint8_t uart_bufind = 0;
volatile uint8_t heart_hundreths = 0;
volatile char sensor_buf[16];
volatile bool sensors_received = false;
volatile uint16_t x_rest = 511;
volatile uint16_t y_rest = 507; 


int main(void)
{
	int16_t last_left = 0;
	int16_t last_right = 0;
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
		if(!message_is_displaying()) display_next_message();
		if(sensors_received){
			 display_sensors();
			 sensors_received = false;
		}
		if(!(BTN_PIN & 1<<DEADMANBTN)){
			if(!(LED_PORT & (1<<LEDG))){
				uart_send_line("041");
				LED_PORT |= 1<<LEDG;
			}
			int16_t adjusted_x = x_rest - read_avg_adc(1, 25);
			int16_t adjusted_y = y_rest - read_avg_adc(2, 25);
			int16_t left = adjusted_x-adjusted_y;
			int16_t right = adjusted_x+adjusted_y;
			if(left > 127){
				left = 127;
			}
			else if(left < -127){
				left = -127;
			} else if(left > -10 && left < 10) left = 0;
			
			if(right > 127) {
				right = 127;
			}
			else if(right < -127){
				right = -127;
			} else if(right > -10 && right < 10) right = 0;
			
			if(left != last_left || right != last_right){
				itoa(left, itoabuf+2, 10);
				uint8_t len = strlen(itoabuf);
				itoabuf[len++] = ' ';
				itoabuf[len] = '\0';
				itoa(right, itoabuf+len, 10);
				uart_send_line(itoabuf);
			}
			last_left = left;
			last_right = right;
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
	messages_timerproc();
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
					force_message("Stopp!");
					break;
				case '1':
					strcpy((char*)sensor_buf, (char*)args);
					sensors_received = true;
					break;
				case '2':
					queue_message("PONG");
			}
			break;
		case '2': //To remote
			switch(command){
				case '0':
					clear_heart();
					break;
				case '1':
					queue_message((char*)args);
					break;
				case '2':
					Summer_PlayMelody(MELODY_HONK);
					break;
			}
			break;
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
	if(b == 2 && args[0] == '0'){ //0 is sent for front
		write_lcd_string("!!!");
	} else{
		for (int8_t i = 3-strlen(args); i > 0; i--) write_lcd_char(' ');
		write_lcd_string(args);
	}
	
	set_cursor_pos(BACK_POS);
	if(k-b == 2 && args[b] == '0'){ //0 is sent for back
		write_lcd_string("!!!");
	} else{
		for (int8_t i = 3-strlen(args+b); i > 0; i--) write_lcd_char(' ');
		write_lcd_string(args+b);
	}
	set_cursor_pos(CR_POS);
	write_lcd_char((args[k] == '0') ? 'R' : '_');
	set_cursor_pos(CL_POS);
	write_lcd_char((args[k+1] == '0') ? 'L' : '_');
	set_cursor_pos(PIR_POS);
	write_lcd_char((args[k+3] == '0') ? 'X' : '_');
	if(args[k+2] == '0'){
		if(!(LED_PORT & 1<<LEDR1)){
			//Summer_PlayMelody(MELODY_HONK);
			LED_PORT |= 1<<LEDR1;
		}
	} else{
		LED_PORT &= ~(1<<LEDR1);
	}
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
	if((BTN_PIN & 1<<DEADMANBTN) && (read_avg_adc(0, 5) > 630)){
		x_rest = read_avg_adc(1, 25);
		y_rest = read_avg_adc(2, 25);
	}
}

//Honk
ISR(INT1_vect){
	if(!(BTN_PIN & 1<<DEADMANBTN)){
		uart_send_line("020");
	} else{
		uart_send_line("32");
	}
}

void led_init(){
	LED_DDR |= (1<<LEDG)|(1<<LEDR1)|(1<<LEDR2);
	LED_PORT &= ~((1<<LEDG)|(1<<LEDR1));
	LED_PORT |= 1<<LEDR2;
}

