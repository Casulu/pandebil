#ifndef DRIVE_REGISTER_H_
#define DRIVE_REGISTER_H_

typedef struct drive_register drive_register;
typedef struct drive_register_entry drive_register_entry;

drive_register *drive_register_empty();
void drive_register_register_entry(drive_register* d, int8_t speed_l, int8_t speed_r, bool motor_forward_lock, bool motor_backward_lock);
void drive_register_add_entry(drive_register* d);
void drive_register_tick(drive_register* d);
bool drive_register_registers(drive_register* d);
void drive_register_backtrack(drive_register* d);

#endif