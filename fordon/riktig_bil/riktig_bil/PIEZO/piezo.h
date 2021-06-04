#ifndef PIEZO_H_
#define PIEZO_H_

/*
 * Frequencies for all middle octave tones
 */
#define C_TONE 261.63f
#define C_S_TONE 277.18f
#define D_TONE 293.66f
#define D_S_TONE 311.13f
#define E_TONE 329.63f
#define F_TONE 349.23f
#define F_S_TONE 369.99f
#define G_TONE 392.00f
#define G_S_TONE 415.30f
#define A_TONE 440.00f
#define A_S_TONE 466.16f
#define B_TONE 493.88f

#define Middel_Octave 4

void piezo_init();
void piezo_play_tone(float tone, uint8_t octave, uint8_t note, uint8_t number);
void piezo_play_tone_continous(float tone, uint8_t octave);
void piezo_test();
void piezo_pause(uint8_t note);
void piezo_set_pace(uint16_t bar_time_ms);
void piezo_stop();
void piezo_delay(uint16_t delay);

#endif /* PIEZO_H_ */