#include "Ring_Buffer.h"

// define constant masks for use later based on length chosen
// these are global scope only to this c file
const uint8_t RB_MASK_F = RB_LENGTH_F-1;
const uint8_t RB_MASK_C = RB_LENGTH_C-1; 


/* Initialization */
void rb_initialize_F( struct Ring_Buffer_F* p_buf )
{
    // set start and end indicies to 0
    // no point changing data
    p_buf->start_index = 0;
    p_buf->end_index = 0;
}

void rb_initialize_C( struct Ring_Buffer_C* p_buf )
{
    // set start and end indicies to 0
    // no point changing data
    p_buf->start_index = 0;
    p_buf->end_index = 0;
}


/* Return active Length of Buffer */
uint8_t rb_length_F( const struct Ring_Buffer_F* p_buf)
{
    // calculate the active length using the mask and 2's complement to help
    // verify for your self why this works!
    uint8_t length = (p_buf->end_index - p_buf->start_index) & RB_MASK_F;
    return length;
}
uint8_t rb_length_C( const struct Ring_Buffer_C* p_buf)
{
    uint8_t length = (p_buf->end_index - p_buf->start_index) & RB_MASK_C;
    // make sure to use the correct mask!
    return length;
    
}

/* Append element to end and lengthen */
void rb_push_back_F( struct Ring_Buffer_F* p_buf, float value)
{   

    // Increment the end index and wrap using the mask.
    // If the end equals the start increment the start index
    // Set the value at the end index as desired.
    p_buf->buffer[p_buf->end_index] = value;
    p_buf->end_index = (p_buf->end_index+1) & RB_MASK_F;
    if (p_buf->end_index == p_buf->start_index){
        p_buf->start_index = (p_buf->start_index + 1) & RB_MASK_F;
    }
    return;
}
void rb_push_back_C( struct Ring_Buffer_C* p_buf, char value)
{
    // Increment the end index and wrap using the mask.
    // If the end equals the start increment the start index`
    // Set the value at the end index as desired.
    p_buf->buffer[p_buf->end_index] = value;
    p_buf->end_index = (p_buf->end_index + 1) & RB_MASK_C;
    if (p_buf->end_index == p_buf->start_index){
        p_buf->start_index = (p_buf->start_index + 1) & RB_MASK_C;
    }
    return;
}

/* Append element to front and lengthen */
void rb_push_front_F( struct Ring_Buffer_F* p_buf, float value)
{
    // Decrement the start index and wrap using the mask.
    // If the end equals the start decrement the end index`
    // Set the value at the start index as desired.
    p_buf->start_index = (p_buf->start_index - 1) & RB_MASK_F;
    if (p_buf->start_index == p_buf->end_index){
        p_buf->end_index = (p_buf->end_index - 1) & RB_MASK_F;
    }
    p_buf->buffer[p_buf->start_index] = value;
    return;
}
void rb_push_front_C( struct Ring_Buffer_C* p_buf, char value)
{
    // Decrement the start index and wrap using the mask.
    // If the end equals the start decrement the end index`
    // Set the value at the start index as desired.
        p_buf->start_index = (p_buf->start_index - 1) & RB_MASK_C;
    if (p_buf->start_index == p_buf->end_index){
        p_buf->end_index = (p_buf->end_index - 1) & RB_MASK_C;
    }
    p_buf->buffer[p_buf->start_index] = value;
    return;
}

/* Remove element from end and shorten */
float rb_pop_back_F( struct Ring_Buffer_F* p_buf)
{
    // get value to return at end
    // if end does not equal start (length zero),
    //    reduce end index by 1 and mask
    // return value
    float value = p_buf->buffer[p_buf->end_index - 1];
    if (p_buf->end_index != p_buf->start_index){
        p_buf->end_index = (p_buf->end_index - 1) & RB_MASK_F;
        return value; // update
    } 
    else {
        return 0;
    }
    
}
char  rb_pop_back_C( struct Ring_Buffer_C* p_buf)
{
    // get value to return at end
    // if end does not equal start (length zero),
    //    reduce end index by 1 and mask
    // return value
    char value = p_buf->buffer[p_buf->end_index];
    if (p_buf->end_index != p_buf->start_index){
        p_buf->end_index = (p_buf->end_index - 1) & RB_MASK_C;
        return value; // update
    } 
    else {
        return 0;
    }
   
}

/* Remove element from start and shorten */
float rb_pop_front_F( struct Ring_Buffer_F* p_buf)
{
    // get value to return at front
    // if end does not equal start (length zero),
    //    increase start index by 1 and mask
    // return value
    float value = p_buf->buffer[p_buf->start_index];
    if(p_buf->start_index != p_buf->end_index){
        p_buf->start_index = (p_buf->start_index + 1) & RB_MASK_F;
        return value; // update
    }
    else {
        return 0;
    }
    
}
char  rb_pop_front_C( struct Ring_Buffer_C* p_buf)
{
    // get value to return at front
    // if end does not equal start (length zero),
    //    increase start index by 1 and mask
    // return value
    char value = p_buf->buffer[p_buf->start_index];
    if(p_buf->start_index != p_buf->end_index){
        p_buf->start_index = (p_buf->start_index + 1) & RB_MASK_F;
        return value; // update
    } 
    else {
        return 0;
    }
   
}

/* access element */
float rb_get_F( const struct Ring_Buffer_F* p_buf, uint8_t index)
{
    // return value at start + index wrapped properly
    if (p_buf->start_index != p_buf->end_index){ //if buffer is not empty
        index = (p_buf->start_index + index) & RB_MASK_F; //wrap to active length
        return p_buf->buffer[index]; // update
    }
    else{
        return 0;
    }
    
}

char  rb_get_C( const struct Ring_Buffer_C* p_buf, uint8_t index)
{
    // return value at start + index wrapped properly
    if (p_buf->start_index != p_buf->end_index){
        index = (p_buf->start_index + index) & RB_MASK_C; //wrap to active length
        return p_buf->buffer[index]; // update
    }
    else{
        return 0;
    }
}

/* set element - This behavior is 
   poorly defined if index is outside of active length.
   Use of the push_back or push_front methods are prefered.
*/
void  rb_set_F( struct Ring_Buffer_F* p_buf, uint8_t index, float value)
{
    // set value at start + index wrapped properly
    index = (p_buf->start_index + index) & RB_MASK_F;
    p_buf->buffer[index] = value;
    return;
}
void  rb_set_C( struct Ring_Buffer_C* p_buf, uint8_t index, char value)
{
    // set value at start + index wrapped properly
    index = (p_buf->start_index + index) & RB_MASK_C;
    p_buf->buffer[index] = value;
    return;
}

