#include "song.h"
#include "piezo.h"
#include <avr/pgmspace.h>

/************************************************************************/
/* Super Mario Melodi start                                             */
/************************************************************************/
#define MARIO_NR_NOTES 12

const note mario_notes[MARIO_NR_NOTES] PROGMEM = {
	{E_TONE, Middel_Octave + 1, 8, 1},
	{E_TONE, Middel_Octave + 1, 8, 1},
	{0, 0, 8, 1},
	{E_TONE, Middel_Octave + 1, 8, 1},
	{0, 0, 8, 1},
	{C_TONE, Middel_Octave + 1, 8, 1},
	{E_TONE, Middel_Octave + 1, 8, 1},
	{0, 0, 8, 1},
	{G_TONE, Middel_Octave + 1, 8, 1},
	{0, 0, 4, 1},
	{0, 0, 8, 1},
	{G_TONE, Middel_Octave, 8, 1}
};

const song mario_song = {
	SONG_CALC_PACE(50),
	MARIO_NR_NOTES,
	mario_notes
};

const song* get_mario_song()
{
	return &mario_song;
}

/************************************************************************/
/* Super Mario Melodi end                                               */
/************************************************************************/

/************************************************************************/
/* Mii start                                                            */
/************************************************************************/
#define MII_NR_NOTES 29

const note mii_notes[MII_NR_NOTES] PROGMEM = {
	{F_S_TONE, Middel_Octave, 4, 1},
	{A_TONE, Middel_Octave, 8, 1},
	{C_S_TONE, Middel_Octave + 1, 8, 1},
	{0, 0, 8, 1},
	{A_TONE, Middel_Octave, 8, 1},
	{0, 0, 8, 1},
	{F_S_TONE, Middel_Octave, 8, 1},
	{D_TONE, Middel_Octave, 32, 3},
	{0, 0, 32, 1},
	{D_TONE, Middel_Octave, 32, 3},
	{0, 0, 32, 1},
	{D_TONE, Middel_Octave, 32, 3},
	{0, 0, 32, 1},
	{0, 0, 8, 1},
	{0, 0, 8, 1},
	{0, 0, 4, 1},
	{0, 0, 8, 1},
	{C_S_TONE, Middel_Octave, 8, 1},
	{D_TONE, Middel_Octave, 8, 1,},
	{F_S_TONE, Middel_Octave, 8, 1},
	{A_TONE, Middel_Octave, 8, 1},
	{C_S_TONE, Middel_Octave + 1, 8, 1},
	{0, 0, 8, 1},
	{A_TONE, Middel_Octave, 8, 1},
	{0, 0, 8, 1},
	{F_S_TONE, Middel_Octave, 8, 1},
	{E_TONE, Middel_Octave + 1, 8, 3},
	{D_S_TONE, Middel_Octave + 1, 8, 1},
	{D_TONE, Middel_Octave + 1, 4, 1}
};

const song mii_song = {
	SONG_CALC_PACE(28),
	MII_NR_NOTES,
	mii_notes
};

const song* get_mii_song()
{
	return &mii_song;
}

/************************************************************************/
/* Mii end                                                              */
/************************************************************************/


/************************************************************************/
/* Allstars start                                                       */
/************************************************************************/
#define ALLSTARS_NR_NOTES 14

const note allstars_notes[ALLSTARS_NR_NOTES] PROGMEM = {
	{F_S_TONE, Middel_Octave, 4, 1},
	{C_S_TONE, Middel_Octave + 1, 8, 1},
	{A_S_TONE, Middel_Octave, 8, 1},
	{A_S_TONE, Middel_Octave, 4, 1},
	{G_S_TONE, Middel_Octave, 8, 1},
	{F_S_TONE, Middel_Octave, 8, 1},
	{F_S_TONE, Middel_Octave, 8, 1},
	{B_TONE, Middel_Octave, 4, 1},
	{A_S_TONE, Middel_Octave, 8, 1},
	{A_S_TONE, Middel_Octave, 8, 1},
	{G_S_TONE, Middel_Octave, 8, 1},
	{G_S_TONE, Middel_Octave, 8, 1},
	{F_S_TONE, Middel_Octave, 4, 1},
	{F_S_TONE, Middel_Octave, 8, 1}
};

