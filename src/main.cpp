#include <avr/io.h>
#include <util/delay.h>
#include "iidx_state.h"

typedef struct {
  uint16_t button_state;
  uint8_t  tt_state;
} input_state;

#define IN_PIN_PULLUP(PortNum,PinNum) \
  DDR##PortNum &= ~( 1 << DD##PortNum##PinNum ); \
  PORT##PortNum |= ( 1 << PORT##PortNum##PinNum) ; \

#define OUT_PIN(PortNum,PinNum) \
  DDR##PortNum |=  ( 1 << DD##PortNum##PinNum ); \

inline void SetPinModes()
{
  IN_PIN_PULLUP(D,1);
  IN_PIN_PULLUP(D,0);
  IN_PIN_PULLUP(D,4);
  IN_PIN_PULLUP(C,6);
  IN_PIN_PULLUP(D,7);
  IN_PIN_PULLUP(B,4);
  IN_PIN_PULLUP(B,5);
  IN_PIN_PULLUP(B,6);
  OUT_PIN(C,7);
}

 
int main()
{
  SetPinModes();
  for (;;) {
  }
}
