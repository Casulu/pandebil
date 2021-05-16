#pragma once

#include <avr/io.h>
#include <inttypes.h>

void adc_init();												/*Initiates adc with ports 0 and 1 enabled and with adc-clock at F_CPU/64*/
uint16_t read_avg_adc(uint8_t port, uint16_t iterations);		/*Reads avg value from given port based on given amount of iterations*/
uint16_t read_adc(uint8_t port);								/*Reads a value from given adc port*/