const song allstars_song = {
	SONG_CALC_PACE(25),
	ALLSTARS_NR_NOTES,
	allstars_notes
};

const song* get_allstars_song()
{
	return &allstars_song;
}

/************************************************************************/
/* Allstars end                                                         */
/************************************************************************/


/************************************************************************/
/* Halo theme start                                                     */
/************************************************************************/
#define HALO_THEME_NR_NOTES 29

const note halo_theme_notes[HALO_THEME_NR_NOTES] PROGMEM = {
	
	{E_TONE, Middel_Octave, 2, 1},
	{G_TONE, Middel_Octave, 4, 1},
	{F_S_TONE, Middel_Octave, 4, 1},
	
	{A_TONE, Middel_Octave, 4, 1},
	{G_TONE, Middel_Octave, 4, 1},
	{F_S_TONE, Middel_Octave, 2, 1},
	
	{E_TONE, Middel_Octave, 1, 1},
	
	{0, 0, 2, 1},
	{B_TONE, Middel_Octave, 4, 1},
	{C_S_TONE, Middel_Octave + 1, 4, 1},
	
	{D_TONE, Middel_Octave + 1, 2, 1},
	{C_S_TONE, Middel_Octave + 1, 4, 1},
	{A_TONE, Middel_Octave, 4, 1},
	
	{C_S_TONE, Middel_Octave + 1, 4, 1},
	{B_TONE, Middel_Octave, 4, 3},
	
	{0, 0, 4, 1},
	{B_TONE, Middel_Octave - 1, 4, 1},
	{D_TONE, Middel_Octave, 4, 1},
	{E_TONE, Middel_Octave, 4, 1},
	
	{G_TONE, Middel_Octave, 4, 1},
	{A_TONE, Middel_Octave, 4, 1},
	{F_S_TONE, Middel_Octave, 2, 1},
	
	{E_TONE, Middel_Octave, 4, 1},
	{G_TONE, Middel_Octave, 4, 1},
	{F_S_TONE, Middel_Octave, 4, 1},
	{E_TONE, Middel_Octave, 4, 1},
	
	{F_S_TONE, Middel_Octave, 4, 1},
	{D_TONE, Middel_Octave, 4, 3},
	{E_TONE, Middel_Octave, 2, 1}
};

const song halo_theme_song = {
	SONG_CALC_PACE(29),
	HALO_THEME_NR_NOTES,
	halo_theme_notes
};

const song* get_halo_theme_song()
{
	return &halo_theme_song;
}

/************************************************************************/
/* Halo theme end                                                       */
/************************************************************************/


/************************************************************************/
/* Imperial march start                                                 */
/************************************************************************/
#define IMPERIAL_MARCH_NR_NOTES 18

const note imperial_march_notes[IMPERIAL_MARCH_NR_NOTES] PROGMEM = {
	
	{G_TONE, Middel_Octave, 4, 1},
	{G_TONE, Middel_Octave, 4, 1},
	{G_TONE, Middel_Octave, 4, 1},
	{D_S_TONE, Middel_Octave, 16, 3},
	{A_S_TONE, Middel_Octave, 16, 1},
	
	{G_TONE, Middel_Octave, 4, 1},
	{D_S_TONE, Middel_Octave, 16, 3},
	{A_S_TONE, Middel_Octave, 16, 1},
	{G_TONE, Middel_Octave, 2, 1},
	
	{D_TONE, Middel_Octave + 1, 4, 1},
	{D_TONE, Middel_Octave + 1, 4, 1},
	{D_TONE, Middel_Octave + 1, 4, 1},
	{D_S_TONE, Middel_Octave + 1, 16, 3},
	{A_S_TONE, Middel_Octave, 16, 1},
	
	{F_S_TONE, Middel_Octave, 4, 1},
	{D_S_TONE, Middel_Octave, 16, 3},
	{A_S_TONE, Middel_Octave, 16, 1},
	{G_TONE, Middel_Octave, 4, 1}
};

