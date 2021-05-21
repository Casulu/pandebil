#include <stdbool.h>
#include "song.h"

struct note {
	struct note *next;
	float tone;
	char octave;
	char lenght;
	char number;
	bool pause;
};

struct song {
	struct note *start;
	struct note *current_note;
	struct note *end_note;
	int pace_ms;
	int timer;
};

volatile song *current_song = NULL;

song *song_empty(int bars_per_min)
{
	song *s = calloc(1, sizeof(*s));
	
	s->pace_ms = 60000/bars_per_min;
	
	s->timer = 0;
	
	s->start = calloc(1, sizeof(struct note));
	
	s->start->next = NULL;
	
	s->end_note = s->start;
	
	return s;
}

void song_add_note(volatile song *s, float tone, char octave, char lenght, char number, bool pause)
{
	note *n = calloc(1, sizeof(struct note));
	n->next = NULL;
	n->tone = tone;
	n->octave = octave;
	n->lenght = lenght;
	n->number = number;
	n->pause = pause;
	s->end_note->next = n;
	s->end_note = n;
	s->current_note = n;
}

void song_start(volatile song *s)
{
	current_song = s;
	s->timer = 0;
	s->current_note = s->start;
}

void song_stop()
{
	current_song = NULL;
	piezo_stop();
}

bool song_playing()
{
	return current_song != NULL;
}

void song_play()
{
	if (current_song != NULL)
	{
		if (current_song->timer != 0)
		{
			current_song->timer--;
		}
		
		if (current_song->current_note->next == NULL && current_song->timer == 0)
		{
			piezo_stop();
			current_song = NULL;
		}
		else if (current_song->timer == 1)
		{
			piezo_stop();
		}
		else if (current_song->timer == 0)
		{
			current_song->current_note = current_song->current_note->next;
			if (current_song->current_note->pause == false)
			{
				piezo_play_tone_continous(current_song->current_note->tone, current_song->current_note->octave);
			}
			else
			{
				piezo_stop();
			}
			current_song->timer = current_song->current_note->number * (current_song->pace_ms/current_song->current_note->lenght);
		}
	}
}