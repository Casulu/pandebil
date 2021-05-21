#ifndef SONG_H_
#define SONG_H_

#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include "piezo.h"

typedef struct note note;

void song_empty(uint8_t number_of_notes, uint8_t bars_per_min);
void song_add_note(float tone, char octave, char lenght, char number, bool pause);
void song_start();
void song_stop();
bool song_playing();
void song_play();

#endif /* SONG_H_ */