#include <math.h>
#include <avr/io.h>
#include <util/delay.h>
#include "piezo.h"

char piezo_calc_ocr(float tone, char multiplier);
int pace_ms;

/*
 * Initiates the piezo buzzer with OC2B.
 */
void piezo_init()
{
	TCCR1A = (1<<COM0A0);
	TCCR1B |= (1<<WGM12);
	DDRB &=~ (1<<DDB1);
}

/*
 * Sets the pace, must be set to play notes or delays in different lengths.
 * Takes number of bars per minute to calculate the length of a whole note in ms.
 */
void piezo_set_pace(int bars_per_min)
{
	pace_ms = 60000/bars_per_min;
}

/*
 * Playes a tone for a while. The note input tells the length.
 * Ex. if the note parameter is 4 it will be playing in the length of a quarter note.
 * The octave input represent the octave.
 * The tone input represents the tone and takes frequency for the middle octave.
 * The number input is to be able to play a tone for ex. three quarter notes without interrupt.
 */
void piezo_play_tone(float tone, char octave, char note, char number)
{
	piezo_play_tone_continous(tone, octave);
	piezo_delay(number * (pace_ms/note) - 1);
	piezo_stop();
	piezo_delay(1);
}

/*
 * Sets a silent delay. The note input tells the length.
 * Ex. if the note parameter is 4 it will be silent in the length of a quarter note.
 */
void piezo_pause(char note)
{
	piezo_stop(pace_ms/note);
	piezo_delay(pace_ms/note);
}

/*
 * Starts playing a continous tone.
 * The octave input represent the octave.
 * The tone input represents the tone and takes frequency for the middle octave.
 */
void piezo_play_tone_continous(float tone, char octave)
{
	/* Sets TCCR2B and multiplier to get the right tone to the correct octave. */
	int multiplier;
	switch(octave)
	{
		case 8:
		TCCR1B |= (1<<CS11);
		TCCR1B &=~ ((1<<CS12) | (1<<CS10));
		multiplier = 1;
		break;
		
		case 7:
		TCCR1B |= (1<<CS11);
		TCCR1B &=~ ((1<<CS12) | (1<<CS10));
		multiplier = 2;
		break;
		
		case 6:
		TCCR1B |= (1<<CS11);
		TCCR1B &=~ ((1<<CS12) | (1<<CS10));
		multiplier = 4;
		break;
		
		case 5:
		TCCR1B |= (1<<CS11) | (1<<CS10);
		TCCR1B &=~ (1<<CS12);
		multiplier = 1;
		break;
		
		case 4:
		TCCR1B |= (1<<CS11) | (1<<CS10);
		TCCR1B &=~ (1<<CS12);
		multiplier = 2;
		break;
		
		case 3:
		TCCR1B |= (1<<CS12);
		TCCR1B &=~ ((1<<CS11) | (1<<CS10));
		multiplier = 1;
		break;
		
		case 2:
		TCCR1B |= (1<<CS12);
		TCCR1B &=~ ((1<<CS11) | (1<<CS10));
		multiplier = 2;
		break;
		
		case 1:
		TCCR1B |= (1<<CS12) | (1<<CS10);
		TCCR1B &=~ (1<<CS11);
		multiplier = 1;
		break;
		
		case 0:
		TCCR1B |= (1<<CS12) | (1<<CS10);
		TCCR1B &=~ (1<<CS11);
		multiplier = 2;
		break;
		
		default:
		return;
	}
	
	OCR1A = piezo_calc_ocr(tone, multiplier);
	TCNT1 = 0;
	DDRB |= (1<<DDB1);
}

/*
 * Sets a delay for the piezo.
 */
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

/*
 * Silences the piezo.
 */
void piezo_stop()
{
	DDRB &=~ (1<<DDB1);
}

/*
 * Calculates the OCR value to get correct frequency.
 */
char piezo_calc_ocr(float tone, char multiplier)
{
	return (char)round((double)((F_CPU/(2*128*tone))*multiplier-1));
}

/*
 * Playes throuh the tunes possible for the piezo.
 */
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