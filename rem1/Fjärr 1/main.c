/*
 * Fjärr 1.c
 *
 * Created: 2021-05-10 14:54:00
 * Author : Rasmus Lyxell
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "uart.h"
#include "lcd.h"
#include "spi.h"
#include "adc.h"
#include "summer.h"

#define BTN_DDR DDRD
#define BTN_PORT PORTD
#define BTN_PIN PIND
#define HONKBTN 3
#define EXTRABTN 2
#define DEADMANBTN 4
#define LED_DDR DDRD
#define LED_PORT PORTD
#define LEDG 5
#define LEDR1 6
#define LEDR2 7

void btn_init();
void led_init();

volatile char uart_linebuf[40];
volatile uint8_t uart_bufind = 0;

static int uart_put_char(char c, FILE* stream){
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = c;
	return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(uart_put_char, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	spi_init();
	_delay_ms(100);
	lcd_init();
	adc_init();
	uart_init();
	uart_flush();
	led_init();
	btn_init();
	Summer_Init();
	sei();
	stdout = &mystdout;
    /* Replace with your application code */
    while (1) 
    {
		if(!(BTN_PIN & 1<<DEADMANBTN)){
			
		}
		_delay_ms(10);
    }
}

ISR(USART_RX_vect){
	uart_linebuf[uart_bufind] = UDR0;
	if(uart_linebuf[uart_bufind] == '\n'){
		uart_linebuf[uart_bufind] = '\0';
		//Do something
		clear_line(0);
		set_cursor_pos(0);
		write_lcd_string(uart_linebuf);
		if(uart_linebuf[2] == 'l'){
			switch(uart_linebuf[4]){
				case '1':
					LED_PORT ^= 1<<LEDR1;
					break;
				case '2':
					LED_PORT ^= 1<<LEDR2;
					break;
				case '0':
				default:
					LED_PORT ^= 1<<LEDG;
					break;
			}
		} else if(uart_linebuf[2] == 'p') Summer_PlayMelody(uart_linebuf[4]-48);
		//Done something
		uart_bufind = 0;
	} else{
		uart_bufind++;
	}
}

void btn_init(){
	BTN_DDR &= ~((1<<HONKBTN)|(1<<DEADMANBTN)|(1<<EXTRABTN));
	BTN_PORT |= (1<<HONKBTN)|(1<<DEADMANBTN)|(1<<EXTRABTN);
	EICRA = (2<<ISC00)|(2<<ISC10);
	EIMSK = 3;
}

//Extra
ISR(INT0_vect){
	Summer_PlayMelody(MELODY_HONK);
}

//Honk
ISR(INT1_vect){
	if(BTN_PIN & 1<<DEADMANBTN){
		uart_send_line("2 2 h");
	} else{
		uart_send_line("0 m");
	}
}

void led_init(){
	LED_DDR |= (1<<LEDG)|(1<<LEDR1)|(1<<LEDR2);
	LED_PORT &= ~((1<<LEDG)|(1<<LEDR1)|(1<<LEDR2));
}

