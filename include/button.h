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
} GAMEPLAY_BUTTONS;

void setupGButtons(void);

uint8_t gameplayButtonState(uint64_t currentTime);

#endif
