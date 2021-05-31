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
#include "util/music.h"
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
#define TIMEOUT_VAL 5000
#define MOVE_DELAY_VAL 45

#define NUM_SONGS 5
#define MAX_SONG_LEN 256
#define HONK_SONG 0
#define USSR_SONG 1
#define AMOGUS_SONG 2
#define GERUDO_SONG 3
#define UNTITLED_SONG 4

#define MAIN_MODE 0
#define FIR_MODE 1

#define SETBIT(x, y) ((x) |= (1<<(y)))		//Sets bit y on port x
#define CLEARBIT(x, y) ((x) &= ~(1<<(y)))	//Clears bit y on port x
#define CHANGEBIT(x, y, z) {\
				if(z) SETBIT(x, y);\
				else CLEARBIT(x, y);\
			}
#define DEADMAN_PRESSED (!(BTN_PIN & 1<<DEADMANBTN))
#define GAMEBTN_PRESSED (!(PINB & 1<<GAMEBTN))

void timer_init();
void btn_init();
void led_init();
void do_normal_actions();
void do_fir_actions();
void perform_command(uint8_t topic, uint8_t command, volatile uint8_t* args);
void process_fir_command(volatile uint8_t* args);
void display_sensors();
void print_lcd_static();
void send_move_command();
void enter_main_mode();

static const uint8_t melodies[NUM_SONGS][MAX_SONG_LEN+2] PROGMEM = {
	{ // Basic Honk
		180, 0x71, 0x80, 0x71, 0x00
	},
	{ // USSR
		75, 0x2d, 0x4d, 0x60, 0x68, 0x4d, 0x68, 0x88, 0x8a, 0x4c, 0x85, 0x80, 0x65,
		0x4a, 0x68, 0x88, 0x86, 0x48, 0x81, 0x80, 0x61, 0x63, 0x83, 0x80, 0x63,
		0x83, 0x85, 0x66, 0x86, 0x80, 0x66, 0x86, 0x88, 0x4a, 0x6c, 0x6d, 0x4f,
		0x6f, 0x68, 0x51, 0x6f, 0x8f, 0x8d, 0x4f, 0x6c, 0x68, 0x4d, 0x6c, 0x8c,
		0x8a, 0x4c, 0x85, 0x80, 0x65, 0x4a, 0x68, 0x88, 0x86, 0x48, 0x61, 0x80,
		0x81, 0x4d, 0x6c, 0x8c, 0x8a, 0x68, 0x6c, 0x6d, 0x6f, 0x31, 0x6f, 0x6d,
		0x6c, 0x6d, 0x4f, 0x6f, 0x88, 0x80, 0x68, 0x6c, 0x6d, 0x6f, 0x2d, 0x6c,
		0x6a, 0x68, 0x6a, 0x4c, 0x6c, 0x85, 0x80, 0x65, 0x68, 0x6a, 0x6c, 0x4d,
		0x6a, 0x8a, 0x8c, 0x4d, 0x6a, 0x8a, 0x8c, 0x4d, 0x6a, 0x6d, 0x52, 0x72,
		0x60, 0x32, 0x71, 0x6f, 0x6d, 0x6f, 0x51, 0x71, 0x8d, 0x80, 0x2d, 0x2f,
		0x6d, 0x6c, 0x6a, 0x6c, 0x4d, 0x6d, 0x8a, 0x80, 0x2a, 0x4d, 0x6c, 0x8c,
		0x8a, 0x48, 0x61, 0x80, 0x81, 0x28, 0x4a, 0x4c, 0x2d, 0x6d, 0
	},
	{ //Amogus
		100, 0x8D, 0x80, 0x90, 0x80, 0x92, 0x80, 0x93, 0x80, 0x92, 0x80, 0x90, 0x80, 0x8D, 0x40, 0x80, 0x8B, 0x8F, 0x8D, 0
	},
	{//Gerudo
		100, 0x80, 0x83, 0x88, 0x8a, 0x6b, 0x80, 0x83, 0x88, 0x8a, 0x6b, 0x60, 0x80, 0x80, 0x84, 0x88, 0x8a, 0x6b, 0x80, 0x84, 0x88, 0x8a, 0x6b, 0x80, 0x81, 0x86, 0x88, 0x6a, 0x80, 0x81, 0x86, 0x88, 0x6a, 0x60, 0x80, 0x80, 0x88, 0x8a, 0x88, 0x67, 0
	},
	{ //Untitled - Simple Plan
		96, 0x60, 0x6e, 0xe0, 0x6e, 0xe0, 0x6e, 0x72, 0x73, 0x72, 0x73, 0x53, 0x60, 0x6e, 0x72, 0x73, 0x72, 0x73, 0x53, 0x60, 0x6e, 0x72, 0x73, 0x72, 0x73, 0x53, 0x60, 0x6e, 0x73, 0xe0, 0x73, 0x60, 0x53, 0x72, 0x60, 0x72, 0xe0, 0x52, 0xe0, 0x72, 0x73, 0x72, 0x73, 0x60, 0x6e, 0x72, 0x73, 0x72, 0x73, 0x53, 0x60, 0x6e, 0x72, 0x73, 0xe0, 0x73, 0xe0, 0x73, 0x33, 0x53, 0x40, 0x60, 0x6e, 0xe0, 0x6e, 0xe0, 0x6e, 0x60, 0x72, 0x73, 0x72, 0x73, 0x53, 0
	}
};

