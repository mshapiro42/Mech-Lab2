#include "Digital_Filter.h"
#include "Ring_Buffer.h"

struct Ring_Buffer_F inputs;
struct Ring_Buffer_C outputs;

//a[0] should be entered as the inverse of the coeff, for computation speed
float a[order] = {1, 1, 1, 1};
float b[order] = {1, 1, 1, 1};
uint8_t i;

/* Initialization */
void digital_filter_init(float reading){
	rb_initialize_F(&inputs);
	rb_initialize_F(&outputs);
	
	for(i = 0; i < order; i++){
		rb_push_front_F(&inputs, reading);
		rb_push_front_F(&outputs, reading);
	}
}

float filterValue(float angularVelocity){
	rb_pop_back_F(&inputs);
	rb_push_front_F(angularVelocity);
	float angVel = 0.0;
	for(i=0; i < order; i++){
		angVel += b[i]*rb_get_F(&inputs,i);
		if (i>0){
			angVel -= a[i]*rb_get_F(&outputs,i);
		}
	}
	angVel *= a[0];
	rb_pop_back_F(&outputs);
	rb_push_front_F(&outputs, angVel);
	return angVel;
}