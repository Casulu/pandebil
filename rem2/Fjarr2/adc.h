#ifndef ADC_H_
#define ADC_H_

/*
* Code for handling adc readings .
*
* Author: Victor Löfstedt (c19vlt@cs.umu.se)
*/

#include <avr/io.h>

/*
* adc_setup() - Sets relevant registers in preperations for using the adc.
*
* Returns: Nothing
*/
void adc_setup();

/*
* adc_read() - Reads the input voltage from a adc pin.
*
* Returns: The 8 most significant bits in the read value
*/
uint8_t adc_read(uint8_t pin);

#endif