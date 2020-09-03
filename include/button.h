#ifndef __BUTTON_H_
#define __BUTTON_H_

#include <avr/io.h>

typedef struct {
  uint8_t mask;
  uint8_t state;
  uint8_t isDebouncing;
  uint8_t B_LEDS[4];
  uint8_t F_LEDS[4];
  uint64_t last_pressed[8];
} PORTD_BUTTONS;

void setupDButtons(PORTD_BUTTONS *b);

uint8_t getDState(PORTD_BUTTONS *b, uint64_t currentTime);

void LED_D_toggle(PORTD_BUTTONS *b); 

#endif
