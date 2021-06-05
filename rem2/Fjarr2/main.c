/*
 * Fjarr2.c
 *
 * Created: 2021-05-10 10:32:57
 * Author : vilo0017
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdbool.h>
#include "display.h"
#include "adc.h"
#include "piezosummer.h"
#include "uart.h"
#include "messages.h"
#include "fiveinarow.h"

#define MESSAGES_LENGHT 5
#define SONGS_LENGHT 11
#define COMMAND_DELAY_SET 10
#define HORN_DELAY_SET 100
#define ROW_REQUEST_DELAY 1000
#define LOCKOUT_DELAY 60000
#define JOYSTICK_DELAY 10

enum menus{Home, Messages, Songs, End, Custom, Row};

//Init global variables
volatile uint8_t command_delay = COMMAND_DELAY_SET;
volatile uint8_t horn_delay = HORN_DELAY_SET;
volatile uint16_t lockout_delay = 0;

volatile uint8_t cursor_pos = 0;
//Joystick
volatile uint8_t vert = 0;
volatile uint8_t horz = 0;

volatile int16_t left = 0;
volatile int16_t right = 0;

volatile uint8_t speed_mod = 1;

volatile bool verticla_center = false;
volatile bool horizontal_center = false;

bool joystick_up = false;
bool joystick_down = false;
bool joystick_left = false;
bool joystick_right = false;

volatile uint8_t joystick_delay = 0;
//Buttons
bool button_joystick = false;
bool button_pressed_joystick = false;
bool button_deadman = false;
bool button_pressed_deadman = false;
bool button_released_deadman = false;
bool button_1 = false;
bool button_pressed_1 = false;
bool button_2 = false;
bool button_pressed_2 = false;
bool button_3 = false;
bool button_pressed_3 = false;
bool button_4 = false;
bool button_pressed_4 = false;

volatile enum menus menu = Home;
volatile bool new_menu = true;

volatile bool car_button = false;
volatile bool row_game = false;
volatile uint16_t row_request = 0;

volatile uint8_t messages_selected = 0;
volatile char messages[MESSAGES_LENGHT][17] = {
	//--------------//
	"Hello world!    ",
	"Can I drive?    ",
	"Enable E-Stop   ",
	"Disable E-Stop  ",
	"Lost Heartbeat  "
};

volatile uint8_t song_selected = 1;
volatile char songs[SONGS_LENGHT][17] = {
	//--------------//
	"Stop            ",
	"Cantina Band    ",
	"Imperial March  ",
	"Soviet Anthem   ",
	"All Star        ",
	"Mii Theme       ",
	"Halo Theme      ",
	"Mario Theme     ",
	"Mario Galaxy    ",
	"Pokemon Center  ",
	"Petalberg       "
};

volatile char custom_message[17] = "                ";
volatile uint8_t custom_index = 0;
volatile uint8_t custom_cursor = 0;

volatile int8_t last_left = 0;
volatile int8_t last_right = 0;

char buffer[49];
char msg[5];

/*
* setup() - Preformes setup for in-out pins, runs other setup functions and other start of program setup.
*
* Returns: Nothing
*/
void setup();
/*
* print_control_info() - Prints info related to controling the car to the display.
*
* Returns: Nothing
*/
void print_control_info();
/*
* set_menu() - Switches the currently selected menu.
* @m: The menu to switch to.
*
* Returns: Nothing
*/
void set_menu(enum menus m);
/*
* read_buttons() - Reads the state of the input pins connected to the button and stores their states in varaibles.
*
* Returns: Nothing
*/
void read_buttons();
/*
* update_left_rigth() - Uppdate the calculated .
*
* Returns: Nothing
*/
void update_left_rigth();
/*
* update_joystick_navigation() - Uppdate the variables related to joystick.
*
* Returns: Nothing
*/
void update_joystick_navigation();
/*
* menu_messages() - Handels the messages menus inputs/outputs.
*
* Returns: Nothing
*/
void menu_messages();
/*
* menu_songs() - Handels the song menus inputs/outputs.
*
* Returns: Nothing
*/
void menu_songs();
/*
* menu_custom() - Handels the custom menus inputs/outputs.
*
* Returns: Nothing
*/
void menu_custom();
/*
* menu_row() - Handels the five in a row menus inputs/outputs.
*
* Returns: Nothing
*/
void menu_row();

