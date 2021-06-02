#ifndef DRIVE_REGISTER_H_
#define DRIVE_REGISTER_H_

void drive_register_init();
void drive_register_register_entry(int8_t speed_l, int8_t speed_r);
void drive_register_add_entry();
void drive_register_tick();
bool drive_register_registers();
void drive_register_backtrack();

#endif