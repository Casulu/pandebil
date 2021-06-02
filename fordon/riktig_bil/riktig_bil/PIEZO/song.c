#include <stdbool.h>
#include <avr/pgmspace.h>
#include "song.h"

uint16_t timer;
uint8_t current_note;
bool playing;
const song* current_song;

/*
 * Sets a song to be played.
 */
void song_set(const song* set_song)
{
	current_song = set_song;
	
	timer = 0;
	
	current_note = 0;
	
	playing = false;
}

/*
 * Starts song if there is one set.
 */
void song_start()
{
	if (current_song != NULL)
	{
		current_note = 0;
		playing = true;
		timer = 0;
	}
}

/*
 * Stops a playing song.
 */
void song_stop()
{
	playing = false;
	current_song = NULL;
	piezo_stop();
}

/*
 * Returns whether a song is playing or not.
 */
bool song_playing()
{
	return playing;
}

/*
 * Continues song and should be called on a frequency of 1kHz.
 */
void song_play()
{
	if (playing)
	{
		if (timer != 0)
		{
			timer--;
		}
		
		if (current_note == current_song->number_of_notes && timer == 0) //If its the last note and its time has ended. End song.
		{
			song_stop();
		}
		else if (timer == 5)
		{
			piezo_stop();
		}
		else if (timer == 0)
		{
			//Loads note values from program memory.
			float tone = pgm_read_float(&(current_song->notes[current_note].tone));
			uint8_t octave = pgm_read_byte(&(current_song->notes[current_note].octave));
			uint8_t lenght = pgm_read_byte(&(current_song->notes[current_note].length));
			uint8_t number = pgm_read_byte(&(current_song->notes[current_note].number));
			if (tone != 0)
			{
				piezo_play_tone_continous(tone, octave);
			}
			else
			{
				piezo_stop();
			}
			timer = (number) * (current_song->pace_ms/(lenght));
			current_note++;
		}
	}
}