const song imperial_march_song = {
	SONG_CALC_PACE(27),
	IMPERIAL_MARCH_NR_NOTES,
	imperial_march_notes
};

const song* get_imperial_march_song()
{
	return &imperial_march_song;
}

/************************************************************************/
/* Imperial march end                                                   */
/************************************************************************/


/************************************************************************/
/* Soviet national anthem start                                         */
/************************************************************************/
#define SOVIET_ANTHEM_NR_NOTES 26

const note soviet_anthem_notes[SOVIET_ANTHEM_NR_NOTES] PROGMEM = {
	
	{C_TONE, Middel_Octave + 1, 4, 3},
	{0, 0, 8, 1},
	{G_TONE, Middel_Octave, 8, 1},
	
	{C_TONE, Middel_Octave + 1, 4, 1},
	{G_TONE, Middel_Octave, 16, 3},
	{A_TONE, Middel_Octave, 16, 1},
	{B_TONE, Middel_Octave, 4, 1},
	{E_TONE, Middel_Octave, 8, 1},
	{E_TONE, Middel_Octave, 8, 1},
	
	{A_TONE, Middel_Octave, 4, 1},
	{G_TONE, Middel_Octave, 16, 3},
	{F_TONE, Middel_Octave, 16, 1},
	{G_TONE, Middel_Octave, 4, 1},
	{C_TONE, Middel_Octave, 8, 1},
	{C_TONE, Middel_Octave, 8, 1},
	
	{D_TONE, Middel_Octave, 4, 1},
	{D_TONE, Middel_Octave, 16, 3},
	{E_TONE, Middel_Octave, 16, 1},
	{F_TONE, Middel_Octave, 4, 1},
	{F_TONE, Middel_Octave, 16, 3},
	{G_TONE, Middel_Octave, 16, 1},
	
	{A_TONE, Middel_Octave, 4, 1},
	{B_TONE, Middel_Octave, 8, 1},
	{C_TONE, Middel_Octave + 1, 8, 1},
	{D_TONE, Middel_Octave + 1, 8, 3},
	{G_TONE, Middel_Octave, 8, 1}
};

const song soviet_anthem_song = {
	SONG_CALC_PACE(17),
	SOVIET_ANTHEM_NR_NOTES,
	soviet_anthem_notes
};

const song* get_soviet_anthem_song()
{
	return &soviet_anthem_song;
}

/************************************************************************/
/* Soviet national anthem end                                           */
/************************************************************************/


/************************************************************************/
/* Horn start                                                           */
/************************************************************************/
#define HORN_NR_NOTES 3

const note horn_notes[HORN_NR_NOTES] PROGMEM = {
	{A_TONE, Middel_Octave, 8, 1},
	{0, 0, 8, 1},
	{A_TONE, Middel_Octave, 8, 1}
};

const song horn_song = {
	SONG_CALC_PACE(62),
	HORN_NR_NOTES,
	horn_notes
};

const song* get_horn_song()
{
	return &horn_song;
}

/************************************************************************/
/* Horn end                                                             */
/************************************************************************/


/************************************************************************/
/* Cantina band start                                                   */
/************************************************************************/
#define CANTINA_BAND_NR_NOTES 61

