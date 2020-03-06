/*
 * Mech-Lab2.c
 *
 * Created: 2/12/2020 1:14:53 PM
 * Author : Megan Shapiro
 */ 

#define F_CPU 16000000L //Clock speed of Arduino 16 MHz
#define BAUD 9600       //Desired Baud Rate of Serial Communication
#define MYUBRR F_CPU/16/BAUD-1 //Calculated value of UBRR to initialize USART
#define TRANSMIT_READY UCSR0A & (1<<UDRE0)
#define TIMER0_FLAG (TIFR0 & (1 << OCF0A))
#define TIMER1_FLAG (TIFR1 & (1 << OCF1A))

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>
#include "Serial.h"
#include "Ring_Buffer.h"
#include "Digital_Filter.h"


void timer0_init();
void timer1_init();
void adc_init();
uint16_t adc_read(uint8_t ch);
float angDist(float a1, float a2);
/*
union floatChars {
	float asFloat;
	char asChars[4];
};*/

int main(void)
{
	//struct Ring_Buffer_C output_queue;
	USART_Init(MYUBRR, 0); //Send floats out as a group
	//USART_Init(MYUBRR, 1); //Send floats out as a 4 bytes
    //rb_initialize_C(&output_queue);

	timer0_init();
	timer1_init();
	adc_init();
	digital_filter_init();
	
	//Set AI0 to Output and rest as Input
	DDRC |= 0b00000001;
	
	//Set output to 1 to power sensor
	PORTC |= 0b00000001;
	
	//Sampling frequency for converting to velocity, 1/0.001
	float sampPer = 1000;
	float volt = 0;
	float angPos = 0;
	float filteredPos = 0;
	float angPosLast = 0;
	float angVel = 0;
	//union floatChars printVal;
	float convertCoeff[] = {-470.5305, 7.2116, -0.0543, 1.9698E-4, -3.5356E-7, 3.0609E-10, -1.0193E-13};
	//float convertCoeff[] = {-354.5305, 7.2116, -0.0543, 1.9698E-4, -3.5356E-7, 3.0609E-10, -1.0193E-13};
	float tempSum;
	float voltTemp = 0;

    while (1) 
    {
		//if TIMER0_flag
		if(TIMER0_FLAG)
		{
			//printVal.asFloat = 500; //edit so we don't drop readings during prints
			/*printVal.asFloat = angPos;
			for(int i = 0; i < 4; i ++){
				rb_push_back_C(&output_queue, printVal.asChars[i]);
			}*/
			print_float(angVel);
			//reset TIMER0_flag
			TIFR0 |= (1 << OCF0A);
		}
		//if TIMER1_flag
		if(TIMER1_FLAG)
		{
			//read voltage 
			volt = adc_read(1);	
			voltTemp = volt;
			//convert to position in radians
			tempSum = convertCoeff[0];
			// Apply 6th order best fit line found in Matlab
			for (int i = 1; i <=6; i++){
				tempSum += convertCoeff[i]*voltTemp;
				voltTemp *= volt;
			}
			//wrap result
			angPos = tempSum;

			//filter position
			filteredPos = filterValue(angPos);
			//convert to velocity
			if (!(filteredPos < -200 || angPosLast < -200)){
				angVel = angDist(filteredPos, angPosLast) * sampPer * .0027778; // rev/s
			}
			
			//add angPos to queue
			angPosLast = filteredPos;
			
			//reset TIMER1_flag
			TIFR1 |= (1 << OCF1A);
		} 
		/*if (rb_length_C(&output_queue) > 0 && TRANSMIT_READY){
			print_byte(rb_pop_front_C(&output_queue));
		}*/
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
	//Enable ADC w/ prescaler
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


float angDist(float a1, float a2){
	if (a1<0 && a2>0) {
		return (360 - (a2 + abs(a1)));
	} else {
		return a1-a2;
	}
}
