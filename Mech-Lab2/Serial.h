/*
 * Serial.h
 *
 * Created: 2/12/2020 12:13:50 PM
 * Updated 2/12/2020 2:15:00 PM
 *  Authors: Anna Corman, Joslyne Lovelace, Megan Shapiro
 */ 


void USART_Init(uint8_t);
uint8_t receive_byte();
float receive_float();
void print_byte(uint8_t);
void print_float(float);
uint8_t newMsg();