const note cantina_band_notes[CANTINA_BAND_NR_NOTES] PROGMEM = {
	
	{A_TONE, Middel_Octave, 4, 1},
	{D_TONE, Middel_Octave + 1, 4, 1},
	{A_TONE, Middel_Octave, 4, 1},
	{D_TONE, Middel_Octave + 1, 4, 1},
	
	{A_TONE, Middel_Octave, 4, 1},
	{D_TONE, Middel_Octave + 1, 4, 1},
	{A_TONE, Middel_Octave, 8, 1},
	{G_S_TONE, Middel_Octave, 8, 1},
	{A_TONE, Middel_Octave, 4, 1},
	
	{A_TONE, Middel_Octave, 8, 1},
	{G_S_TONE, Middel_Octave, 8, 1},
	{A_TONE, Middel_Octave, 8, 1},
	{G_TONE, Middel_Octave, 8, 1},
	{0, 0, 8, 1},
	{F_S_TONE, Middel_Octave, 8, 1},
	{G_TONE, Middel_Octave, 8, 1},
	{F_S_TONE, Middel_Octave, 8, 1},
	
	{F_TONE, Middel_Octave, 2, 1},
	{D_TONE, Middel_Octave, 4, 1},
	{0, 0, 4, 1},
	
	{A_TONE, Middel_Octave, 4, 1},
	{D_TONE, Middel_Octave + 1, 4, 1},
	{A_TONE, Middel_Octave, 4, 1},
	{D_TONE, Middel_Octave + 1, 4, 1},
	
	{A_TONE, Middel_Octave, 4, 1},
	{D_TONE, Middel_Octave + 1, 4, 1},
	{A_TONE, Middel_Octave, 8, 1},
	{G_S_TONE, Middel_Octave, 8, 1},
	{A_TONE, Middel_Octave, 4, 1},
	
	{G_TONE, Middel_Octave, 4, 1},
	{G_TONE, Middel_Octave, 4, 1},
	{F_S_TONE, Middel_Octave, 4, 1},
	{G_TONE, Middel_Octave, 4, 1},
	
	{C_TONE, Middel_Octave + 1, 4, 1},
	{A_S_TONE, Middel_Octave, 4, 1},
	{A_TONE, Middel_Octave, 4, 1},
	{G_TONE, Middel_Octave, 4, 1},
	
	{A_TONE, Middel_Octave, 4, 1},
	{D_TONE, Middel_Octave + 1, 4, 1},
	{A_TONE, Middel_Octave, 4, 1},
	{D_TONE, Middel_Octave + 1, 4, 1},
	
	{A_TONE, Middel_Octave, 4, 1},
	{D_TONE, Middel_Octave + 1, 4, 1},
	{A_TONE, Middel_Octave, 8, 1},
	{G_S_TONE, Middel_Octave, 8, 1},
	{A_TONE, Middel_Octave, 4, 1},
	
	{C_TONE, Middel_Octave + 1, 4, 1},
	{C_TONE, Middel_Octave + 1, 4, 1},
	{A_TONE, Middel_Octave, 4, 1},
	{G_TONE, Middel_Octave, 4, 1},
	
	{F_TONE, Middel_Octave, 2, 1},
	{D_TONE, Middel_Octave, 2, 1},
	
	{D_TONE, Middel_Octave, 2, 1},
	{F_TONE, Middel_Octave, 2, 1},
	
	{A_TONE, Middel_Octave, 2, 1},
	{C_TONE, Middel_Octave + 1, 2, 1},
	
	{D_S_TONE, Middel_Octave + 1, 4, 1},
	{D_TONE, Middel_Octave + 1, 4, 1},
	{G_S_TONE, Middel_Octave, 8, 1},
	{A_TONE, Middel_Octave, 4, 1},
	{F_TONE, Middel_Octave, 8, 1}
};

const song cantina_band_song = {
	SONG_CALC_PACE(62),
	CANTINA_BAND_NR_NOTES,
	cantina_band_notes
};

const song* get_cantina_band_song()
{
	return &cantina_band_song;
}

/************************************************************************/
/* Cantina band end                                                     */
/************************************************************************/