int main(void){
	setup();
	
	while (true) {
		//Display message
		if(menu != Row){
			messages_move_queue();
			if(!output_buf_read){
				display_set_cursor_pos(32);
				if(*messages_get_buffer() == '\0')display_clear_line(2);
				else printf("%s", messages_get_buffer());
			}
		}
		
		//Get inputs
		vert = adc_read(4);
		horz = adc_read(3);
		
		read_buttons();
		
		//Send dead mans switch messages
		if(button_pressed_deadman && lockout_delay == 0){
			uart_set_stdout();
			printf("043\n");
			printf("241\n");
			display_set_stdout();
		}
		
		if(button_released_deadman && lockout_delay == 0){//Stop the car
			new_menu = true;
			uart_set_stdout();
			printf("030 0\n");
			printf("042\n");
			printf("240\n");
			display_set_stdout();
		}
		
		if(button_deadman && lockout_delay == 0){//Drive car
			//Horn
			if(button_pressed_1){
				uart_set_stdout();
				printf("020\n");
				display_set_stdout();
			}
			
			//Speed modifier
			if(button_pressed_2){
				speed_mod += 1;
				if(speed_mod > 3)speed_mod = 1;
			}
			
			//Return to home
			if(button_pressed_3){
				uart_set_stdout();
				printf("080\n");
				display_set_stdout();
			}
			
			//Set home
			if(button_pressed_4){
				uart_set_stdout();
				printf("081\n");
				display_set_stdout();
			}
			
			update_left_rigth();
			if(button_pressed_deadman){//Switch display
				if(menu == Row)display_clear();
				else display_clear_top_rows();
				printf("Vert:   Horz:   L:    R:    S:  ");
			}
			print_control_info();
			
			//Don't send same instructions twice or send instructions to frequently
			if(command_delay == 0 &&(left != last_left || right != last_right)){
				uart_set_stdout();
				printf("03%d %d\n", left, right);
				display_set_stdout();
				command_delay = COMMAND_DELAY_SET;
				
				last_left = left;
				last_right = right;
			}
		}else{//Controller mode
			//Horn
			if(button_pressed_1){
				uart_set_stdout();
				printf("220\n");
				display_set_stdout();
			}
			
			//Joystick cursor navigation
			update_joystick_navigation();
			
			//Navigate between menus
			if(menu != Custom && menu != Row){
				if(joystick_right){
					menu++;
					if(menu >= End)menu = 0;
					new_menu = true;
				}
				if(joystick_left){
					if(menu == 0)menu = End;
					menu--;
					new_menu = true;
				}
			}
			
			//Custom message
			if(button_pressed_2){
				if(menu == Custom){
					//Send message
					if(custom_index > 0){
						uart_set_stdout();
						printf("01%s\n", custom_message);
						printf("21%s\n", custom_message);
						display_set_stdout();
					}
					
					set_menu(Home);
				}else{
					//Reset custom message settings
					for(uint8_t i = 0; i < 16; i++)custom_message[i] = ' ';
					custom_message[17] = '\0';
					
					custom_index = 0;
					custom_cursor = 0;
					set_menu(Custom);
				}
			}
			
			//Send/accept 5 in a row invite
			if(button_pressed_3){
				if(menu == Row){
					display_clear();
					set_menu(Home);
					//Cancel game
					row_game = false;
					uart_set_stdout();
					printf("23AA\n");
					display_set_stdout();
					lockout_delay = LOCKOUT_DELAY;
				}else{
					if(row_game){
						set_menu(Row);
					}else{
						if(row_request == 0){//Send invite
							uart_set_stdout();
							printf("23//\n");
							display_set_stdout();
						}else{//Accept invite
							fiveinarow_setup(true);
							row_game = true;
							set_menu(Row);
						}
					}
				}
			}
			
			switch(menu){
				case Home:
					if(new_menu){
						display_clear_top_rows();
						display_set_cursor_pos(18);
						printf("Home Screen!");
						new_menu = false;
					}
				break;
				case Messages: menu_messages(); break;
				case Songs: menu_songs(); break;
				case Custom: menu_custom(); break;
				case Row: menu_row(); break;
				default:
					if(new_menu){
						display_clear_top_rows();
						printf("ERROR: Unknown  menu state");
						new_menu = false;
					}
				break;
			}
		}
	}
}

