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
#include <stdlib.h>
#include "Serial.h"
#include "Ring_Buffer.h"
#include "Digital_Filter.h"
/*
union floatChar(){
	float asFloat;
	char asChars[4];
};*/

uint8_t filtInit = 0;

void timer0_init();
void timer1_init();
void adc_init();
uint16_t adc_read(uint8_t ch);


int main(void)
{
	struct Ring_Buffer_F input_queue;
	struct Ring_Buffer_C output_queue;
	USART_Init(MYUBRR);
	rb_initialize_F(&input_queue);
    rb_initialize_C(&output_queue);
	//union floatChar filteredVel;

	timer0_init();
	timer1_init();
	adc_init();
	
	//Set AI0 to Output and rest as Input
	DDRC |= 0b00000001;
	
	//Set output to 1 to power sensor
	PORTC |= 0b00000001;
	
	//Sampling frequency for converting to velocity, 1/0.001
	float sampPer = 1000;
	float volt = 0;
	float angPos = 0;
	float angPosLast = 0;
	float angVel = 0;
	float filteredVel = 0;
    while (1) 
    {
		//if TIMER0_flag
		if((TIFR0 & (1 << OCF0A)))
		{
			/*for(int i=0; i <4; i++){
				rb_push_back_C(&output_queue, filteredVel.asChars[i]);
			}*/
			print_float(filteredVel); //edit so we don't drop readings during prints
			//reset TIMER0_flag
			TIFR0 |= (1 << OCF0A);
		}
		//if TIMER1_flag
		if((TIFR1 & (1 << OCF1A)))
		{
			//read voltage 
			volt = adc_read(1);	
			//convert to position in radians
			//inefficient
			angPos = abs((-1.0193E-13)*(pow(volt,6)) + (3.0609E-10)*(pow(volt,5)) + (-3.5356E-7)*(pow(volt,4)) + (1.9698E-4)*(pow(volt,3)) + -0.0543*(pow(volt,2)) + 7.2116*volt + (-354.5305)); 
			//wrap result

			//convert to velocity
			// handle wrapping
			angVel = (angPos - angPosLast) *0.00277778*sampPer; // rev/s
			
			if(!filtInit){
				digital_filter_init(angVel);
				filtInit = 1;
			}
			
			//add angPos to queue
			angPosLast = angPos;
			
			//filter velocity
			//filteredVel.asFloat = filterValue(angVel);
			filteredVel = filterValue(angVel);
				
			//reset TIMER1_flag
			TIFR1 |= (1 << OCF1A);
		} 
		if (rb_length_C(&output_queue) > 0){
			print_byte(rb_pop_front_C(&output_queue));
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
	
	//Set reference to built in channels
	ADMUX = (1<<REFS0);
	//Enable ADC w/  prescaler
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
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