/*
 * music.c
 *
 * Created: 2021-05-25 15:12:22
 *  Author: Rasmus Lyxell
 */ 
#include "music.h"
#include <stddef.h>

/*Note system inspired by Filip Henningsson from c18*/
#define NOTE_DURATION_MASK      (0b11100000)
#define NOTE_PITCH_MASK         (~NOTE_DURATION_MASK)

#define GET_DURATION_DIV(x)     (1<<(x>>5))
#define GET_PITCH_EXP(x)        ((x & NOTE_PITCH_MASK) - 1)

// Defines for pitch values
#define LOWC_OCR (F_CPU/520)    // Lowest possible note
#define ROOT12_2 1.05946309     // Multiply by twelfth root of 2 to get next note
								// See https://en.wikipedia.org/wiki/Equal_temperament

static volatile uint16_t curr_note_time = 0;
static volatile uint8_t note_ind;  
static volatile uint8_t curr_bpm;
static const volatile uint8_t* curr_song = NULL;


void music_init(){
	DDRB |= 1<<OCPIN;
	TCCR1A = 1<<COM1A0;
	TCCR1B = 1<<WGM12;
}

void music_play_song_pgm(const uint8_t* song){
	curr_song = song;
	curr_bpm = pgm_read_byte(&(curr_song[0])); /*Read bpm from first byte*/
	note_ind = 1;							   /*Start at the first actual note*/
	music_play_note(pgm_read_byte(&(curr_song[note_ind])), curr_bpm);	/*Play the first note to start the chain*/
}

void music_play_note(uint8_t note, uint8_t bpm){
	TIMSK1 |= 1<<OCIE1A;
	TCCR1B |= 1<<CS10;
	if(GET_PITCH_EXP(note) == -1) TCCR1A &= ~(1<<COM1A0); /*If pitch value is zero, note is a rest. Turn buzzer off*/
	else{ /*Start buzzer and set frequency*/
		TCCR1A |= 1<<COM1A0;
		OCR1A = LOWC_OCR/pow(ROOT12_2,GET_PITCH_EXP(note));		/*See https://en.wikipedia.org/wiki/Equal_temperament*/
	}
	// cycles for current note = frequency/(time of one bar)/(note length divisor)
	curr_note_time = ((F_CPU/(OCR1A + 1))/(bpm/(240.0)))/GET_DURATION_DIV(note);
}

bool music_song_is_playing(){
	return curr_song != NULL;
}

ISR(TIMER1_COMPA_vect){
	if(--curr_note_time == 0){ /*If note is over*/
		uint8_t note;
		/*Read next note*/
		if(curr_song != NULL && (note = pgm_read_byte(&(curr_song[++note_ind]))) != 0){ /*If a song is playing and the next note is not the end of the song*/
			music_play_note(note, curr_bpm);
		} else{ /*Else song is over or the single note played is over. Shut buzzer off and clear song*/
			TIMSK1 = 0;
			TCCR1B &= ~(1<<CS10);
			curr_song = NULL;
		}
	}
}