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
#include <stdbool.h>

/*Notes in this module are 8 bits in size where the three most significant bits denote the divident of the note where the value of these bits
  is the number to raise 2 to the power of. This means a value of 0 is 1/2^0 and 1 is 1/2^1. The 5 least significant bits denote the pitch of the note
  where a value of 1 is a C2 and each step in value is a step in pitch. A value of 0 for pitch is treated as a rest*/

/*A song is a zero-terminated array of notes where the first item in the array is the bpm of the song*/

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

void music_init();									/*Init the music module. Sets up timer1*/
void music_play_song_pgm(const uint8_t* song);		/*Play a song. Takes a pointer to program memory with correctly formatted array of notes*/
void music_play_note(uint8_t note, uint8_t bpm);	/*Plays a given note in a given bpm*/
bool music_song_is_playing();						/*Returns whether a song is currently playing*/


#endif /* MUSIC_H_ */