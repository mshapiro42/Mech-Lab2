/*
* 
* Digital_Filter.h
* 
* Created : 2/23/20 1:15pm
* Author : Megan Shapiro
* 
*/


#ifndef DIGITAL_FILTER_H
#define DIGITAL_FILTER_H

#include "stdint.h"

#define order 4

/******* Functions ********/

//Initialize our digital filter and its variables
void digital_filter_init(float reading);

// Return the filtered value
float filterValue(float angularVelocity);