/************************************************************************/
/* Mario galaxy start                                                   */
/************************************************************************/
#define MARIO_GALAXY_NR_NOTES 61

const note mario_galaxy_notes[MARIO_GALAXY_NR_NOTES] PROGMEM = {
	
	{F_TONE, Middel_Octave + 1, 8, 5},
	{E_TONE, Middel_Octave + 1, 8, 1},
	{G_TONE, Middel_Octave + 1, 8, 1},
	{F_TONE, Middel_Octave + 1, 8, 1},
	
	{C_TONE, Middel_Octave + 1, 8, 5},
	{A_TONE, Middel_Octave, 8, 1},
	{A_S_TONE, Middel_Octave, 8, 1},
	{C_TONE, Middel_Octave + 1, 8, 1},
	
	{C_TONE, Middel_Octave + 1, 8, 3},
	{A_S_TONE, Middel_Octave, 8, 5},
	
	{0, 0, 1, 1},
	
	{G_TONE, Middel_Octave + 1, 8, 5},
	{F_S_TONE, Middel_Octave + 1, 8, 1},
	{A_TONE, Middel_Octave + 1, 8, 1},
	{G_TONE, Middel_Octave + 1, 8, 1},
	
	{F_TONE, Middel_Octave + 1, 8, 3},
	{E_TONE, Middel_Octave + 1, 4, 1},
	{D_TONE, Middel_Octave + 1, 4, 1},
	{E_TONE, Middel_Octave + 1, 8, 1},
	
	{D_TONE, Middel_Octave + 1, 8, 3},
	{C_TONE, Middel_Octave + 1, 8, 5},
	
	{0, 0, 1, 1},
	
	{C_TONE, Middel_Octave + 2, 8, 5},
	{B_TONE, Middel_Octave + 1, 8, 1},
	{D_TONE, Middel_Octave + 2, 8, 1},
	{C_TONE, Middel_Octave + 2, 8, 1},
	
	{A_S_TONE, Middel_Octave + 1, 8, 3},
	{A_TONE, Middel_Octave + 1, 8, 5},
	
	{A_S_TONE, Middel_Octave + 1, 8, 5},
	{A_TONE, Middel_Octave + 1, 8, 1},
	{C_TONE, Middel_Octave + 2, 8, 1},
	{A_S_TONE, Middel_Octave + 1, 8, 1},
		
	{A_TONE, Middel_Octave + 1, 8, 3},
	{G_TONE, Middel_Octave + 1, 8, 5},
	
	{A_TONE, Middel_Octave + 1, 8, 5},
	{G_S_TONE, Middel_Octave + 1, 8, 1},
	{A_S_TONE, Middel_Octave + 1, 8, 1},
	{A_TONE, Middel_Octave + 1, 8, 1},
	
	{G_TONE, Middel_Octave + 1, 8, 3},
	{F_TONE, Middel_Octave + 1, 8, 2},
	{A_TONE, Middel_Octave + 1, 4, 1},
	{G_TONE, Middel_Octave + 1, 8, 1},
	
	{F_TONE, Middel_Octave + 1, 8, 3},
	{E_TONE, Middel_Octave + 1, 8, 5},
	
	{F_TONE, Middel_Octave + 1, 8, 3},
	{G_TONE, Middel_Octave + 1, 8, 5},
};

const song mario_galaxy_song = {
	SONG_CALC_PACE(38),
	MARIO_GALAXY_NR_NOTES,
	mario_galaxy_notes
};

const song* get_mario_galaxy_song()
{
	return &mario_galaxy_song;
}

/************************************************************************/
/* Mario galaxy end                                                     */
/************************************************************************/

/************************************************************************/
/* Pokemon center start                                                 */
/************************************************************************/
#define POKEMON_CENTER_NR_NOTES 65

