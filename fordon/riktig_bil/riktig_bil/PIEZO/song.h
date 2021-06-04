#ifndef SONG_H_
#define SONG_H_

#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include "piezo.h"

#define SONG_CALC_PACE(x) (uint16_t)60000/x

typedef struct{
	float tone;
	uint8_t octave;
	uint8_t length;
	uint8_t number;
} note;

typedef struct{
	uint16_t pace_ms;
	uint8_t number_of_notes;
	const note* notes;
} song;

void song_set(const song* set_song);
void song_start();
void song_stop();
bool song_playing();
void song_play();

#endif /* SONG_H_ */