/*
*
* Dig_Filter.c
*
* Created: 2/21/2020
* Authors: Anna Corman, Joslyne Lovelace, Megan Shapiro
*/
#include <avr/io.h>
#include "Dig_Filter.h"
#include "Ring_Buffer.h"

void ini_filter() 
{
	rb_initialize_F(&input_queue);
	rb_initialize_F(&output_queue);
	return;
}

float filterValue(float input)
{
	//define coefficients
	float b = [0.0 1.0 2.0];
	float a = [1 2.0 3.0 4.0];
	//initialize indices and angVel
	int i;
	int j;
	float angVel;
	//add input to input queue
	rb_push_front_F(&input_queue,input);
	//loop to perform transfer function
	for(i = 0; i < sizeof(b); i++)
	{
		angVel += input_queue[i]*b[i];
	}
	for(j = 1; j < sizeof(a)-1; j++)
	{
		angVel -= output_queue[j]*a[j];
	}
	angVel *= a[0];
	//add result to output queue
	rb_push_front_F(&output_queue,angVel);
	//return result
	return angVel;
}