/*Uart line buffer*/
volatile uint8_t uart_linebuf[20];
volatile uint8_t uart_bufind = 0;

/*Button debounce check*/
uint8_t gamebtn_last = 0;

/*ISR-counters*/
volatile uint8_t fir_cursor_move_delay = MOVE_DELAY_VAL;
volatile uint8_t heart_hundreths = 0;
volatile uint8_t move_command_hundreths = 0;
volatile uint8_t honk_debounce_hundreths = DEBOUNCE_VAL;
volatile uint16_t invite_timeout_hundreths = 0;

/*Sensor data buffer*/
volatile char sensor_buf[16];
volatile bool sensors_received = false;

/*Rest variables*/
volatile uint16_t x_rest = 512; /*Typical resting values for prototype controller*/
volatile uint16_t y_rest = 501; 
volatile uint16_t z_rest = 633;
/*Stores last outputs*/
int16_t last_left = 0;
int16_t last_right = 0;
/*For writing to car*/
char itoabuf[15] = "03"; //Alwyas starts with "03"

/*Variables for five in a row handling*/
volatile uint8_t program_mode = MAIN_MODE;
volatile bool game_prompted = false;
volatile bool redraw = true;	//Also used for marking redrawing in main mode
volatile bool request_sent = false;
volatile bool punished = false;
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
	music_init();
	timer_init();
	print_lcd_static();
	sei();
    while (1) 
    {
		switch(program_mode){
			case MAIN_MODE:
				do_normal_actions();
				break;
			case FIR_MODE:
				do_fir_actions();
				break;
		}
	}
}

void draw_five_map(){
	fiveinarow_render(row_buf);
	set_cursor_pos(0);
	write_lcd_string(row_buf);
}

void do_fir_actions(){
	if(game_over !=  0){ /*Game is over*/
		draw_five_map();
		_delay_ms(3000);
		enter_main_mode(game_over == 1 ? "You won!" : "You lost!");
	} else{ /*Game is running*/
		if(DEADMAN_PRESSED){ /*If deadmans is pressed, attempt to place at cursor*/
			fiveinarow_place(placebuf, '3');
			if(placebuf[0] != '\0'){ /*If could be placed*/
				music_play_note(NOTE(F_PITCH, 2, SIXTEENTH_NOTE), 100);
				if(fiveinarow_check_win()){ /*Check for win*/
					game_over = 1;
					punished = false;
					music_play_song_pgm(melodies[AMOGUS_SONG]);
				}
				uart_send_line(placebuf);
				redraw = true;
			}
		} else { /*If deadmans is not pressed. Check for moving cursor*/
			uint16_t x = read_avg_adc(1, 20);
			uint16_t y = read_avg_adc(2, 20);
			
			
			if(fir_cursor_move_delay == 0){
				if(x > x_rest+20){
					fiveinarow_down();
					redraw = true;
				}
				else if(x < x_rest-15){
					fiveinarow_up();
					redraw = true;
				}
				if(y > y_rest+20){
					fiveinarow_left();
					redraw = true;
				}
				else if(y < y_rest-20){
					fiveinarow_right();
					redraw = true;
				}
				fir_cursor_move_delay = MOVE_DELAY_VAL;
			}
			if(redraw){
				draw_five_map();
			}
		}
	}
	_delay_ms(30);
}