void setup(){
	
	//Setup spi
	DDRB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2) | (1<<PINB3) | (1<<PINB5);
	//Master, clock rate fck/16
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	
	//Input pins
	DDRB &= ~(1<<PINB0);
	PORTB |= (1<<PINB0);
	
	DDRC &= ~(1<<PINC0)|(1<<PINC1)|(1<<PINC2)|(1<<PINC5);
	PORTC |= (1<<PINC0)|(1<<PINC1)|(1<<PINC2)|(1<<PINC5);
	
	DDRD &= ~(1<<PINB2);
	PORTD |= (1<<PINB2);
	
	//Output pins
	DDRD |= (0b11100000);
	PORTD &= (0b00011111);
	
	//100Hz timer
	OCR0A = 77;
	TCCR0A = (1<<WGM01);
	TCCR0B = 5;
	TIMSK0 = (1<<OCIE0A);
	
	display_setup();
	adc_setup();
	piezosummer_setup();
	uart_init();
	
	fiveinarow_init();
	
	sei();
}

void print_control_info(){
	display_set_cursor_pos(5);
	printf("   ");
	display_set_cursor_pos(5);
	printf("%d", vert);
	
	display_set_cursor_pos(13);
	printf("   ");
	display_set_cursor_pos(13);
	printf("%d", horz);
	
	display_set_cursor_pos(18);
	printf("    ");
	display_set_cursor_pos(18);
	printf("%d", left);
	
	display_set_cursor_pos(24);
	printf("    ");
	display_set_cursor_pos(24);
	printf("%d", right);
	
	display_set_cursor_pos(30);
	printf("    ");
	display_set_cursor_pos(30);
	printf("%d", speed_mod);
}

void set_menu(enum menus m){
	menu = m;
	new_menu = true;
}

void read_buttons(){
	if(!button_joystick && ((PINC&(1<<PINC2)) == 0))button_pressed_joystick = true;
	else button_pressed_joystick = false;
	
	if((PINC&(1<<PINC2)) == 0){
		button_joystick = true;
		joystick_delay = JOYSTICK_DELAY;
	}else{
		if(joystick_delay == 0)button_joystick = false;
	}
	
	if(!button_deadman && ((PINB&(1<<PINB0)) == 0))button_pressed_deadman = true;
	else button_pressed_deadman = false;
	
	if(button_deadman && ((PINB&(1<<PINB0)) != 0))button_released_deadman = true;
	else button_released_deadman = false;
	button_deadman = ((PINB&(1<<PINB0)) == 0);
	
	if(!button_1 && ((PINC&(1<<PINC0)) == 0))button_pressed_1 = true;
	else button_pressed_1 = false;
	button_1 = ((PINC&(1<<PINC0)) == 0);
	
	if(!button_2 && ((PINC&(1<<PINC1)) == 0))button_pressed_2 = true;
	else button_pressed_2 = false;
	button_2 = ((PINC&(1<<PINC1)) == 0);
	
	if(!button_3 && ((PINC&(1<<PINC5)) == 0))button_pressed_3 = true;
	else button_pressed_3 = false;
	button_3 = ((PINC&(1<<PINC5)) == 0);
	
	if(!button_4 && ((PIND&(1<<PIND2)) == 0))button_pressed_4 = true;
	else button_pressed_4 = false;
	button_4 = ((PIND&(1<<PIND2)) == 0);
}

