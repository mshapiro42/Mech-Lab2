/*
 * Serial.c
 *
 * Created: 2/12/2020 12:13:50 PM
 * Updated 2/12/2020 2:15:00 PM
 *  Authors: Anna Corman, Joslyne Lovelace, Megan Shapiro
 */ 
#include "Serial.h"

union floatChars {
	float asFloat;
	char asChar[4];
	};

void USART_Init(uint8_t ubrr){ //Initialize USART for serial communication
	UBRR0H = (unsigned char)(ubrr>>8); // Split ubrr value to 2 registers, high and low
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); //Initialize USART as Input and Output
	UCSR0C = 0x06; //8 Data bits, No Parity Bit, 1 Stop bit
}

uint8_t receive_byte() { //Collect data from USART
	uint8_t value = UDR0; //Retrieve byte from register
	return value;
}

float receive_float() { //Collect float from USART
	
	union floatChars a; // create helper union instance
	for(uint8_t i = 0; i < 4; i++){ //for 4 bytes
		while(!(UCSR0A & (1<<RXC0))); //wait for new byte
		a.asChar[i] = receive_byte(); //collect byte
	}
	return a.asFloat; //return float value
}


void print_byte(uint8_t value){
	while(!(UCSR0A & (1<<UDRE0))); //after transmit line is ready
	UDR0 = value; //set transmit register to value
}

void print_float(float value){ 
	union floatChars b; //create helper union instance
	b.asFloat = value; //set float value
	for(uint8_t i = 0; i < 4; i++){ //for 4 bytes
		print_byte(b.asChar[i]); //send each byte
	}
}

uint8_t newMsg() {
	return UCSR0A & (1<<RXC0);
}