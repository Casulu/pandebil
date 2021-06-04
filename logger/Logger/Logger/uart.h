/************************************************************************
* Project:		Logger.c
* Author:		Simon Ullerstam <ullr@cs.umu.se>
* File:			uart.h
* Description:	Header file for uart.c
* Created:		2021-05-10
************************************************************************/

#ifndef UART_H_
#define UART_H_

/*************** External library's ***************/
#include <avr/io.h>
#include <stdio.h>

/*************** Function declaration ***************/
/**
 @brief  Initializes uart. Needs to be called once
 @param  void
 @return none
 */
void uartInit(void);

#endif // UART_H_
