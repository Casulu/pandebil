#include "adc.h"

void adc_setup(){
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);    // 128 prescale for 8Mhz / Enable the ADC 
}

uint8_t adc_read(uint8_t pin){
	int ADCval;

	//Select adc pin
	ADMUX = pin;
	//Select AVcc as reference
	ADMUX |= (1 << REFS0);    

	//Start the adc
	ADCSRA |= (1 << ADSC);    
	// Wait for the ADC to finish
	while(ADCSRA & (1 << ADSC));      

	//Read value from adc
	ADCval = ADCL;
	ADCval = (ADCH << 8) + ADCval;

	return (uint8_t) (ADCval >> 2);
}