void enter_main_mode(char* message){
	program_mode = MAIN_MODE;
	/*Reset variables and force given message in buffer*/
	game_prompted = false;
	redraw = true;
	request_sent = false;
	messages_force(message);
}

void enter_fir_mode(){
	program_mode = FIR_MODE;
	game_over = 0;
	redraw = true;
	if(LED_PORT & (1<<LEDG)){ /*If deadmans is active, disable*/
		uart_send_line("040");
		LED_PORT &= ~(1<<LEDG);
	}
}

void do_normal_actions(){
	/*If a game has been prompted, draw prompt screen if not drawn and poll for accept*/
	if(game_prompted){
		if(redraw){
			clear_LCD();
			set_cursor_pos(16);
			write_lcd_string("Want to play?");
			redraw = false;
		}
		if(DEADMAN_PRESSED){
			enter_fir_mode();
			fiveinarow_setup(true); /*Game starts with player as first turn*/
			_delay_ms(100);			/* Avoid debounce*/
		}
		/*Canceling a game prompt is handled in ISR*/
	} else{
		/*Redraw static text if returning from other screen*/
		if(redraw){
			clear_line(0);
			clear_line(1);
			print_lcd_static();
			redraw = false;
		}
		/*Move along message queue (if message done, handled by module) and write buffer if not read before*/
		messages_move_queue();
		if(!output_buf_read){
			set_cursor_pos(32);
			char* buffer = (char*)messages_get_buffer();
			/*If buffer is empty, write clear line. Otherwise write buffer*/
			write_lcd_string((buffer[0] != '\0') ? buffer :  "                ");
		}
		/*If sensor data was recived in ISR write data to screen*/
		if(sensors_received){
			display_sensors();
			sensors_received = false;
		}
		/*Poll for deadmans switch. If pressed (And not pusnished) send status to car and send move_commands*/
		if(DEADMAN_PRESSED && !punished){
			if(!(LED_PORT & (1<<LEDG))){
				uart_send_line("041");
				uart_send_line("341");
				LED_PORT |= 1<<LEDG;
			}
			send_move_command();
		} else if(LED_PORT & (1<<LEDG)){ /*If button is not pressed AND LED indicating dms is on*/
			uart_send_line("040"); /*Send deadman off to car*/
			uart_send_line("340"); /*Send deadman off to rem2*/
			LED_PORT &= ~(1<<LEDG); /*Disable LED*/
		}
		
		if(GAMEBTN_PRESSED){
			if(DEADMAN_PRESSED){
				if(gamebtn_last != GAMEBTN_PRESSED) uart_send_line("08"); /*Return car to home*/;
			} else if(!request_sent){ /*If button is pressed and no request is waiting, send game request*/
				uart_send_line("33."); /*Game request to rem2. '.' < '0'*/
				messages_force("Invitation sent");
				request_sent = true;
				invite_timeout_hundreths = 0;
			}
			gamebtn_last = 1;
		} else gamebtn_last = 0;
	}
	_delay_ms(50);
}

void send_move_command(){
	if(move_command_hundreths == CMD_DELAY){ /*If no cooldown*/
		int16_t x_diff = x_rest - read_avg_adc(1, 25);
		int16_t y_diff = y_rest - read_avg_adc(2, 25);
		int16_t left = x_diff+y_diff;
		int16_t right = x_diff-y_diff;
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
			move_command_hundreths = 0; /*Reset cooldown*/
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
	/*Toggles heartbeat LED back on every half second to produce a blinking effect
	  when heartbeat is active*/
	if(++heart_hundreths > 50){
		LED_PORT |= 1<<LEDR2;
		heart_hundreths = 0;
	}
	/*Cooldown for sending move commands to vehicle*/
	if(move_command_hundreths < CMD_DELAY) move_command_hundreths++;
	/*Cooldown for honk button to reduce debounce*/
	if(honk_debounce_hundreths > 0) honk_debounce_hundreths--;
	/*Timout for sending game invitations*/
	if(request_sent && invite_timeout_hundreths < TIMEOUT_VAL) invite_timeout_hundreths++;
	else{
		request_sent = false;
		invite_timeout_hundreths = 0;
	}
	if(fir_cursor_move_delay > 0) fir_cursor_move_delay--;
	/*Timer proc for messages module*/
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
				case '0': /*Emergency break triggered*/
					messages_force("Stopp!          ");
					break;
				case '1': /*Sensor data received*/
					strcpy((char*)sensor_buf, (char*)args);
					sensors_received = true;
					break;
				case '2': /*PONG response to a PING*/
					messages_queue("PONG            ");
			}
			break;
		case '2': //To remote
			switch(command){
				case '0': /*Heartbeat received*/
					LED_PORT &= ~(1<<LEDR2);
					break;
				case '1': /*Message received*/
					messages_queue((char*)args);
					break;
				case '2': /*Honk request received*/
					music_play_song_pgm(melodies[args[0]-'0']);
					break;
				case '3': /*Five in a row related command received*/
					process_fir_command(args);
					break;
				case '4': /*Received deadman info from rem2*/
					/*Set lamp to correspond with opponents deadman switch*/
					if(args[0] == '1'){
						SETBIT(LED_PORT, LEDR1);
					} else{
						CLEARBIT(LED_PORT, LEDR1);
						/*Clear punished state when rem2 releases deadmans switch*/
						punished = false;
					}
					break;
			}
			break;
	}
}

