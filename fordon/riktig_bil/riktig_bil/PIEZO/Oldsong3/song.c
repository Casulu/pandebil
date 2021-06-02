#include <stdbool.h>
#include "song.h"

struct note {
	uint8_t ocr;
	uint8_t tcc;
	uint16_t time;
};

struct note* song;
uint16_t pace_ms;
uint16_t timer;
uint8_t nr_notes;
uint8_t current_note;
bool playing;

void song_empty(uint8_t number_of_notes, uint8_t bars_per_min)
{
	song = calloc(number_of_notes, sizeof(note));
	
	pace_ms = 60000/bars_per_min;
	
	timer = 0;
	
	nr_notes = number_of_notes;
	
	current_note = 0;
	
	playing = false;
}

void song_add_note(float tone, char octave, char lenght, char number, bool pause)
{
	if (pause == false)
	{
		song[current_note].ocr = piezo_calc_ocr(tone, piezo_calc_multiplier(octave));
	}
	else
	{
		song[current_note].ocr = 0;
	}
	song[current_note].tcc = piezo_calc_tcc(octave);
	song[current_note].time = (number) * (pace_ms/(lenght));
	current_note++;
}

void song_start()
{
	if (song != NULL)
	{
		current_note = 0;
		playing = true;
		timer = 0;
	}
}

void song_stop()
{
	playing = false;
	free(song);
	song = NULL;
	piezo_stop();
}

bool song_playing()
{
	return playing;
}

void song_play()
{
	if (playing)
	{
		if (timer != 0)
		{
			timer--;
		}
		
		if (current_note == nr_notes && timer == 0)
		{
			piezo_stop();
			song_stop();
		}
		else if (timer == 1)
		{
			piezo_stop();
		}
		else if (timer == 0)
		{
			if (song[current_note].ocr != 0)
			{
				piezo_play_tone_continous_pre_calc(song[current_note].ocr, song[current_note].tcc);
			}
			else
			{
				piezo_stop();
			}
			timer = song[current_note].time;
			current_note++;
		}
	}
}