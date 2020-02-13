/*
 * Mech-Lab2.c
 *
 * Created: 2/12/2020 1:14:53 PM
 * Author : Megan Shapiro
 */ 

#define F_CPU 16000000L //Clock speed of Arduino 16 MHz
#define BAUD 9600       //Desired Baud Rate of Serial Communication
#define MYUBRR F_CPU/16/BAUD-1 //Calculated value of UBRR to initialize USART

#include <util/delay.h>
#include <avr/io.h>
#include "Ring_Buffer.h"
#include "Serial.h"



int main(void)
{
	struct Ring_Buffer_F input_queue;
	struct Ring_Buffer_F output_queue;
	USART_Init(MYUBRR);
	rb_initialize_F(&input_queue);
    rb_initialize_F(&output_queue);
	
	/* Replace with your application code */
    while (1) 
    {
    }
}

void timer0_init()
{	
	// timer with no prescaler, we will need one for 16MHz but not sure which yet
	// need to add CTC information but not sure which register this is for Timer0
	TCCR0 |= (1 << CS00);
	
	// initialize counter to zero
	TCNT0 = 0;
	
	// initialize compare value for CTC
	OCR1A = 
}

void timer1_init()
{
	// timer with no prescaler
	// need to add setup for CTC
	TCCR1B |= (1 << CS10);
	
	// initialize counter to zero
	TCNT2 = 0;
}