void process_fir_command(volatile uint8_t* args){
	if(program_mode == FIR_MODE){ /*If already in a game*/
		if(*args >= 'A'){ /*Protocol dictates that argument over 'A' is "Cancel match" during match*/
			enter_main_mode("Game canceled");
			} else{ /*All other arguments during match are moves*/
			if(!row_player_turn){
				fiveinarow_recive((char*)(args-2));
				music_play_note(NOTE(G_PITCH, 2, SIXTEENTH_NOTE), 100);
				if(fiveinarow_check_win()){
					game_over = 2; /*Mark game as over and lost*/
					music_play_song_pgm(melodies[UNTITLED_SONG]);
				}
				redraw = true;
			}
		}
		} else if(*args < '0'){ /*Protocol dictates that outside of a match < '0' means "Shall we play?"*/
		game_prompted = true;
		redraw = true;
		} else if(*args < 'A'){ /*Else the command received is the first move of the opponent*/
		enter_fir_mode();
		fiveinarow_setup(false); /*Start match where opponent starts first*/
		fiveinarow_recive((char*)(args-2)); /*Receive first move*/
		music_play_note(NOTE(G_PITCH, 2, SIXTEENTH_NOTE), 100);
	}
}

void display_sensors(){
	char* args = (char*)sensor_buf;
	/*Write out contact sensors, PIR-sensor*/
	set_cursor_pos(CR_POS);
	write_lcd_char((args[0] == '0') ? 'R' : '_');
	set_cursor_pos(CL_POS);
	write_lcd_char((args[1] == '0') ? 'L' : '_');
	set_cursor_pos(PIR_POS);
	write_lcd_char((args[3] == '0') ? 'X' : '_');
	/*If button is pressed, honk*/
	if(args[2] == '0') music_play_song_pgm(melodies[HONK_SONG]);
	
	/*Read through arguments and store indices of range data*/
	uint8_t b = 5;
	while(args[++b] != ' ');
	args[b++] = '\0';
	uint8_t k = b;
	while(args[++k] != ' ');
	args[k++] = '\0';
	
	/*Write front value*/
	set_cursor_pos(FRONT_POS);
	if(b == 7 && args[5] == '0'){ //0 is sent for front
		write_lcd_string("!!!");
	} else{
		for (int8_t i = 3-strlen(args+5); i > 0; i--) write_lcd_char(' ');
		write_lcd_string(args+5);
	}
	
	/*Write back value*/
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
		} else if((BTN_PIN & 1<<DEADMANBTN) && (read_adc(0) > z_rest-5)){ /*If deadmans is not pressed and device is flat*/
			z_rest = read_avg_adc(0, 25);
			x_rest = read_avg_adc(1, 25);
			y_rest = read_avg_adc(2, 25);
		}
	}
	
}

//Honk
ISR(INT1_vect){
	if(honk_debounce_hundreths == 0){ /*If no cooldown*/
		if(DEADMAN_PRESSED){
			uart_send_line("020"); /*Send honk to car*/
		} else{
			uart_send_line("32");	/*Send honk to rem2*/
		}
		honk_debounce_hundreths = DEBOUNCE_VAL;	
	}
}

void led_init(){
	LED_DDR |= (1<<LEDG)|(1<<LEDR1)|(1<<LEDR2);
	LED_PORT &= ~((1<<LEDG)|(1<<LEDR1));
	LED_PORT |= 1<<LEDR2;
}

