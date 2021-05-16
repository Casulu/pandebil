/*
 * adc.c
 *
 * Created: 2021-04-25 12:30:14
 *  Author: Rasmus Lyxell
 */ 

#include "adc.h"

/************************************************************************/
/* Initializes the MCU ADC with 100KHz clock and with ports 0-3 enabled */
/************************************************************************/
void adc_init(){
	DDRC &= ~((1<<1)|(1<<0));					//Enable input on ADC-ports
	ADCSRA = (1<<ADEN)|(1<<ADPS1)|(1<<ADPS2);   //ADC-clock F_CPU/64
	DIDR0 = (1<<ADC5D)|(1<<ADC4D)|(1<<ADC3D);	//Disable unused ADC-ports			
	ADCSRB = 0;									//No auto trigger
}

/************************************************************************/
/* Reads the given ADC port using AVcc as reference voltage and returns */
/* the ADC-value from ADCH and ADCL										*/
/* @param port The port to read from (0-3)								*/
/************************************************************************/
uint16_t read_adc(uint8_t port){
	//Choose port and reference voltage
	ADMUX = port|(1<<REFS0);
	//Start conversion
	ADCSRA |= 1<<ADSC;
	//Wait for completion and return result
	while( ADCSRA & 1<<ADSC);
	return ADC;
}


/************************************************************************/
/* Reads the given ADC port using AVcc as reference voltage and returns */
/* the average ADC-value from ADCH and ADCL	from the given amount		*/
/* of iterations														*/
/* @param port The port to read from (0-3)								*/
/* @param iterations The amount of iterations to take average from		*/
/************************************************************************/
uint16_t read_avg_adc(uint8_t port, uint16_t iterations){
	//Choose port and reference voltage
	ADMUX = port|(1<<REFS0);
	uint32_t sum = 0;
	//Start given amount of conversions and add up results
	for (uint8_t i = 0; i < iterations; i++){
		ADCSRA |= 1<<ADSC;
		while( ADCSRA & 1<<ADSC);
		sum += ADC;
	}
	//Return truncated average
	return (uint16_t)(sum/iterations);
}