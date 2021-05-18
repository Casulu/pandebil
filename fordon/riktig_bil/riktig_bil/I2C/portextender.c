#include "i2cmaster.h"
#include "portextender.h"

char portextender;
char portextender2;

char portextender_read()
{
	i2c_start_wait(0x70 + I2C_READ);
	char data = i2c_readNak();
	i2c_stop();
	return data;
}

void portextender_write(char data)
{
	i2c_start_wait(0x70 + I2C_WRITE);
	portextender = data;
	i2c_write(data);
	i2c_stop();
}

void portextender_port_in(char port)
{
	char data = portextender | (1<<port);
	portextender_write(data);
}

void portextender_port_out(char port)
{
	char data = portextender & ~(1<<port);
	portextender_write(data);
}