void update_left_rigth(){
	//Clamp vert
	if(vert < 8)vert = 0;
	else if(vert > 248)vert = 255;
	else if(120 < vert && vert < 136)vert = 128;
	
	//Clamp horz
	if(horz < 8)horz = 0;
	else if(horz > 248)horz = 255;
	else if(120 < horz && horz < 136)horz = 128;
	
	//Convert joystick inputs to wheel speeds
	left = vert-horz;
	right = vert+horz-256;
	
	//Clamp left
	if(left < -127)left = -127;
	else if(left > 127)left = 127;
	else if(-2 < left && left < 2)left = 0;
	left /= speed_mod;
	
	//Clamp right
	if(right < -127)right = -127;
	else if(right > 127)right = 127;
	else if(-2 < right && right < 2)right = 0;
	right /= speed_mod;
}

void update_joystick_navigation(){
	joystick_up = false;
	joystick_down = false;
	
	if(64 < vert && vert < 192){
		verticla_center = true;
	}else if(verticla_center){
		verticla_center = false;
		if(vert > 128){//Up
			if(cursor_pos < 16)cursor_pos += 48;
			cursor_pos -= 16;
			joystick_up = true;
			
		}else{//Down
			cursor_pos += 16;
			if(cursor_pos > 47) cursor_pos -= 48;
			joystick_down = true;
			
		}
	}
	
	joystick_left = false;
	joystick_right = false;
	
	if(64 < horz && horz < 192){
		horizontal_center = true;
	}else if(horizontal_center){
		horizontal_center = false;
		if(horz > 128){//Left
			if(cursor_pos == 0 || cursor_pos == 16 || cursor_pos == 32)cursor_pos += 16;
			cursor_pos--;
			joystick_left = true;	
			
		}else{//Right
			cursor_pos++;
			if(cursor_pos == 16 ||cursor_pos == 32 || cursor_pos == 48)cursor_pos -= 16;
		
			joystick_right = true;
		}
	}
}

void menu_messages(){
	if(new_menu){
		display_clear_top_rows();
		printf("Send Message");
		new_menu = false;
	}

	if(joystick_up){
		if(messages_selected == 0)messages_selected += MESSAGES_LENGHT;
		messages_selected--;
	}

	if(joystick_down){
		messages_selected++;
		if(messages_selected >= MESSAGES_LENGHT)messages_selected -= MESSAGES_LENGHT;
	}

	if(button_pressed_joystick){
		uart_set_stdout();
		printf("01%s\n", messages[messages_selected]);
		printf("21%s\n", messages[messages_selected]);
		display_set_stdout();
	}

	display_set_cursor_pos(16);
	printf("%s", messages[messages_selected]);
}

void menu_songs(){
	char song_id;
	if(new_menu){
		display_clear_top_rows();
		printf("Play Song");
		new_menu = false;
	}

	if(joystick_up){
		if(song_selected == 0)song_selected += SONGS_LENGHT;
		song_selected--;
	}

	if(joystick_down){
		song_selected++;
		if(song_selected >= SONGS_LENGHT)song_selected -= SONGS_LENGHT;
	}

	if(button_pressed_joystick){
		//Convert song_selected to song_id
		song_id = song_selected+1;
		if(song_id <= 9)song_id += '0';
		else song_id += 'A'-10;
		
		uart_set_stdout();
		printf("02%c\n", song_id);
		display_set_stdout();
	}

	display_set_cursor_pos(16);
	printf("%s", songs[song_selected]);
}

void menu_custom(){
	if(new_menu){
		display_clear_top_rows();
		new_menu = false;
	}
	display_set_cursor_pos(0);
	for(uint8_t i = 0; i < 16; i++){
		display_write_char(((custom_cursor+i)%26)+65, NULL);
	}
	printf("%s", custom_message);

	if(joystick_left){
		if(custom_cursor == 0)custom_cursor = 26;
		custom_cursor--;
	}

	if(joystick_right){
		custom_cursor++;
		if(custom_cursor >= 26)custom_cursor = 0;
	}

	if(joystick_up){
		custom_cursor += 8;
		if(custom_cursor >= 26)custom_cursor -= 26;
	}

	if(joystick_down){
		if(custom_cursor < 8)custom_cursor += 26;
		custom_cursor -= 8;
	}

	if(button_pressed_joystick && custom_index < 16){
		custom_message[custom_index] = custom_cursor+65;
		custom_index++;
	}
}

