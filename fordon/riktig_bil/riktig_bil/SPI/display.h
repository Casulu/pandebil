#ifndef DISPLAY_H_
#define DISPLAY_H_

#define RS_DISP 1
#define SS_DISP 2

void display_clear();
void display_position_cursor(int position);
void display_move_cursor(int steps);
int display_put_char(char c, FILE *stream);
void display_char(char c);
void display_init(void);
void display_port_init(void);

#endif