#ifndef DISPLAY_H_
#define DISPLAY_H_

#define RS_DISP 1
#define SS_DISP 2

void display_clear();
void display_position_cursor(uint8_t position);
void display_move_cursor(uint8_t steps);
int display_put_char(char c, FILE *stream);
void display_char(char c);
void display_init(void);
void display_port_init(void);
void display_buff(void);
void display_add_to_buff(const char* string, uint8_t length, uint8_t position);

#endif