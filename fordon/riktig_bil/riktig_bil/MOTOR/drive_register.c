#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "drive_register.h"
#include "motor.h"

#define BUFF 500

struct drive_entry
{
	int8_t speed_l;
	int8_t speed_r;
	uint8_t time;
};

struct drive_entry drive_register[BUFF];

bool register_entries;
bool entry_registered;
bool end_backtrack;
uint8_t time;
uint16_t end_entry;
uint16_t current_entry;
uint16_t prev_entry;

/*
 * Initializes the drive register. Can be used as reset.
 */
void drive_register_init()
{
	register_entries = true;
	entry_registered = false;
	time = 0;
	end_entry = 0;
	current_entry = 0;
	prev_entry = 0;
	end_backtrack = true;
}

/*
 * Registers speed thats not standing still.
 */
void drive_register_register_entry(int8_t speed_l, int8_t speed_r)
{
	if (speed_l != 0 || speed_r != 0)
	{
		drive_register[current_entry].speed_l = speed_l;
		drive_register[current_entry].speed_r = speed_r;
		entry_registered = true;
	}
}

/*
 * If there is a registered speed. Register time and add go to the next entry in circular buffer.
 */
void drive_register_add_entry()
{
	if (entry_registered == true)
	{
		drive_register[current_entry].time = time;
		prev_entry = current_entry;
		
		//Move one step in circular buffer. Both for current entry and if needed end entry.
		if (current_entry == BUFF - 1)
		{
			if (end_entry == 0)
			{
				end_entry++;
			}
			current_entry = 0;
		}
		else
		{
			if (end_entry == current_entry + 1)
			{
				if (end_entry == BUFF - 1)
				{
					end_entry = 0;
				}
				else
				{
					end_entry++;
				}
			}
			current_entry++;
		}
		
		//Note that registered speed have been added.
		time = 0;
		entry_registered = false;
		end_backtrack = false;
	}
}

/*
 * Assigns the speed of the current entry to the motors and assigns the time.
 * Also moves to the previous entry in the circular buffer.
 */
void drive_register_pop()
{
	motors_set_speed(-drive_register[current_entry].speed_l, -drive_register[current_entry].speed_r);
	time = drive_register[current_entry].time;
	if (current_entry == 0)
	{
		current_entry = BUFF - 1;
	}
	else
	{
		current_entry--;
	}
}

/*
 * Function that does time sensitive things. It counts in millisecond the time between motor input changes.
 * Also count how long each input should stay when backtracking. Should run on a 1kHz timer.
 */
void drive_register_tick()
{
	if (register_entries == true)
	{
		if (entry_registered == true)
		{
			time++;
			if (time == 0xff)
			{
				drive_register_add_entry();
				drive_register_register_entry(drive_register[prev_entry].speed_l, drive_register[prev_entry].speed_r);
			}
		}
		else
		{
			time = 0;
		}
	}
	else
	{
		if (time != 0)
		{
			time--;
		}
		
		if (time == 0)
		{
			if (end_backtrack == false)
			{
				if (current_entry == end_entry)
				{
					end_backtrack = true;
				}
				drive_register_pop();
			}
			else
			{
				drive_register_init();
				motors_set_speed(0, 0);
				motor_set_lock(false);
				motor_forward_set_lock(false);
				motor_backward_set_lock(false);
			}
		}
	}
}

/*
 * Returns if the register registers.
 */
bool drive_register_registers()
{
	return register_entries;
}

/*
 * Sets drive register to not register and backtrack.
 */
void drive_register_backtrack()
{
	register_entries = false;
	drive_register_add_entry(); //Adds the last registered speeds
}