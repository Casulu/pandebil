#ifndef PIEZO_H_
#define PIEZO_H_

#define C_TONE 4186.01f
#define C_S_TONE 4434.92f
#define D_TONE 4698.63f
#define D_S_TONE 4978.03f
#define E_TONE 5274.04f
#define F_TONE 5587.65f
#define F_S_TONE 5919.91f
#define G_TONE 6271.93f
#define G_S_TONE 6644.88f
#define A_TONE 7040.00f
#define A_S_TONE 7458.62f
#define B_TONE 7902.13f

#define Middel_Octave 4

void piezo_init();
void piezo_play_tone(float tone, char octave, char note, char number);
void piezo_play_tone_continous(float tone, char octave);
void piezo_test();
void piezo_pause(char note);
void piezo_set_pace(int bar_time_ms);
void piezo_stop();
void piezo_delay(int delay);

#endif /* PIEZO_H_ */