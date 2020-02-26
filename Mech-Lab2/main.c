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
#include <math.h>
#include "Serial.h"
#include "Ring_Buffer.h"
#include "Digital_Filter.h"

uint8_t filtInit = 0;

void timer0_init();
void timer1_init();
void adc_init();
uint16_t adc_read(uint8_t ch);

int main(void)
{
	struct Ring_Buffer_F input_queue;
	struct Ring_Buffer_F output_queue;
	USART_Init(MYUBRR);
	rb_initialize_F(&input_queue);
    rb_initialize_F(&output_queue);

	timer0_init();
	timer1_init();
	adc_init();
	
	//Set AI0 to Output and rest as Input
	DDRC |= 0b00000001;
	
	//Set output to 1 to power sensor
	PORTC |= 0b00000001;
	
	//Sampling frequency for converting to velocity, 1/0.001
	float sampPer = 1000;
	float volt, angPos;
	float angPosLast = 0;
	float angVel = 0;

    while (1) 
    {
		//print_byte('.');
		//if TIMER0_flag
		if((TIFR0 & (1 << OCF0A)))
		{		
			//dequeue output
			float output = rb_pop_front_F(&output_queue);			
			//print_float*/
			print_float(output);
			//reset TIMER0_flag
			TIFR0 |= (1 << OCF0A);
		}
		//if TIMER1_flag
		if((TIFR1 & (1 << OCF1B)))
		{
			//read voltage 
			volt = adc_read(1);	
			//convert to position in radians
			angPos = (-1.347E-13)*(pow(volt,6)) + (4.0362E-10)*(pow(volt,5)) + (-4.6747E-7)*(pow(volt,4)) + (2.6326E-4)*(pow(volt,3)) + -0.0743*(pow(volt,2)) + 10.1678*volt + (-645.4082 + 120); 
			//wrap result

			//convert to velocity
			//angVel = (angPos - rb_pop_front_F(&input_queue))*sampPer*0.16667;
			angVel = (angPos - angPosLast);
			angVel =
			angVel *= sampPer*0.16667;
			
			if(!filtInit){
				digital_filter_init(angVel);
				filtInit = 1;
			}
			
			//add angPos to queue
			angPosLast = angPos;
			rb_push_back_F(&input_queue, angPos); //needs to change to input_queue for this is for testing
			
			//filter velocity
			angVel = filterValue(angVel);
				
			//add velocity to output queue
			rb_push_back_F(&output_queue, angVel);			
		
			//reset TIMER1_flag
			TIFR1 |= (1 << OCF1A);
		} 
    }
}

void timer0_init()
{	
	// enable CTC for Timer0
	TCCR0A |= (1 << WGM01);
	// enable prescaler of 1024 for Timer0
	TCCR0B |= (1 << CS02)|(1 << CS00);	
	// initialize counter to zero
	TCNT0 = 0;	
	// initialize compare value for CTC
	OCR0A = 155;
}

void timer1_init()
{
	TCCR1A |= 0;
	// Enable CTC for Timer1 with no prescaler
	TCCR1B |= (1 << WGM12)|(1 << CS10);
	// initialize counter to zero
	TCNT1 = 0;	
	// initialize compare value
	OCR1B = 15999;
}


void adc_init() {
	
	//Set reference to built in channels, set MUX to ADC1 to read from AI1
	//ADMUX = (1<<REFS0)|(1<<MUX0);
	ADMUX = (1<<REFS0);
	//Enable ADC w/ auto-trigger
	//ADCSRA = (1<<ADEN)|(1<<ADATE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	//Set auto-trigger source to timer1 compare match
	//ADCSRB = (1<<ADTS2)|(1<<ADTS0);
	
	//Disable digital input buffer on ADC1, saves power
	//DIDR0 = (1<<ADC1D);
	
}

uint16_t adc_read(uint8_t ch)
{
	//select channel to read
	ch &= 0b00000111;
	ADMUX = (ADMUX & 0xF8)|ch;	
	//start conversion
	ADCSRA |= (1<<ADSC);	
	//wait for conversion to complete
	while(ADCSRA & (1<<ADSC));	
	//return result
	return (ADC);
}