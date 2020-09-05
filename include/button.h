#ifndef __BUTTON_H_
#define __BUTTON_H_

#include <avr/io.h>

#define GBUTTON1     0b00000001
#define GBUTTON2     0b00000010
#define GBUTTON3     0b00000100
#define GBUTTON4     0b00001000
#define GBUTTON5     0b00010000
#define GBUTTON6     0b01000000
#define GBUTTON7     0b10000000
#define MBUTTONSTART 0b00000001
#define MBUTTONVEFX  0b00000010

typedef struct {
  uint8_t mask;
  uint8_t state;
  uint8_t isDebouncing;
  uint8_t B_LEDS[4];
  uint8_t F_LEDS[4];
  uint64_t last_pressed[8];
} GAMEPLAY_BUTTONS;

void setupGameplayButtons(void);

void setupGameplayLEDs(void);

uint8_t gameplayButtonState(uint64_t currentTime);

void setupMetaButtons(void);

uint8_t metaButtonState(void);

#endif
