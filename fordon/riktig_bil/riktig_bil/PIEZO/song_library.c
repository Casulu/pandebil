#include "song.h"

void set_imperial_march();
void set_halo_theme();
void set_soviet_anthem();
void set_mario();
void set_allstars();
void set_mii();
void set_cantina_band();

void set_cantina_band()
{
	song_empty(61, 62);
	
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	song_add_note(D_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	song_add_note(D_TONE, Middel_Octave + 1, 4, 1, false);
	
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	song_add_note(D_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(A_TONE, Middel_Octave, 8, 1, false);
	song_add_note(G_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	
	song_add_note(A_TONE, Middel_Octave, 8, 1, false);
	song_add_note(G_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(A_TONE, Middel_Octave, 8, 1, false);
	song_add_note(G_TONE, Middel_Octave, 8, 1, false);
	song_add_note(0, 0, 8, 1, true);
	song_add_note(F_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(G_TONE, Middel_Octave, 8, 1, false);
	song_add_note(F_S_TONE, Middel_Octave, 8, 1, false);
	
	song_add_note(F_TONE, Middel_Octave, 2, 1, false);
	song_add_note(D_TONE, Middel_Octave, 4, 1, false);
	song_add_note(0, 0, 4, 1, true);
	
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	song_add_note(D_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	song_add_note(D_TONE, Middel_Octave + 1, 4, 1, false);
	
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	song_add_note(D_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(A_TONE, Middel_Octave, 8, 1, false);
	song_add_note(G_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	
	song_add_note(G_TONE, Middel_Octave, 4, 1, false);
	song_add_note(G_TONE, Middel_Octave, 4, 1, false);
	song_add_note(F_S_TONE, Middel_Octave, 4, 1, false);
	song_add_note(G_TONE, Middel_Octave, 4, 1, false);
	
	song_add_note(C_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(A_S_TONE, Middel_Octave, 4, 1, false);
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	song_add_note(G_TONE, Middel_Octave, 4, 1, false);
	
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	song_add_note(D_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	song_add_note(D_TONE, Middel_Octave + 1, 4, 1, false);
	
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	song_add_note(D_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(A_TONE, Middel_Octave, 8, 1, false);
	song_add_note(G_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	
	song_add_note(C_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(C_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	song_add_note(G_TONE, Middel_Octave, 4, 1, false);
	
	song_add_note(F_TONE, Middel_Octave, 2, 1, false);
	song_add_note(D_TONE, Middel_Octave, 2, 1, false);
	
	song_add_note(D_TONE, Middel_Octave, 2, 1, false);
	song_add_note(F_TONE, Middel_Octave, 2, 1, false);
	
	song_add_note(A_TONE, Middel_Octave, 2, 1, false);
	song_add_note(C_TONE, Middel_Octave + 1, 2, 1, false);
	
	song_add_note(D_S_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(D_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(G_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	song_add_note(F_TONE, Middel_Octave, 8, 1, false);
	
}

void set_soviet_anthem()
{
	song_empty(25, 17);
	
	song_add_note(C_TONE, Middel_Octave + 1, 4, 3, false);
	song_add_note(0, 0, 8, 1, true);
	song_add_note(G_TONE, Middel_Octave, 8, 1, false);
	
	song_add_note(C_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(G_TONE, Middel_Octave, 16, 3, false);
	song_add_note(A_TONE, Middel_Octave, 16, 1, false);
	song_add_note(B_TONE, Middel_Octave, 4, 1, false);
	song_add_note(E_TONE, Middel_Octave, 8, 1, false);
	song_add_note(E_TONE, Middel_Octave, 8, 1, false);
	
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	song_add_note(G_TONE, Middel_Octave, 16, 3, false);
	song_add_note(F_TONE, Middel_Octave, 16, 1, false);
	song_add_note(G_TONE, Middel_Octave, 4, 1, false);
	song_add_note(C_TONE, Middel_Octave, 8, 1, false);
	song_add_note(C_TONE, Middel_Octave, 8, 1, false);
	
	song_add_note(D_TONE, Middel_Octave, 4, 1, false);
	song_add_note(D_TONE, Middel_Octave, 16, 3, false);
	song_add_note(E_TONE, Middel_Octave, 16, 1, false);
	song_add_note(F_TONE, Middel_Octave, 4, 1, false);
	song_add_note(F_TONE, Middel_Octave, 16, 3, false);
	song_add_note(G_TONE, Middel_Octave, 16, 1, false);
	
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	song_add_note(B_TONE, Middel_Octave, 8, 1, false);
	song_add_note(C_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(D_TONE, Middel_Octave + 1, 8, 3, false);
	song_add_note(G_TONE, Middel_Octave, 8, 1, false);
}

void set_imperial_march()
{
	song_empty(17, 27);
	
	song_add_note(G_TONE, Middel_Octave, 4, 1, false);
	song_add_note(G_TONE, Middel_Octave, 4, 1, false);
	song_add_note(G_TONE, Middel_Octave, 4, 1, false);
	song_add_note(D_S_TONE, Middel_Octave, 16, 3, false);
	song_add_note(A_S_TONE, Middel_Octave, 16, 1, false);
	
	song_add_note(G_TONE, Middel_Octave, 4, 1, false);
	song_add_note(D_S_TONE, Middel_Octave, 16, 3, false);
	song_add_note(A_S_TONE, Middel_Octave, 16, 1, false);
	song_add_note(G_TONE, Middel_Octave, 2, 1, false);
	
	song_add_note(D_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(D_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(D_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(D_S_TONE, Middel_Octave + 1, 16, 3, false);
	song_add_note(A_S_TONE, Middel_Octave, 16, 1, false);
	
	song_add_note(F_S_TONE, Middel_Octave, 4, 1, false);
	song_add_note(D_S_TONE, Middel_Octave, 16, 3, false);
	song_add_note(A_S_TONE, Middel_Octave, 16, 1, false);
	song_add_note(G_TONE, Middel_Octave, 4, 1, false);
}

void set_halo_theme()
{
	song_empty(28, 25);
	
	song_add_note(E_TONE, Middel_Octave, 2, 1, false);
	song_add_note(G_TONE, Middel_Octave, 4, 1, false);
	song_add_note(F_S_TONE, Middel_Octave, 4, 1, false);
	
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	song_add_note(G_TONE, Middel_Octave, 4, 1, false);
	song_add_note(F_S_TONE, Middel_Octave, 2, 1, false);
	
	song_add_note(E_TONE, Middel_Octave, 1, 1, false);
	
	song_add_note(0, 0, 2, 1, true);
	song_add_note(B_TONE, Middel_Octave, 4, 1, false);
	song_add_note(C_S_TONE, Middel_Octave + 1, 4, 1, false);
	
	song_add_note(D_TONE, Middel_Octave + 1, 2, 1, false);
	song_add_note(C_S_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	
	song_add_note(C_S_TONE, Middel_Octave + 1, 4, 1, false);
	song_add_note(B_TONE, Middel_Octave, 4, 3, false);
	
	song_add_note(0, 0, 4, 1, true);
	song_add_note(B_TONE, Middel_Octave - 1, 4, 1, false);
	song_add_note(D_TONE, Middel_Octave, 4, 1, false);
	song_add_note(E_TONE, Middel_Octave, 4, 1, false);
	
	song_add_note(G_TONE, Middel_Octave, 4, 1, false);
	song_add_note(A_TONE, Middel_Octave, 4, 1, false);
	song_add_note(F_S_TONE, Middel_Octave, 2, 1, false);
	
	song_add_note(E_TONE, Middel_Octave, 4, 1, false);
	song_add_note(G_TONE, Middel_Octave, 4, 1, false);
	song_add_note(F_S_TONE, Middel_Octave, 4, 1, false);
	song_add_note(E_TONE, Middel_Octave, 4, 1, false);
	
	song_add_note(F_S_TONE, Middel_Octave, 4, 1, false);
	song_add_note(D_TONE, Middel_Octave, 4, 3, false);
	song_add_note(E_TONE, Middel_Octave, 2, 1, false);
}

void set_mario()
{
	song_empty(12, 50);
	
	song_add_note(E_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(E_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(0.0f, 0, 8, 1, true);
	song_add_note(E_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(0.0f, 0, 8, 1, true);
	song_add_note(C_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(E_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(0.0f, 0, 8, 1, true);
	song_add_note(G_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(0.0f, 0, 4, 1, true);
	song_add_note(0.0f, 0, 8, 1, true);
	song_add_note(G_TONE, Middel_Octave, 8, 1, false);
}

void set_allstars()
{
	song_empty(14, 25);
	
	song_add_note(F_S_TONE, Middel_Octave, 4, 1, false);
	song_add_note(C_S_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(A_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(A_S_TONE, Middel_Octave, 4, 1, false);
	song_add_note(G_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(F_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(F_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(B_TONE, Middel_Octave, 4, 1, false);
	song_add_note(A_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(A_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(G_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(G_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(F_S_TONE, Middel_Octave, 4, 1, false);
	song_add_note(F_S_TONE, Middel_Octave, 8, 1, false);
}

void set_mii()
{
	song_empty(28, 29);
	
	song_add_note(F_S_TONE, Middel_Octave, 4, 1, false);
	song_add_note(A_TONE, Middel_Octave, 8, 1, false);
	song_add_note(C_S_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(0.0f, 0, 8, 1, true);
	song_add_note(A_TONE, Middel_Octave, 8, 1, false);
	song_add_note(0.0f, 0, 8, 1, true);
	song_add_note(F_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(D_TONE, Middel_Octave, 32, 3, false);
	song_add_note(0.0f, 0, 32, 1, true);
	song_add_note(D_TONE, Middel_Octave, 32, 3, false);
	song_add_note(0.0f, 0, 32, 1, true);
	song_add_note(D_TONE, Middel_Octave, 32, 3, false);
	song_add_note(0.0f, 0, 8, 1, true);
	song_add_note(0.0f, 0, 8, 1, true);
	song_add_note(0.0f, 0, 4, 1, true);
	song_add_note(0.0f, 0, 8, 1, true);
	song_add_note(C_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(D_TONE, Middel_Octave, 8, 1, false);
	song_add_note(F_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(A_TONE, Middel_Octave, 8, 1, false);
	song_add_note(C_S_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(0.0f, 0, 8, 1, true);
	song_add_note(A_TONE, Middel_Octave, 8, 1, false);
	song_add_note(0.0f, 0, 8, 1, true);
	song_add_note(F_S_TONE, Middel_Octave, 8, 1, false);
	song_add_note(E_TONE, Middel_Octave + 1, 8, 3, false);
	song_add_note(D_S_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(D_TONE, Middel_Octave + 1, 4, 1, false);
}