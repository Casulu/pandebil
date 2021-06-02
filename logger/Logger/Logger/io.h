/************************************************************************
* Project:		Logger.c
* Author:		Simon Ullerstam <ullr@cs.umu.se>
* File:			io.h
* Description:	Header file for io.c
* Created:		2021-05-19
************************************************************************/

#ifndef IO_H_
#define IO_H_

/*************** External library ***************/
#include <avr/io.h>
#include <stdbool.h>

/*************** Constants ***************/
#define LED_DDR DDRC
#define LED_PORT PORTC
#define SD_LED PORTC2
#define STOP_LED PORTC3
#define GREEN_HEART_LED PORTC4
#define RED_HEART_LED PORTC5
#define IO_DDR DDRD
#define IO_PORT PORTD
#define IO_PIN PIND
#define STOP_BTN PIND2
#define SUMMER PIND3
#define UNSTOP_BTN PIND4
#define PING_BTN PIND5
#define MODE_SWITCH PIND6

/*************** Global variables ***************/
volatile uint32_t timePressed;
volatile uint8_t btnPressed;

/*************** Function declaration ***************/
/**
 @brief  Initializes IO. Needs to be called once
 @param  void
 @return none
 */
void ioInit(void);

/**
 @brief  Sets led for SD mounted
 @param  The led to set, true or false if on or off
 @return none
 */
void setLed(uint8_t led, bool on);

/**
 @brief  Checks if buttons pressed
 @param  system time in ms
 @return number corresponding to button, 0 if none
 */
uint8_t btnIsPressed(uint32_t ms);

/**
 @brief  Sets the frequency of timer summer gets triggered by
 @param  freq  frequency to set
 @return none
 */
void summerSetFrequency(uint8_t freq);

/**
 @brief  Checks which mode is selected
 @param  void
 @return true or false depending on mode
 */
bool normalMode(void);

#endif // IO_H_