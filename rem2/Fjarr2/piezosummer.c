#include "piezosummer.h"

void piezosummer_setup(){
	TCCR2A = (1<<COM2B0) | (1<<WGM21);
	TCCR2B = (1<<CS21) | (1<<CS20);
	piezosummer_disable();
}

void piezosummer_enable(){
	DDRD |= 0b00001000;
}


void piezosummer_disable(){
	DDRD &= 0b11110111;
}

void piezosummer_set_frequency(uint8_t frequency){
	OCR2A = frequency;	
}

void piezosummer_play_laser(){
	for(uint8_t i = 0; i < 255; i++){
		piezosummer_set_frequency(i);
		_delay_ms(1);
	}
}

void piezosummer_play_swoop(){
	for(uint8_t i = 0; i < 255; i++){
		piezosummer_set_frequency(255-i);
		_delay_ms(1);
	}
}