#include <util/delay.h>
#include <stdbool.h>
#include "i2cmaster.h"

uint16_t srf02_timer;

void srf02_trigger(char srf02)
{
	i2c_start_wait(srf02 + I2C_WRITE);
	i2c_write(0x00);
	i2c_write(0x51);
	i2c_stop();
}

int srf02_read(char srf02)
{
	i2c_start_wait(srf02 + I2C_WRITE);
	i2c_write(0x02);
	i2c_rep_start(srf02 + I2C_READ);
	int data = (i2c_readAck()<<8);
	data |= i2c_readNak();
	i2c_stop();
	return data;
}

int srf02_get_distance(char srf02)
{
	srf02_trigger(srf02);
	_delay_ms(70);
	return srf02_read(srf02);
}

void srf02_timer_tick()
{
	if (srf02_timer > 0)
	{
		srf02_timer--;
	}
}

bool srf02_timer_alarm()
{
	return (srf02_timer == 0);
}

void srf02_timer_reset()
{
	srf02_timer = 700;
}