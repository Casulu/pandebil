#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "drive_register.h"
#include "motor.h"

struct drive_register
{
	struct drive_register_entry* top;
	struct drive_register_entry* bottom;
	struct drive_register_entry* temp;
	bool register_entries;
	uint8_t time;
};

struct drive_entry
{
	int8_t speed_l;
	int8_t speed_r;
	bool motor_forward_lock;
	bool motor_backward_lock;
	uint16_t time;
};

drive_register *drive_register_empty()
{
	drive_register *d = calloc(1, sizeof(*d));
	
	d->time = 0;
	d->bottom = NULL;
	d->top = d->bottom;
	d->register_entries = true;
	
	return d;
}

void drive_register_register_entry(drive_register* d, int8_t speed_l, int8_t speed_r, bool motor_forward_lock, bool motor_backward_lock)
{
	d->temp = calloc(1, sizeof(struct drive_register_entry));
	d->temp->speed_l = speed_l;
	d->temp->speed_r = speed_r;
	d->temp->motor_forward_lock = motor_forward_lock;
	d->temp->motor_backward_lock = motor_backward_lock;
	d->temp->prev = d->top;
}

void drive_register_add_entry(drive_register* d)
{
	if (d->temp != NULL)
	{
		drive_register_entry* de = d->temp;
		de->time = d->time;
		d->time = 0;
		d->top = de;
		d->temp = NULL;
	}
}

void drive_register_pop(drive_register* d)
{
	motor_forward_set_lock(d->top->motor_forward_lock);
	motor_backward_set_lock(d->top->motor_backward_lock);
	motors_set_speed(-d->top->speed_l, -d->top->speed_r);
	struct drive_register_entry* de = d->top;
	d->top = de->prev;
	free(de);
}

void drive_register_tick(drive_register* d)
{
	if (d->register_entries == true)
	{
		if (d->top != NULL)
		{
			d->time++;
			if (d->time == 0xffff)
			{
				drive_register_add_entry(d);
				drive_register_register_entry(d, d->top->speed_l, d->top->speed_r, d->top->motor_forward_lock, d->top->motor_backward_lock);
			}
		}
		else
		{
			d->time = 0;
		}
	}
	else
	{
		if (d->time == 0)
		{
			if (d->top != NULL)
			{
				d->time = d->top->time;
				drive_register_pop(d);
			}
			else
			{
				d->register_entries = true;
				motors_set_speed(0, 0);
				motor_set_lock(false);
				motor_forward_set_lock(false);
				motor_backward_set_lock(false);
			}
		}
		else
		{
			d->time--;
		}
	}
}

bool drive_register_registers(drive_register* d)
{
	return d->register_entries;
}

void drive_register_backtrack(drive_register* d)
{
	d->register_entries = false;
}