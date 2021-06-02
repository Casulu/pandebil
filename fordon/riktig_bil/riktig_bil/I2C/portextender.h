#ifndef PORTEXTENDER_H_
#define PORTEXTENDER_H_

#define YELLOW_LED 0
#define GREEN_LED 1
#define COLLISION_DET_R 2
#define COLLISION_DET_L 3
#define BUTTON 4
#define PIR 5

uint8_t portextender_read();
void portextender_write();
void portextender_port_out(uint8_t port);
void portextender_port_in(uint8_t port);
void portextender_set_data(uint8_t data);

#endif /* PORTEXTENDER_H_ */