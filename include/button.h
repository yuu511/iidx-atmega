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

#define GBUTTON_D 0b11011111 // PD 0,1,2,3,4,6,7

#define GLED1  PF5
#define GLED2  PF4
#define GLED3  PF0
#define GLED4  PF1
#define GLED5  PF6
#define GLED6  PB7
#define GLED7  PB6
#define GLED1_PORT PORTF
#define GLED2_PORT PORTF
#define GLED3_PORT PORTF
#define GLED4_PORT PORTF
#define GLED5_PORT PORTF
#define GLED6_PORT PORTB
#define GLED7_PORT PORTB

#define GLED_F_MASK (1<<GLED1) | (1<<GLED2) | (1<<GLED3) | (1<<GLED4) | (1<<GLED5)
#define GLED_B_MASK (1<<GLED6) | (1<<GLED7)

#define TURN_ON(ledname) BIT_SET(ledname##_PORT , ledname);
#define TURN_OFF(ledname) BIT_CLEAR(ledname##_PORT, ledname);

#define BIT_CHECK(variable,pos)      ((variable) & (1 << pos))
#define BIT_CLEAR(variable,pos)      variable &= ~(1 << pos) 
#define BIT_SET(variable,pos)        variable |= (1 << pos)

typedef struct {
  uint8_t state;
  uint8_t isDebouncing;
  uint64_t last_pressed[8];
} GAMEPLAY_BUTTONS;

void setupGameplayButtons(void);

void setupGameplayLEDs(void);

uint8_t gameplayButtonState(uint64_t currentTime);

void setupMetaButtons(void);

uint8_t metaButtonState(void);

#endif
