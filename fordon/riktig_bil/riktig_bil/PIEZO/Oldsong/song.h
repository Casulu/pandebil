#ifndef SONG_H_
#define SONG_H_

#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include "piezo.h"

typedef struct note note;
typedef struct song song;

song *song_empty(int bars_per_min);
void song_add_note(volatile song *s, float tone, char octave, char lenght, char number, bool pause);
void song_start(volatile song *s);
void song_stop();
bool song_playing();
void song_play();

#endif /* SONG_H_ */