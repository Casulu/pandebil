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
#include <avr/pgmspace.h>
#include "uart.h"
#include "fiveinarow.h"
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
#define GAMEBTN 6
#define LED_DDR DDRD
#define LED_PORT PORTD
#define LEDG 5
#define LEDR1 6
#define LEDR2 7
#define FRONT_POS 5
#define BACK_POS 20
#define CL_POS 30
#define CR_POS 31
#define PIR_POS 15
#define CMD_DELAY 20
#define DEBOUNCE_VAL 20

#define MAIN_MODE 0
#define FIR_MODE 1

#define SETBIT(x, y) ((x) |= (1<<(y)))		//Sets bit y on port x
#define CLEARBIT(x, y) ((x) &= ~(1<<(y)))	//Clears bit y on port x
#define CHANGEBIT(x, y, z) {\
				if(z){\
					SETBIT(x, y);\
				} else CLEARBIT(x, y);\
			}
#define DEADMAN_PRESSED (!(BTN_PIN & 1<<DEADMANBTN))

void timer_init();
void btn_init();
void led_init();
void do_normal_program();
void do_fir_actions();
void perform_command(uint8_t topic, uint8_t command, volatile uint8_t* args);
void display_sensors();
void print_lcd_static();
void send_move_command();
void enter_main_mode();

volatile uint8_t uart_linebuf[20];
volatile uint8_t uart_bufind = 0;

volatile uint8_t heart_hundreths = 0;
volatile uint8_t command_hundreths = 0;
volatile uint8_t debounce_hundreths = 0;

volatile char sensor_buf[16];
volatile bool sensors_received = false;

volatile uint16_t x_rest = 512;
volatile uint16_t y_rest = 501; 
volatile uint16_t z_rest = 633;
int16_t last_left = 0;
int16_t last_right = 0;
char itoabuf[15] = "03";

volatile uint8_t program_mode = MAIN_MODE;
volatile bool game_prompted = false;
volatile bool controller_rested = true;
volatile bool redraw = true;
volatile bool request_sent = false;
volatile bool punished = false;
volatile uint16_t timeout_hundreths = 0;
volatile uint8_t game_over = 0;
char placebuf[5];
char row_buf[49];


int main(void)
{
	spi_init();
	_delay_ms(100);
	lcd_init();
	fiveinarow_init();
	adc_init();
	uart_init();
	uart_flush();
	led_init();
	btn_init();
	Summer_Init();
	timer_init();
	print_lcd_static();
	sei();
	
    while (1) 
    {
		switch(program_mode){
			case MAIN_MODE:
				do_normal_program();
				break;
			case FIR_MODE:
				do_fir_actions();
				break;
		}
	}
}

void do_fir_actions(){
	if(game_over !=  0){
		_delay_ms(1000);
		enter_main_mode(game_over == 1 ? "You won!" : "You lost!");
	} else{
		if(DEADMAN_PRESSED){
			fiveinarow_place(placebuf, '3');
			if(placebuf[0] != '\0'){
				if(fiveinarow_check_win()){
					game_over = 1;
				}
				uart_send_line(placebuf);
				redraw = true;
			}
		}
		
		uint16_t x = read_avg_adc(1, 20);
		uint16_t y = read_avg_adc(2, 20);
		
		if(read_avg_adc(0, 5) > z_rest-5) controller_rested = true;
		
		if(controller_rested){
			if(x > x_rest+40){
				fiveinarow_down();
				redraw = true;
				controller_rested = false;
			}
			else if(x < x_rest-45){
				fiveinarow_up();
				redraw = true;
				controller_rested = false;
			}
			if(y > y_rest+25){
				fiveinarow_left();
				redraw = true;
				controller_rested = false;
			}
			else if(y < y_rest-25){
				fiveinarow_right();
				redraw = true;
				controller_rested = false;
			}
			
		}
		if(redraw){
			fiveinarow_render(row_buf);
			//Draw field
			set_cursor_pos(0);
			write_lcd_string(row_buf);
		}	
	}
	_delay_ms(50);
}

void enter_main_mode(char* message){
	program_mode = MAIN_MODE;
	game_prompted = false;
	redraw = true;
	request_sent = false;
	messages_force(message);
}

void enter_fir_mode(){
	program_mode = FIR_MODE;
	game_over = 0;
	redraw = true;
	if(LED_PORT & (1<<LEDG)){
		uart_send_line("040");
		LED_PORT &= ~(1<<LEDG);
	}
}

void do_normal_program(){
	if(game_prompted){
		if(redraw){
			clear_LCD();
			set_cursor_pos(16);
			write_lcd_string("Want to play?");
			redraw = false;
		}
		if(DEADMAN_PRESSED){
			enter_fir_mode();
			fiveinarow_setup(true);
			_delay_ms(100);
		}
	} else if(redraw){
		clear_line(0);
		clear_line(1);
		print_lcd_static();
		redraw = false;
	} else{
		messages_move_queue();
		if(!output_buf_read){
			set_cursor_pos(32);
			char* buffer = (char*)messages_get_buffer();
			/*If buffer is empty, write clear line. Otherwise write buffer*/
			write_lcd_string((buffer[0] != '\0') ? buffer :  "                ");
		}
		if(sensors_received){
			display_sensors();
			sensors_received = false;
		}
		if(DEADMAN_PRESSED && !punished){
			if(!(LED_PORT & (1<<LEDG))){
				uart_send_line("041");
				LED_PORT |= 1<<LEDG;
			}
			send_move_command();
		} else if(LED_PORT & (1<<LEDG)){
			uart_send_line("040");
			LED_PORT &= ~(1<<LEDG);
		}
		if(!(PINB & 1<<GAMEBTN) && !request_sent){
			uart_send_line("33.");
			messages_force("Invitation sent");
			request_sent = true;
			timeout_hundreths = 0;
		}
	}
	_delay_ms(50);
}

