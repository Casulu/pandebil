#include <stdbool.h>
#include "i2cmaster.h"
#include "portextender.h"

uint8_t portextender_data;
bool portextender_new_data = false;

/*
 * Reads from portextender.
 */
uint8_t portextender_read()
{
	i2c_start_wait(0x70 + I2C_READ);
	uint8_t data = i2c_readNak();
	i2c_stop();
	return data;
}

/*
 * Writes set data to the portextender.
 */
void portextender_write()
{
	if (portextender_new_data == true)
	{
		i2c_start_wait(0x70 + I2C_WRITE);
		i2c_write(portextender_data);
		i2c_stop();
		portextender_new_data = false;
	}
}

/*
 * Set a port as input port in write data.
 */
void portextender_port_in(uint8_t port)
{
	portextender_data = portextender_data | (1<<port);
	portextender_new_data = true;
}

/*
 * Set a port as output port in write data.
 */
void portextender_port_out(uint8_t port)
{
	portextender_data = portextender_data & ~(1<<port);
	portextender_new_data = true;
}

/*
 * Sets the full byte of write data.
 */
void portextender_set_data(uint8_t data)
{
	portextender_data = data;
	portextender_new_data = true;
}