const note pokemon_center_notes[POKEMON_CENTER_NR_NOTES] PROGMEM = {
	{D_TONE, Middel_Octave + 1, 8, 1},
	{A_TONE, Middel_Octave, 8, 1},
	{D_TONE, Middel_Octave + 1, 8, 1},
	{A_TONE, Middel_Octave + 1, 4, 1},
	{G_TONE, Middel_Octave + 1, 4, 1},
	{F_S_TONE, Middel_Octave + 1, 8, 1},
		
	{E_TONE, Middel_Octave + 1, 8, 1},
	{C_S_TONE, Middel_Octave + 1, 8, 3},
	{0, 0, 2, 1},
	
	{C_S_TONE, Middel_Octave + 1, 8, 1},
	{A_TONE, Middel_Octave, 8, 1},
	{C_S_TONE, Middel_Octave + 1, 8, 1},
	{F_S_TONE, Middel_Octave + 1, 4, 1},
	{E_TONE, Middel_Octave + 1, 4, 1},
	{C_S_TONE, Middel_Octave + 1, 8, 1},
	
	{D_TONE, Middel_Octave + 1, 8, 1},
	{F_S_TONE, Middel_Octave + 1, 8, 3},
	{0, 0, 2, 1},
	
	{D_TONE, Middel_Octave + 1, 8, 1},
	{A_TONE, Middel_Octave, 8, 1},
	{D_TONE, Middel_Octave + 1, 8, 1},
	{A_TONE, Middel_Octave + 1, 4, 1},
	{G_TONE, Middel_Octave + 1, 4, 1},
	{F_S_TONE, Middel_Octave + 1, 8, 1},
	
	{E_TONE, Middel_Octave + 1, 8, 1},
	{C_S_TONE, Middel_Octave + 1, 8, 3},
	{0, 0, 2, 1},
	
	{C_S_TONE, Middel_Octave + 1, 8, 1},
	{A_TONE, Middel_Octave, 8, 1},
	{C_S_TONE, Middel_Octave + 1, 8, 1},
	{F_S_TONE, Middel_Octave + 1, 4, 1},
	{E_TONE, Middel_Octave + 1, 4, 1},
	{C_S_TONE, Middel_Octave + 1, 8, 1},
	
	{D_TONE, Middel_Octave + 1, 2, 1},
	{0, 0, 2, 1},
	
	{F_S_TONE, Middel_Octave + 1, 2, 1},
	{A_TONE, Middel_Octave + 1, 2, 1},
	
	{G_TONE, Middel_Octave + 1, 8, 1},
	{A_TONE, Middel_Octave + 1, 8, 1},
	{G_TONE, Middel_Octave + 1, 8, 1},
	{F_S_TONE, Middel_Octave + 1, 8, 1},
	{E_TONE, Middel_Octave + 1, 2, 1},
	
	{C_S_TONE, Middel_Octave + 1, 2, 1},
	{E_TONE, Middel_Octave + 1, 2, 1},
	
	{F_S_TONE, Middel_Octave + 1, 8, 1},
	{G_TONE, Middel_Octave + 1, 8, 1},
	{F_S_TONE, Middel_Octave + 1, 8, 1},
	{E_TONE, Middel_Octave + 1, 8, 1},
	{D_TONE, Middel_Octave + 1, 2, 1},
	
	{F_S_TONE, Middel_Octave + 1, 2, 1},
	{A_TONE, Middel_Octave + 1, 2, 1},
	
	{G_TONE, Middel_Octave + 1, 8, 1},
	{F_S_TONE, Middel_Octave + 1, 8, 1},
	{G_TONE, Middel_Octave + 1, 8, 1},
	{A_TONE, Middel_Octave + 1, 8, 1},
	{B_TONE, Middel_Octave + 1, 2, 1},
	
	{A_TONE, Middel_Octave + 1, 4, 1},
	{G_TONE, Middel_Octave + 1, 8, 1},
	{F_S_TONE, Middel_Octave + 1, 8, 1},
	{G_TONE, Middel_Octave + 1, 2, 1},
	
	{F_S_TONE, Middel_Octave + 1, 8, 1},
	{G_TONE, Middel_Octave + 1, 8, 1},
	{F_S_TONE, Middel_Octave + 1, 8, 1},
	{E_TONE, Middel_Octave + 1, 8, 1},
	{D_TONE, Middel_Octave + 1, 2, 1},
};

