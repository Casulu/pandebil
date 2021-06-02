#include <util/delay.h>
#include <stdbool.h>
#include "i2cmaster.h"

uint16_t srf02_timer;

/*
 * Triggers a supersonic puls from a SRF02.
 */
void srf02_trigger(uint8_t srf02)
{
	i2c_start_wait(srf02 + I2C_WRITE);
	i2c_write(0x00);
	i2c_write(0x51);
	i2c_stop();
}

/*
 * Reads distance data from a SRF02.
 */
uint16_t srf02_read(uint8_t srf02)
{
	i2c_start_wait(srf02 + I2C_WRITE);
	i2c_write(0x02);
	i2c_rep_start(srf02 + I2C_READ);
	uint16_t data = (i2c_readAck()<<8);
	data |= i2c_readNak();
	i2c_stop();
	return data;
}

/*
 * Triggers and reads data from a SRF02. Uses a 70ms delay.
 */
uint16_t srf02_get_distance(uint8_t srf02)
{
	srf02_trigger(srf02);
	_delay_ms(70);
	return srf02_read(srf02);
}

/*
 * Counts down a timer. Should run on a 1kHz timer.
 */
void srf02_timer_tick()
{
	if (srf02_timer > 0)
	{
		srf02_timer--;
	}
}

/*
 * Returns if the timer reached 0.
 */
bool srf02_timer_alarm()
{
	return (srf02_timer == 0);
}

/*
 * Resets timer to 70ms.
 */
void srf02_timer_reset()
{
	srf02_timer = 70;
}