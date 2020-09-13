#ifndef __ENCODER_H_
#define __ENCODER_H_

#include "button.h"
#include <avr/interrupt.h>

void setupEncoder(void);
uint8_t checkEncoderOutputs(void);
void resetState(void);

#endif
