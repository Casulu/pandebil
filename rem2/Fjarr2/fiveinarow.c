#include "fiveinarow.h"

volatile char row_markers[15][15];
volatile uint8_t row_cursor_x = 0;
volatile uint8_t row_cursor_y = 0;
volatile uint8_t row_last_x = 0;
volatile uint8_t row_last_y = 0;
volatile uint8_t row_display_pos = 0;
volatile bool row_turn = true;

static const uint8_t x_curs_char[8] PROGMEM = {0x11, 0x15, 0x1F, 0x0E, 0x1F, 0x15, 0x11, 0x00};
static const uint8_t o_curs_char[8] PROGMEM = {0x0E, 0x15, 0x15, 0x1F, 0x15, 0x15, 0x0E, 0x00};
static const uint8_t under_char[8] PROGMEM = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F};
static const uint8_t x_under_char[8] PROGMEM = {0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11, 0x1F};
static const uint8_t o_under_char[8] PROGMEM = {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E, 0x1F};

void fiveinarow_init(){
	display_set_character_pgm(X_CURS_CODE, x_curs_char);
	display_set_character_pgm(O_CURS_CODE, o_curs_char);
	display_set_character_pgm(UNDER_CODE, under_char);
	display_set_character_pgm(X_UNDER_CODE, x_under_char);
	display_set_character_pgm(O_UNDER_CODE, o_under_char);
}

void fiveinarow_setup(bool turn){
	row_cursor_x = 7;
	row_cursor_y = 7;
	row_display_pos = 6;
	row_turn = turn;
	
	for(uint8_t q = 0; q < 15; q++){
		for(uint8_t i = 0; i < 15; i++){
			row_markers[i][q] = ' ';
		}
	}
}
void fiveinarow_left(){
	if(row_cursor_x == 0)row_cursor_x = 1;
	row_cursor_x--;
}
void fiveinarow_right(){
	row_cursor_x++;
	if(row_cursor_x > 14)row_cursor_x = 14;
}
void fiveinarow_up(){
	if(row_cursor_y == 0)row_cursor_y = 1;
	row_cursor_y--;
	
	if(row_cursor_y < row_display_pos)row_display_pos = row_cursor_y;
}
void fiveinarow_down(){
	row_cursor_y++;
	if(row_cursor_y > 14)row_cursor_y = 14;
	
	if(row_cursor_y > row_display_pos+2)row_display_pos = row_cursor_y-2;
}
void fiveinarow_place(char* message, char topic){
	if(row_turn && row_markers[row_cursor_x][row_cursor_y] == ' '){
		row_last_x = row_cursor_x;
		row_last_y = row_cursor_y;
		row_turn = false;
		
		row_markers[row_cursor_x][row_cursor_y] = 'X';
		message[0] = topic;
		message[1] = '3';
		message[2] = row_cursor_x+48;
		message[3] = row_cursor_y+48;
		message[4] = '\0';
	}else{
		message[0] = '\0';
	}
	
}
void fiveinarow_recive(char* message){
	row_turn = true;

	row_last_x = message[2]-48;
	row_last_y = message[3]-48;
	
	row_cursor_x = row_last_x;
	row_cursor_y = row_last_y;
	
	row_markers[row_last_x][row_last_y] = 'O';
	if(row_last_y < row_display_pos){
		row_display_pos = row_last_y;
	}
	if(row_last_y > row_display_pos+2){
		row_display_pos = row_last_y-2;
	}
}
void fiveinarow_render(char* row_buf){
	uint8_t index = 0;
	for(uint8_t q = row_display_pos; q < row_display_pos+3; q++){
		if(q < 9){
			row_buf[index] = q+49;
			}else{
			row_buf[index] = q+56;
		}
		index++;
		    
		for(uint8_t i = 0; i < 15; i++){
			row_buf[index] = row_markers[i][q];
			index++;
		}
	}
	//Custom characters
	uint8_t cursor_string_pos = ((row_cursor_y-row_display_pos)*16)+row_cursor_x+1;
	if(row_turn){
		switch(row_buf[cursor_string_pos]){
			case 'X':
				row_buf[cursor_string_pos] = X_CURS_CODE;
			break;
			case 'O':
				row_buf[cursor_string_pos] = O_CURS_CODE;
			break;
			default:
				row_buf[cursor_string_pos] = '*';
			break;
		}
		}else{
		switch(row_buf[cursor_string_pos]){
			case 'X':
				row_buf[cursor_string_pos] = X_UNDER_CODE;
			break;
			case 'O':
				row_buf[cursor_string_pos] = O_UNDER_CODE;
			break;
			default:
				row_buf[cursor_string_pos] = UNDER_CODE;
			break;
		}
	}
	    
	row_buf[48] = '\0';
}

bool fiveinarow_check_win(){
	char last_player = row_markers[row_last_x][row_last_y];
	uint8_t row_lenght;
	uint8_t test_x;
	uint8_t test_y;
	
	//Horizontal
	row_lenght = 1;
	test_x = row_last_x;
	test_y = row_last_y;
	while(fiveinarow_is_char(--test_x, test_y, last_player))row_lenght++;
	test_x = row_last_x;	
	while(fiveinarow_is_char(++test_x, test_y, last_player))row_lenght++;
	if(row_lenght >= 5)return true;
	
	//Vertical
	row_lenght = 1;
	test_x = row_last_x;
	test_y = row_last_y;
	while(fiveinarow_is_char(test_x, --test_y, last_player))row_lenght++;
	test_y = row_last_y;
	while(fiveinarow_is_char(test_x, ++test_y, last_player))row_lenght++;
	if(row_lenght >= 5)return true;
	
	//Diagonal forward
	row_lenght = 1;
	test_x = row_last_x;
	test_y = row_last_y;
	while(fiveinarow_is_char(--test_x, --test_y, last_player))row_lenght++;
	test_x = row_last_x;
	test_y = row_last_y;
	while(fiveinarow_is_char(++test_x, ++test_y, last_player))row_lenght++;
	if(row_lenght >= 5)return true;
	
	//Diagonal backwards
	row_lenght = 1;
	test_x = row_last_x;
	test_y = row_last_y;
	while(fiveinarow_is_char(--test_x, ++test_y, last_player))row_lenght++;
	test_x = row_last_x;
	test_y = row_last_y;
	while(fiveinarow_is_char(++test_x, --test_y, last_player))row_lenght++;
	if(row_lenght >= 5)return true;
	
	return false;
}

//Internal helper functions
bool fiveinarow_is_char(uint8_t x, uint8_t y, char c){
	if(x > 14)return false;
	if(y > 14)return false;
	if(row_markers[x][y] == c)return true;
	return false;
}