const song pokemon_center_song = {
	SONG_CALC_PACE(33),
	POKEMON_CENTER_NR_NOTES,
	pokemon_center_notes
};

const song* get_pokemon_center_song()
{
	return &pokemon_center_song;
}

/************************************************************************/
/* Pokemon center end                                                   */
/************************************************************************/

/************************************************************************/
/* Petalburg start                                                      */
/************************************************************************/
#define PETALBURG_NR_NOTES 108

const note petalburg_notes[PETALBURG_NR_NOTES] PROGMEM = {
	{A_S_TONE, Middel_Octave, 8, 1},
	{0, 0, 8, 1},
	{C_TONE, Middel_Octave + 1, 8, 1},
	{0, 0, 8, 1},
	{D_TONE, Middel_Octave + 1, 16, 3},
	{D_S_TONE, Middel_Octave + 1, 16, 3},
	{F_TONE, Middel_Octave + 1, 16, 2},
	
	{D_S_TONE, Middel_Octave + 1, 16, 3},
	{0, 0, 16, 1},
	{D_S_TONE, Middel_Octave + 1, 16, 1},
	{F_TONE, Middel_Octave + 1, 16, 1},
	{D_S_TONE, Middel_Octave + 1, 8, 1},
	{D_TONE, Middel_Octave + 1, 16, 3},
	{0, 0, 16, 1},
	{D_TONE, Middel_Octave + 1, 16, 1},
	{D_S_TONE, Middel_Octave + 1, 16, 1},
	{D_TONE, Middel_Octave + 1, 8, 1},
	
	{C_TONE, Middel_Octave + 1, 2, 1},
	{0, 0, 8, 1},
	{F_TONE, Middel_Octave + 1, 8, 1},
	{G_S_TONE, Middel_Octave + 1, 8, 1},
	{F_TONE, Middel_Octave + 1, 8, 1},
	
	{F_TONE, Middel_Octave + 1, 16, 3},
	{0, 0, 16, 1},
	{F_TONE, Middel_Octave + 1, 16, 1},
	{G_TONE, Middel_Octave + 1, 16, 1},
	{G_S_TONE, Middel_Octave + 1, 8, 1},
	{C_TONE, Middel_Octave + 1, 16, 3},
	{0, 0, 16, 1},
	{C_TONE, Middel_Octave + 1, 16, 1},
	{D_TONE, Middel_Octave + 1, 16, 1},
	{D_S_TONE, Middel_Octave + 1, 8, 1},
	
	{D_TONE, Middel_Octave + 1, 8, 3},
	{C_TONE, Middel_Octave + 1, 8, 1},
	{A_S_TONE, Middel_Octave, 2, 1},
	
	{G_TONE, Middel_Octave + 1, 16, 3},
	{0, 0, 16, 1},
	{G_TONE, Middel_Octave + 1, 16, 1},
	{G_S_TONE, Middel_Octave + 1, 16, 1},
	{G_TONE, Middel_Octave + 1, 8, 1},
	{F_TONE, Middel_Octave + 1, 16, 3},
	{0, 0, 16, 1},
	{F_TONE, Middel_Octave + 1, 16, 1},
	{G_TONE, Middel_Octave + 1, 16, 1},
	{F_TONE, Middel_Octave + 1, 8, 1},
	
	{D_S_TONE, Middel_Octave + 1, 2, 1},
	{0, 0, 8, 1},
	{A_S_TONE, Middel_Octave, 8, 1},
	{D_S_TONE, Middel_Octave + 1, 8, 1},
	{A_S_TONE, Middel_Octave + 1, 8, 1},
	
	{G_S_TONE, Middel_Octave + 1, 16, 3},
	{0, 0, 16, 1},
	{G_S_TONE, Middel_Octave + 1, 16, 1},
	{A_S_TONE, Middel_Octave + 1, 16, 1},
	{G_S_TONE, Middel_Octave + 1, 8, 1},
	{G_TONE, Middel_Octave + 1, 16, 3},
	{0, 0, 16, 1},
	{G_TONE, Middel_Octave + 1, 16, 1},
	{G_S_TONE, Middel_Octave + 1, 16, 1},
	{G_TONE, Middel_Octave + 1, 8, 1},
	
	{C_S_TONE, Middel_Octave + 1, 8, 3},
	{D_S_TONE, Middel_Octave + 1, 8, 1},
	{F_TONE, Middel_Octave + 1, 4, 1},
	{G_TONE, Middel_Octave, 8, 1},
	{G_S_TONE, Middel_Octave, 8, 1},
	
	{A_S_TONE, Middel_Octave, 8, 1},
	{0, 0, 8, 4},
	{A_S_TONE, Middel_Octave, 8, 1},
	{C_TONE, Middel_Octave + 1, 8, 1},
	{C_S_TONE, Middel_Octave + 1, 8, 1},
		
	{C_TONE, Middel_Octave + 1, 8, 1},
	{0, 0, 8, 4},
	{G_TONE, Middel_Octave, 8, 1},
	{C_TONE, Middel_Octave + 1, 8, 1},
	{G_TONE, Middel_Octave, 8, 1},
	
	{G_S_TONE, Middel_Octave, 8, 1},
	{0, 0, 8, 4},
	{G_S_TONE, Middel_Octave, 8, 1},
	{G_TONE, Middel_Octave, 8, 1},
	{G_S_TONE, Middel_Octave, 8, 1},
	
	{A_S_TONE, Middel_Octave, 8, 1},
	{0, 0, 8, 1},
	{G_S_TONE, Middel_Octave, 8, 1},
	{0, 0, 8, 1},
	{G_TONE, Middel_Octave, 8, 1},
	{F_TONE, Middel_Octave, 4, 1},
	{D_S_TONE, Middel_Octave, 8, 1},
		
	{C_TONE, Middel_Octave + 2, 2, 1},
	{0, 0, 8, 1},
	{C_TONE, Middel_Octave + 2, 8, 1},
	{D_TONE, Middel_Octave + 2, 8, 1},
	{D_S_TONE, Middel_Octave + 2, 8, 1},
		
	{D_TONE, Middel_Octave + 2, 2, 1},
	{0, 0, 8, 1},
	{G_TONE, Middel_Octave + 1, 8, 1},
	{A_S_TONE, Middel_Octave + 1, 8, 1},
	{C_TONE, Middel_Octave + 2, 8, 1},
	
	{G_S_TONE, Middel_Octave + 1, 2, 1},
	{0, 0, 8, 1},
	{G_S_TONE, Middel_Octave + 1, 8, 1},
	{F_TONE, Middel_Octave + 1, 8, 1},
	{G_S_TONE, Middel_Octave + 1, 8, 1},
		
	{F_TONE, Middel_Octave + 1, 4, 1},
	{G_TONE, Middel_Octave + 1, 4, 1},
	{G_S_TONE, Middel_Octave + 1, 16, 3},
	{G_TONE, Middel_Octave + 1, 16, 3},
	{F_TONE, Middel_Octave + 1, 16, 2},
			
	{D_S_TONE, Middel_Octave + 1, 1, 1}
};

const song petalburg_song = {
	SONG_CALC_PACE(33),
	PETALBURG_NR_NOTES,
	petalburg_notes
};

const song* get_petalburg_song()
{
	return &petalburg_song;
}

/************************************************************************/
/* Petalburg end                                                        */
/************************************************************************/