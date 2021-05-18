#ifndef PORTEXTENDER_H_
#define PORTEXTENDER_H_

#define YELLOW_LED 0
#define GREEN_LED 1
#define COLLISION_DET_R 2
#define COLLISION_DET_L 3
#define BUTTON 4
#define PIR 5

char portextender_read();
void portextender_write(char data);
void portextender_port_out(char port);
void portextender_port_in(char port);

#endif /* PORTEXTENDER_H_ */