void send_move_command(){
	if(command_hundreths == CMD_DELAY){
		command_hundreths = 0;
		int16_t adjusted_x = x_rest - read_avg_adc(1, 25);
		int16_t adjusted_y = y_rest - read_avg_adc(2, 25);
		int16_t left = adjusted_x+adjusted_y;
		int16_t right = adjusted_x-adjusted_y;
		/*Keep values within bounds*/
		if(left > 127){
			left = 127;
		}
		else if(left < -127){
			left = -127;
		} else if(left > -5 && left < 5) left = 0;
		
		if(right > 127) {
			right = 127;
		}
		else if(right < -127){
			right = -127;
		} else if(right > -5 && right < 5) right = 0;
		
		/*Ignore duplicate values*/
		if(left != last_left || right != last_right){
			/*Build output string*/
			itoa(left, itoabuf+2, 10);
			uint8_t len = strlen(itoabuf);
			itoabuf[len++] = ' ';
			itoa(right, itoabuf+len, 10);
			/*Send string*/
			uart_send_line(itoabuf);
		}
		last_left = left;
		last_right = right;
	}
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
	if(command_hundreths < CMD_DELAY){
		command_hundreths++;
	}
	if(debounce_hundreths > 0){
		debounce_hundreths--;
	}
	if(timeout_hundreths < 5000){
		timeout_hundreths++;
	} else{
		request_sent = false;
		timeout_hundreths = 0;
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

void perform_command(uint8_t topic, uint8_t command, volatile uint8_t* args){
	switch(topic){
		case '1': //From car
			switch(command){
				case '0':
					messages_force("Stopp!          ");
					break;
				case '1':
					strcpy((char*)sensor_buf, (char*)args);
					sensors_received = true;
					break;
				case '2':
					messages_queue("PONG            ");
			}
			break;
		case '2': //To remote
			switch(command){
				case '0':
					LED_PORT &= ~(1<<LEDR2);
					break;
				case '1':
					messages_queue((char*)args);
					break;
				case '2':
					Summer_PlayMelody(args[0]-'0');
					break;
				case '3':
					if(program_mode == FIR_MODE){
						if(*args >= 'A'){
							enter_main_mode("Game canceled");
						} else{
							fiveinarow_recive((char*)(args-2));
							if(fiveinarow_check_win()){
								game_over = 2;
								//Punishment
							}
							redraw = true;
						}
					} else if(*args < '0'){
						game_prompted = true;
						redraw = true;
					} else{
						enter_fir_mode();
						fiveinarow_setup(false);
						fiveinarow_recive((char*)(args-2));
					}
					break;
				case '4':
					if(args[0] == '1'){
						SETBIT(LED_PORT, LEDR1);
					} else{
						CLEARBIT(LED_PORT, LEDR1);
						punished = false;
					}
					break;
			}
			break;
	}
}

void display_sensors(){
	char* args = (char*)sensor_buf;
	set_cursor_pos(CR_POS);
	write_lcd_char((args[0] == '0') ? 'R' : '_');
	set_cursor_pos(CL_POS);
	write_lcd_char((args[1] == '0') ? 'L' : '_');
	set_cursor_pos(PIR_POS);
	write_lcd_char((args[3] == '0') ? 'X' : '_');
	if(args[2] == '0') Summer_PlayMelody(MELODY_HONK);
	
	
	uint8_t b = 5;
	while(args[++b] != ' ');
	args[b++] = '\0';
	uint8_t k = b;
	while(args[++k] != ' ');
	args[k++] = '\0';
	
	
	set_cursor_pos(FRONT_POS);
	if(b == 7 && args[0] == '0'){ //0 is sent for front
		write_lcd_string("!!!");
	} else{
		for (int8_t i = 3-strlen(args+5); i > 0; i--) write_lcd_char(' ');
		write_lcd_string(args+5);
	}
	
	set_cursor_pos(BACK_POS);
	if(k-b == 2 && args[b] == '0'){ //0 is sent for back
		write_lcd_string("!!!");
	} else{
		for (int8_t i = 3-strlen(args+b); i > 0; i--) write_lcd_char(' ');
		write_lcd_string(args+b);
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
	DDRB &= ~(1<<GAMEBTN);
	PORTB |= 1<<GAMEBTN;
	EICRA = (2<<ISC00)|(2<<ISC10);
	EIMSK = 3;
}

//Extra
ISR(INT0_vect){
	if(program_mode == FIR_MODE){
		enter_main_mode("Game canceled");
		uart_send_line("33A");
		punished = true;
	} else{
		if(game_prompted){
			game_prompted = false;
			redraw = true;
		} else if((BTN_PIN & 1<<DEADMANBTN) && (read_adc(0) > z_rest-5)){
			z_rest = read_avg_adc(0, 25);
			x_rest = read_avg_adc(1, 25);
			y_rest = read_avg_adc(2, 25);
		}
	}
}

//Honk
ISR(INT1_vect){
	if(debounce_hundreths == 0){
		if(DEADMAN_PRESSED){
			uart_send_line("020");
			} else{
			uart_send_line("32");
		}
		debounce_hundreths = DEBOUNCE_VAL;	
	}
}

void led_init(){
	LED_DDR |= (1<<LEDG)|(1<<LEDR1)|(1<<LEDR2);
	LED_PORT &= ~((1<<LEDG)|(1<<LEDR1));
	LED_PORT |= 1<<LEDR2;
}

