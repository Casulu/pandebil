/*
 * music.h
 *
 * Created: 2021-05-25 15:34:43
 *  Author: Rasmus Lyxell
 */ 


#ifndef MUSIC_H_
#define MUSIC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <avr/pgmspace.h>

#define WHOLE_NOTE 0
#define HALF_NOTE (1<<5)
#define QUARTER_NOTE (2<<5)
#define EIGHTH_NOTE (3<<5)
#define SIXTEENTH_NOTE (4<<5)
#define THIRTYTWOOTH_NOTE (5<<5)
#define SIXTYFOURTH_NOTE (6<<5)
#define ONEHUNDREDTWENTEIGHTH_NOTE (7<<5)

#define A_PITCH -2
#define AS_PITCH -1
#define B_PITCH 0
#define C_PITCH 1
#define CS_PITCH 2
#define D_PITCH 3
#define DS_PITCH 4
#define E_PITCH 5
#define F_PITCH 6
#define FS_PITCH 7
#define G_PITCH 8
#define GS_PITCH 9

#define NOTE(x, y, z) ( ((x)+((y)-1)*12) | (z) )


void music_init();
void music_play_song_pgm(const uint8_t* song);
void music_play_note(uint8_t note, uint8_t bpm);


#endif /* MUSIC_H_ */