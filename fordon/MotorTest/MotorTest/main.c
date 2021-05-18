#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "UART/uart.h"
#include "PIEZO/piezo.h"
#include "PIEZO/song.h"
#include "I2C/i2cmaster.h"

#define RS_DISP 1
#define SS_DISP 2
#define MOSI 3
#define MISO 4
#define SCK 5

void spi_master_init(void);
void display_init(void);
void spi_master_transmit(char character);
static int display_put_char(char c, FILE *stream);
void display_move_cursor(int steps);
void display_position_cursor(int position);
void display_clear();

static FILE mystdout = FDEV_SETUP_STREAM(display_put_char,
NULL,_FDEV_SETUP_WRITE);

volatile int i;
volatile int timer_counter;
volatile int timer_millisecounds;
volatile int timer_secounds;
volatile int song_timer;
volatile int rps;
volatile char line_buffer[40];
volatile uint8_t line_ind = 0;

volatile song *mario;
volatile song *allstars;
volatile song *mii;

int main(void)
{
	stdout = &mystdout;
	spi_master_init();
	_delay_ms(4);
	display_init();
	uart_init();
	piezo_init();
	i2c_init();
	mario = song_empty(50);
	allstars = song_empty(25);
	mii = song_empty(29);
	
	song_add_note(mario, E_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(mario, E_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(mario, 0.0f, 0, 8, 1, true);
	song_add_note(mario, E_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(mario, 0.0f, 0, 8, 1, true);
	song_add_note(mario, C_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(mario, E_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(mario, 0.0f, 0, 8, 1, true);
	song_add_note(mario, G_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(mario, 0.0f, 0, 4, 1, true);
	song_add_note(mario, 0.0f, 0, 8, 1, true);
	song_add_note(mario, G_TONE, Middel_Octave, 8, 1, false);
	
	song_add_note(allstars, F_S_TONE, Middel_Octave, 4, 1, false);
	song_add_note(allstars, C_S_TONE, Middel_Octave + 1, 8, 1, false);	
	song_add_note(allstars, A_S_TONE, Middel_Octave, 8, 1, false);	
	song_add_note(allstars, A_S_TONE, Middel_Octave, 4, 1, false);	
	song_add_note(allstars, G_S_TONE, Middel_Octave, 8, 1, false);	
	song_add_note(allstars, F_S_TONE, Middel_Octave, 8, 1, false);	
	song_add_note(allstars, F_S_TONE, Middel_Octave, 8, 1, false);	
	song_add_note(allstars, B_TONE, Middel_Octave, 4, 1, false);	
	song_add_note(allstars, A_S_TONE, Middel_Octave, 8, 1, false);	
	song_add_note(allstars, A_S_TONE, Middel_Octave, 8, 1, false);	
	song_add_note(allstars, G_S_TONE, Middel_Octave, 8, 1, false);	
	song_add_note(allstars, G_S_TONE, Middel_Octave, 8, 1, false);	
	song_add_note(allstars, F_S_TONE, Middel_Octave, 4, 1, false);	
	song_add_note(allstars, F_S_TONE, Middel_Octave, 8, 1, false);
	
	song_add_note(mii, F_S_TONE, Middel_Octave, 4, 1, false);
	song_add_note(mii, A_TONE, Middel_Octave, 8, 1, false);	
	song_add_note(mii, C_S_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(mii, 0.0f, 0, 8, 1, true);
	song_add_note(mii, A_TONE, Middel_Octave, 8, 1, false);
	song_add_note(mii, 0.0f, 0, 8, 1, true);
	song_add_note(mii, F_S_TONE, Middel_Octave, 8, 1, false);	
	song_add_note(mii, D_TONE, Middel_Octave, 32, 3, false);
	song_add_note(mii, 0.0f, 0, 32, 1, true);
	song_add_note(mii, D_TONE, Middel_Octave, 32, 3, false);
	song_add_note(mii, 0.0f, 0, 32, 1, true);
	song_add_note(mii, D_TONE, Middel_Octave, 32, 3, false);
	song_add_note(mii, 0.0f, 0, 8, 1, true);
	song_add_note(mii, 0.0f, 0, 8, 1, true);
	song_add_note(mii, 0.0f, 0, 4, 1, true);
	song_add_note(mii, 0.0f, 0, 8, 1, true);
	song_add_note(mii, C_S_TONE, Middel_Octave, 8, 1, false);	
	song_add_note(mii, D_TONE, Middel_Octave, 8, 1, false);	
	song_add_note(mii, F_S_TONE, Middel_Octave, 8, 1, false);	
	song_add_note(mii, A_TONE, Middel_Octave, 8, 1, false);	
	song_add_note(mii, C_S_TONE, Middel_Octave + 1, 8, 1, false);
	song_add_note(mii, 0.0f, 0, 8, 1, true);
	song_add_note(mii, A_TONE, Middel_Octave, 8, 1, false);
	song_add_note(mii, 0.0f, 0, 8, 1, true);
	song_add_note(mii, F_S_TONE, Middel_Octave, 8, 1, false);	
	song_add_note(mii, E_TONE, Middel_Octave + 1, 8, 3, false);	
	song_add_note(mii, D_S_TONE, Middel_Octave + 1, 8, 1, false);	
	song_add_note(mii, D_TONE, Middel_Octave + 1, 4, 1, false);
	
	//song_start(mii);
	//song_start(mario);
	//song_start(allstars);
	
	i = 0;
	rps = 0;
	timer_counter = 0;
	timer_millisecounds = 0;
	timer_secounds = 0;
	song_timer = 0;
	PORTD |= (1<<PORTD1) | (1<<PORTD2);
	PORTC |= (1<<PORTC0);
	DDRD |= (1<<DDD5) | (1<<DDD6);
	DDRC |= (1<<DDC0);
	DDRB |= (1<<DDB6) | (1<<DDB7);
	PORTB |= (1<<PORTB6);
	PORTB &=~ (1<<PORTB7);
	
	TCCR0A &=~ ((1<<COM0A1) | (1<<COM0B1) | (1<<WGM00));
	TCCR0A |= ((1<<COM0A1) | (1<<COM0B1) | (1<<WGM01) | (1<<WGM00));
	TCCR0B &=~ ((1<<FOC0A) | (1<<FOC0B) | (1<<CS00) | (1<<CS02));
	TCCR0B |= (1<<CS00);
	OCR0A = 0x5f;
	OCR0B = 0x5f;
	
	EIMSK |= (1<<INT0);
	EICRA |= (1<<ISC01) | (1<<ISC00);
	
	OCR2A = 100;
	TCCR2A |= (1<<WGM21);
	TCCR2B |= (1<<CS21);
	TIMSK2 |= (1<<OCIE2A);
	sei();
	/*Encoder 12 counts per varv av shaft*/
	unsigned char temp = 0xff;
	unsigned char temp_prev = 0xff;
	i2c_start_wait(0x70 + I2C_WRITE);
	i2c_write(0xff);
	i2c_stop();
    while (1) 
    {
		i2c_start_wait(0x70 + I2C_READ);
		temp_prev = temp;
		temp = i2c_readNak();
		i2c_stop();
		if (!(temp & (1<<2)) && (temp_prev & (1<<2)))
		{
			uart_send_line("1 p 1");
		}
		if (!(temp & (1<<3)) && (temp_prev & (1<<3)))
		{
			uart_send_line("0 l");
		}
		printf("%u     ", rps);
		display_position_cursor(0);
	    //uart_send_line("1 lamp on");
		//uart_send_line("2 spela spel");
		//_delay_ms(10);
		/*i2c_start_wait(0x70 + I2C_READ);
		uart_send_byte(i2c_readAck());
		i2c_stop();*/
		/*printf("%u", rps);
		_delay_ms(2000);
		display_clear();*/
    }
}

void spi_master_init(void)
{
	DDRB |= (1<<SCK) | (1<<MOSI) | (1<<SS_DISP);
	DDRC |= (1<<RS_DISP);
	SPCR |= (1<<SPE) | (1<<MSTR);
	SPSR |= (1<<SPI2X);
	PORTB |= (1<<SS_DISP);
	SPCR &=~ ((1<<CPOL) | (1<<CPHA)); /* SPI mode 0*/
}

void display_init(void)
{
	SPCR |= (1<<SPR0);
	SPCR &=~ (1<<SPR1);
	
	PORTB &=~ (1<<SS_DISP);
	
	//Function Set
	//8 bit data length, 2 lines, instruction table 1
	spi_master_transmit(0x39);
	_delay_us(30);
	
	//Bias Set
	//BS: 1/4, 3 line LCD
	spi_master_transmit(0x1D);
	_delay_us(30);
	
	//Power Control
	//booster off, contrast C5, set C4
	spi_master_transmit(0x50);
	_delay_us(30);
	
	//Follower Control
	//set voltage follower and gain
	spi_master_transmit(0x6C);
	_delay_us(30);
	
	//Contrast Set
	//set contrast C3, C2, C1
	spi_master_transmit(0x7C);
	_delay_us(30);

	//Function Set
	//switch back to instruction table 0
	spi_master_transmit(0x38);
	_delay_us(30);
	
	//Display ON/OFF
	//display on, cursor on, cursor blink
	spi_master_transmit(0x0F);
	_delay_us(30);
	
	//Clear Display
	//delete display, cursor at home
	spi_master_transmit(0x01);
	_delay_ms(1);
	
	//Entry Mode Set
	//cursor auto-increment
	spi_master_transmit(0x06);
	_delay_ms(4);
	
	PORTC |= (1<<RS_DISP);
	PORTB |= (1<<SS_DISP);
}

void spi_master_transmit(char data)
{
	SPDR = data;
	while (!(SPSR & (1<<SPIF)));
}

static int display_put_char(char c, FILE *stream)
{
	/* Set SPI speed for display */
	SPCR |= (1<<SPR0);
	SPCR &=~ (1<<SPR1);
	
	PORTB &=~ (1<<SS_DISP);
	
	/* Transmit to display*/
	spi_master_transmit(c);
	_delay_us(30);
	
	PORTB |= (1<<SS_DISP);
	return 0;
}

void display_move_cursor(int steps)
{
	SPCR |= (1<<SPR0);
	SPCR &=~ (1<<SPR1);
	PORTC &=~ (1<<RS_DISP);
	PORTB &=~ (1<<SS_DISP);
	for (int i = 0; i < steps; i++)
	{
		spi_master_transmit(0b00010100);
		_delay_us(30);
	}
	PORTC |= (1<<RS_DISP);
	PORTB |= (1<<SS_DISP);
}

void display_position_cursor(int position)
{
	SPCR |= (1<<SPR0);
	SPCR &=~ (1<<SPR1);
	PORTC &=~ (1<<RS_DISP);
	PORTB &=~ (1<<SS_DISP);
	spi_master_transmit(0b00000010);
	_delay_us(30);
	for (int i = 0; i < position; i++)
	{
		spi_master_transmit(0b00010100);
		_delay_us(30);
	}
	PORTC |= (1<<RS_DISP);
	PORTB |= (1<<SS_DISP);
}

void display_clear()
{
	SPCR |= (1<<SPR0);
	SPCR &=~ (1<<SPR1);
	PORTC &=~ (1<<RS_DISP);
	PORTB &=~ (1<<SS_DISP);
	spi_master_transmit(0x01);
	_delay_ms(4);
	PORTC |= (1<<RS_DISP);
	PORTB |= (1<<SS_DISP);
}

ISR(INT0_vect)
{
	i++;
	if (i == 51)
	{
		i = 0;
		/*if (PORTC & (1<<PORTC0))
		{
			PORTC &=~ (1<<PORTC0);
		}
		else
		{
			PORTC |= (1<<PORTC0);
		}*/
		rps = timer_counter;
		timer_counter = 0;
	}
}

ISR(TIMER2_COMPA_vect)
{
	timer_counter++;
	song_timer++;
	if (song_timer == 10)
	{
		song_play();
		song_timer = 0;
	}
}

ISR(USART_RX_vect){
	line_buffer[line_ind] = UDR0;
	if(line_buffer[line_ind] == '\n'){
		line_buffer[line_ind] = '\0';
		//Do something
		
		if(line_buffer[2] == 'l' || line_buffer[2] == 'L') PORTC ^= (1<<PORTC0);
		if(line_buffer[2] == 'm' || line_buffer[2] == 'M') song_start(mario);
		if(line_buffer[2] == 'a' || line_buffer[2] == 'A') song_start(allstars);
		if(line_buffer[2] == 'd' || line_buffer[2] == 'D')
		{
			PORTB |= (1<<PORTB6);
			PORTB &=~ (1<<PORTB7);
		};
		if(line_buffer[2] == 's' || line_buffer[2] == 'S')
		{
			PORTB &=~ (1<<PORTB6);
			PORTB &=~ (1<<PORTB7);
		};
		if(line_buffer[2] == 'r' || line_buffer[2] == 'R')
		{
			PORTB &=~ (1<<PORTB6);
			PORTB |= (1<<PORTB7);
		};
		if(line_buffer[2] == 'h' || line_buffer[2] == 'H')
		{
			OCR0A += 0x11;
			OCR0B += 0x11;
		};
		if(line_buffer[2] == 'l' || line_buffer[2] == 'L')
		{
			OCR0A -= 0x11;
			OCR0B -= 0x11;
		};
		if(line_buffer[2] == 'v' || line_buffer[2] == 'V')
		{
			OCR0A = line_buffer[4];
			OCR0B = line_buffer[4];
		};
		line_ind = 0;
		} else{
		line_ind++;
	}
}