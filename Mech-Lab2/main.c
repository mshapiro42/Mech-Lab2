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
#include <avr/interrupt.h>
#include "Serial.h"
#include "Ring_Buffer.h"
#include "Digital_Filter.h"

uint8_t filtInit = 0;

void timer0_init();
void timer1_init();

int main(void)
{
	struct Ring_Buffer_F input_queue;
	struct Ring_Buffer_F output_queue;
	USART_Init(MYUBRR);
	rb_initialize_F(&input_queue);
    rb_initialize_F(&output_queue);

	timer0_init();
	timer1_init();
	
	//Set AI0 to Output and rest as Input
	DDRC = 0b10000000;
	//Set output to 1 to power sensor
	PORTC = 0b10000000;
	
	//Sampling period for converting to velocity, 1 over since we divide by the sampling period
	//float sampPer = 1/0.001;
	float volt, angPos;
	//float angVel = 0;

    while (1) 
    {
		//if TIMER0_flag
		if(TIFR0 & (1 << OCF0A))
		{
			//dequeue output
			float output = rb_pop_front_F(&output_queue);
			//print_float
			print_float(output);
			//reset TIMER0_flag
			TIFR0 |= (1 << OCF0A);
		}
		//if TIMER1_flag
		if(TIFR1 & (1 << OCF1A))
		{
			//collect input
			volt = PINC1;
			
			//convert to position
			angPos = volt; //add equation to covert
			
			//convert to velocity
			//angVel = (angPos - rb_pop_front_F(&input_queue))*sampPer;
			
			/*if(!filtInit){
				digital_filter_init(angVel);
				filtInit = 1;
			}*/
			
			//add angPos to queue
			rb_push_back_F(&output_queue, angPos); //needs to change to input_queue for this is for testing
			
			//filter velocity
			//angVel = filterValue(angVel);
			
			//add to output queue
			//rb_push_back_F(&output_queue, angVel);
			
			//reset TIMER1_flag
			TIFR1 |= (1 << OCF1A);
		} 
    }
}

void timer0_init()
{	
	// enable CTC for Timer0 and prescaler of 1024
	TCCR0A |= (1 << WGM01)|(1 << CS02)|(1 << CS00);
	
	// initialize counter to zero
	TCNT0 = 0;
	
	// initialize compare value for CTC
	OCR0A = 155;
	
	// enable compare interrupt
	TIMSK0 |= (1 << OCIE0A);
}

void timer1_init()
{
	// Enable CTC for Timer1 with no prescaler
	TCCR1A |= (1 << WGM12)|(1 << CS10);
	
	// initialize counter to zero
	TCNT1 = 0;
	
	// initialize compare value
	OCR1A = 15999;
	
	// enable compare interrupt
	TIMSK1 |= (1 << OCIE1A);
}
