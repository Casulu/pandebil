/************************************************************************
* Project:		Logger.c
* Author:		Simon Ullerstam <ullr@cs.umu.se>
* File:			sd.h
* Description:	Header file for sd.c
* Created:		2021-05-07
************************************************************************/

#ifndef SD_H_
#define SD_H_

/*************** External library's ***************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

/*************** Other header files ***************/
#include "ff.h"
#include "diskio.h"
#include "ffconf.h"

/*************** Constants ***************/
#define SD_PORT PORTB
#define SD_PIN PINB
#define SD_DDR DDRB
#define SD_DET PINB7

#define TO_CAR_LOG "TO_CAR.CSV"
#define FROM_CAR_LOG "FROM_CAR.CSV"
#define TO_REM1_LOG "TO_REM1.CSV"
#define TO_REM2_LOG "TO_REM2.CSV"
#define NO_TOPIC_LOG "NO_TOP.CSV"

/*************** Global variable ***************/
FATFS FatFs;

/*************** Function declaration ***************/
/**
 @brief  Initializes SD-card reader. Needs to be called once
 @param  void
 @return none
 */
void sdInit(void);

/**
 @brief  Checks if the SD-card is inserted
 @param  void
 @return true or false if inserted or not
 */
bool sdCardInserted(void);

/**
 @brief  Mounts the SD-card. Needs to be called after insertion of card
 @param  void
 @return none
 */
void sdCardMount(void);

/**
 @brief  Writes data to file in SD-card 
 @param  The file name of file to write to, the data that should be written
 @return none
 */
void sdCardWrite(const char *fileName, char *str, uint64_t time);

#endif // SD_H_