void menu_row(){
	//Draw board
	fiveinarow_render(buffer);
	display_set_cursor_pos(0);
	printf("%s", buffer);
	
	//Move cursor
	if(joystick_left)fiveinarow_left();
	else if(joystick_right)fiveinarow_right();

	if(joystick_up)fiveinarow_up();
	else if(joystick_down)fiveinarow_down();

	//Attempt to place piece
	if(button_pressed_joystick){
		fiveinarow_place(msg, '2');
		if(msg[0] != '\0'){//Check that piece was able to be placed
			if(fiveinarow_check_win()){
				row_game = false;
				menu = Home;
				new_menu = true;
				lockout_delay = 0;
				messages_force("    You won!");
			}
			uart_set_stdout();
			printf("%s\n", msg);
			display_set_stdout();
		}
	}
}
// ----- Interrupts -----
//100 Hz timer
ISR(TIMER0_COMPA_vect){
	messages_timerproc();
	if(command_delay > 0)command_delay--;
	if(horn_delay > 0)horn_delay--;
	else piezosummer_disable();
	
	if(row_request > 0)row_request--;
	if(lockout_delay > 0)lockout_delay--;
	if(joystick_delay > 0)joystick_delay--;
}

volatile char line_buffer[40];
volatile uint8_t line_ind = 0;

ISR(USART_RX_vect){
	uint8_t temp;
	line_buffer[line_ind] = UDR0;
	if(line_buffer[line_ind] == '\n'){//Check if the whole message was received
		line_buffer[line_ind] = '\0';
		
		if(line_buffer[0] == '3'){//Topic to_rem2
			switch(line_buffer[1]){
				case '0':
					temp = (~(PORTD & (1<<PIND6)) & (1<<PIND6));
					PORTD &= ~(1<<PIND6);
					PORTD |= temp;
				break;
				case '1':
					messages_queue(line_buffer+2);
				break;
				case '2':
					horn_delay = HORN_DELAY_SET;
					piezosummer_enable();
					piezosummer_set_frequency(128);
				break;
				case '3':
					if(line_buffer[2] >= '0'){
						if(line_buffer[2] < 'A'){
							//Start game/maker move
							menu = Row;
							new_menu = true;
							if(!row_game){
								fiveinarow_setup(false);
							}
							row_game = true;
							
							fiveinarow_recive(line_buffer);
							if(fiveinarow_check_win()){
								row_game = false;
								menu = Home;
								new_menu = true;
								lockout_delay = LOCKOUT_DELAY;
								messages_force("   You  lost!");
							}
						}else{
							//Cancel game
							row_request = 0;
							row_game = false;
							menu = Home;
							new_menu = true;
							lockout_delay = 0;
							messages_force("                ");
						}
					}else{
						//Receive Request
						messages_force("5In A Row Invite");
						row_request = ROW_REQUEST_DELAY;
					}
				break;
				case '4':
					if(line_buffer[2] == '0'){
						PORTD &= ~(1<<PIND7);
						lockout_delay = 0;
					}else{
						PORTD |= (1<<PIND7);
					}
				break;
				case '5':
					PORTD |= (1<<PIND5);
				break;
				case '6':
					PORTD &= ~(1<<PIND5);
				break;
			}
			
		}
		
		if(line_buffer[0] == '1' && line_buffer[1] == '1'){//Topic from_car
			if(line_buffer[4] == '0'){
				if(!car_button){
					horn_delay = HORN_DELAY_SET;
					piezosummer_enable();
					piezosummer_set_frequency(192);
				}
				car_button = true;
			}else{
				car_button = false;
			}
		}
		line_ind = 0;
	}else{
		line_ind++;
	}
}