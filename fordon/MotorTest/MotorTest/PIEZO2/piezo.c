#include <math.h>
#include <avr/io.h>
#include <util/delay.h>
#include "piezo.h"

char piezo_calc_ocr(float tone, char multiplier);
int pace_ms;

void piezo_init()
{
	TCCR1A = (1<<COM0A0);
	TCCR1B |= (1<<WGM12);
	DDRB &=~ (1<<DDB1);
}

void piezo_set_pace(int bars_per_min)
{
	pace_ms = 60000/bars_per_min;
}

void piezo_play_tone(float tone, char octave, char note, char number)
{
	piezo_play_tone_continous(tone, octave);
	piezo_delay(number * (pace_ms/note) - 1);
	piezo_stop();
	piezo_delay(1);
}

void piezo_pause(char note)
{
	piezo_stop(pace_ms/note);
	piezo_delay(pace_ms/note);
}

void piezo_play_tone_continous(float tone, char octave)
{
	int divider;
	switch(octave)
	{
		case 8:
		TCCR1B |= (1<<CS10);
		TCCR1B &=~ ((1<<CS12) | (1<<CS11));
		divider = 0;
		break;
		
		case 7:
		TCCR1B |= (1<<CS10);
		TCCR1B &=~ ((1<<CS12) | (1<<CS11));
		divider = 2;
		break;
		
		case 6:
		TCCR1B |= (1<<CS10);
		TCCR1B &=~ ((1<<CS12) | (1<<CS11));
		divider = 4;
		break;
		
		case 5:
		TCCR1B |= (1<<CS10);
		TCCR1B &=~ ((1<<CS12) | (1<<CS11));
		divider = 8;
		break;
		
		case 4:
		TCCR1B |= (1<<CS10);
		TCCR1B &=~ ((1<<CS12) | (1<<CS11));
		divider = 16;
		break;
		
		case 3:
		TCCR1B |= (1<<CS10);
		TCCR1B &=~ ((1<<CS12) | (1<<CS11));
		divider = 32;
		break;
		
		case 2:
		TCCR1B |= (1<<CS10);
		TCCR1B &=~ ((1<<CS12) | (1<<CS11));
		divider = 64;
		break;
		
		case 1:
		TCCR1B |= (1<<CS11);
		TCCR1B &=~ ((1<<CS12) | (1<<CS10));
		divider = 16;
		break;
		
		case 0:
		TCCR1B |= (1<<CS11);
		TCCR1B &=~ ((1<<CS12) | (1<<CS10));
		divider = 32;
		break;
		
		default:
		return;
	}
	
	OCR1A = 15288;//piezo_calc_ocr(tone, divider);
	DDRB |= (1<<DDB1);
}

void piezo_delay(int delay)
{
	for (int i = 0; i < delay; i++)
	{
		if (i == 0)
		{
			_delay_ms(0.9977);
		}
		else
		{
			_delay_ms(0.9987);
		}
	}
}

void piezo_stop()
{
	DDRB &=~ (1<<DDB1);
}

char piezo_calc_ocr(float tone, char divider)
{
	return (char)round((double)((F_CPU/(2*(tone/divider)))-1));
}

void piezo_test()
{
	piezo_play_tone(C_TONE, Middel_Octave - 4, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(D_TONE, Middel_Octave - 4, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(E_TONE, Middel_Octave - 4, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(F_TONE, Middel_Octave - 4, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(G_TONE, Middel_Octave - 4, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(A_TONE, Middel_Octave - 4, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(B_TONE, Middel_Octave - 4, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(C_TONE, Middel_Octave - 3, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(D_TONE, Middel_Octave - 3, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(E_TONE, Middel_Octave - 3, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(F_TONE, Middel_Octave - 3, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(G_TONE, Middel_Octave - 3, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(A_TONE, Middel_Octave - 3, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(B_TONE, Middel_Octave - 3, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(C_TONE, Middel_Octave - 2, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(D_TONE, Middel_Octave - 2, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(E_TONE, Middel_Octave - 2, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(F_TONE, Middel_Octave - 2, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(G_TONE, Middel_Octave - 2, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(A_TONE, Middel_Octave - 2, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(B_TONE, Middel_Octave - 2, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(C_TONE, Middel_Octave - 1, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(D_TONE, Middel_Octave - 1, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(E_TONE, Middel_Octave - 1, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(F_TONE, Middel_Octave - 1, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(G_TONE, Middel_Octave - 1, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(A_TONE, Middel_Octave - 1, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(B_TONE, Middel_Octave - 1, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(C_TONE, Middel_Octave, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(D_TONE, Middel_Octave, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(E_TONE, Middel_Octave, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(F_TONE, Middel_Octave, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(G_TONE, Middel_Octave, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(A_TONE, Middel_Octave, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(B_TONE, Middel_Octave, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(C_TONE, Middel_Octave + 1, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(D_TONE, Middel_Octave + 1, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(E_TONE, Middel_Octave + 1, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(F_TONE, Middel_Octave + 1, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(G_TONE, Middel_Octave + 1, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(A_TONE, Middel_Octave + 1, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(B_TONE, Middel_Octave + 1, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(C_TONE, Middel_Octave + 2, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(D_TONE, Middel_Octave + 2, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(E_TONE, Middel_Octave + 2, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(F_TONE, Middel_Octave + 2, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(G_TONE, Middel_Octave + 2, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(A_TONE, Middel_Octave + 2, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(B_TONE, Middel_Octave + 2, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(C_TONE, Middel_Octave + 3, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(D_TONE, Middel_Octave + 3, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(E_TONE, Middel_Octave + 3, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(F_TONE, Middel_Octave + 3, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(G_TONE, Middel_Octave + 3, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(A_TONE, Middel_Octave + 3, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(B_TONE, Middel_Octave + 3, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(C_TONE, Middel_Octave + 4, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(D_TONE, Middel_Octave + 4, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(E_TONE, Middel_Octave + 4, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(F_TONE, Middel_Octave + 4, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(G_TONE, Middel_Octave + 4, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(A_TONE, Middel_Octave + 4, 100, 1);
	
	_delay_ms(100);
	
	piezo_play_tone(B_TONE, Middel_Octave + 4, 100, 1);
}