/************************************************************************
* Project:		Logger.c
* Author:		Simon Ullerstam <ullr@cs.umu.se>
* File:			sd.c
* Description:	File for handling SD-card reader.
* Created:		2021-05-07
************************************************************************/

/*************** Header file ***************/
#include "sd.h"

/*************** Function definition ***************/
/************************************************************************
Initializes interrupt, SD_DET pin connected to DET.
************************************************************************/
void sdInit(void) {
	// Set SD card-reader DET to input, activate pull-up resistance.
	SD_DDR &= ~(1<<SD_DET);
	SD_PORT |= (1<<SD_DET);
	
	// Start 100Hz system timer with TC0
	OCR0A = F_CPU / 1024 / 100 - 1;
	TCCR0A = (1<<WGM01);
	TCCR0B = 0b101;
	TIMSK0 = (1<<OCIE0A);
}

/************************************************************************
Checks if the SD-card is inserted or not.

Return:	true or false depending on if inserted or not.
************************************************************************/
bool sdCardInserted(void) {
	return bit_is_set(SD_PIN, SD_DET);
}

/************************************************************************
Mounts the SD-card to the default drive.
************************************************************************/
void sdCardMount(void) {
	// Mount to the default drive
	f_mount(&FatFs, "", 1);
}

/************************************************************************
Writes data to the SD-card (has to be mounted). If given file does not
exist it creates it.

Input:	The file name of file to write to, the data that should be 
		written to the file.
************************************************************************/
void sdCardWrite(const char *fileName, char *str, uint64_t ms) {
	FIL fil;
	
	// Open/create and open the file to read, append to end
	f_open(&fil, fileName, FA_OPEN_APPEND | FA_WRITE);
	
	// Print data
	f_printf(&fil, "%s; %d:%02d:%02d\n", str, (uint16_t) (ms/3600000), 
			(uint8_t) ((ms/60000) % 60), (uint8_t) (ms/1000) % 60);
	
	// Close the file
	f_close(&fil);
}
