#include "encoder.h"

#define START 0x0
#define CW_START 0x1
#define CCW_START 0x02
#define MID 0x3
#define CW_END  0x04
#define CCW_END  0x05

#define EMIT_CW 0x08
#define EMIT_CCW 0x10

static uint8_t state_table[6][4] = {
  { START            , EMIT_CW | CW_START  , EMIT_CCW | CCW_START , START             }, //START
  { START            , CW_START            , START                , EMIT_CW  | MID    }, //CW_START
  { START            , START               , CCW_START            , EMIT_CCW | MID    }, //CCW_START
  { START            , EMIT_CCW | CCW_END  , EMIT_CW | CW_END     , MID               }, //MID
  { EMIT_CW | START  , START               , CW_END               , EMIT_CCW | MID    }, //CW_END
  { EMIT_CCW | START , CCW_END             , START                , EMIT_CW | MID     } //CCW_END
};

static volatile uint8_t state = 0;

void setupEncoder(void)
{
  BIT_CLEAR(DDRB,PB4);
  BIT_SET(PORTB,PB4);

  BIT_CLEAR(DDRB,PB5);
  BIT_SET(PORTB,PB5);

  BIT_SET(PCICR, PCIE0);
  BIT_SET(PCICR, PCIE0);
  PCMSK0 |= ((1 << PCINT4) | (1 << PCINT5));

  state = state_table[0][( (BIT_CHECK(~PINB,PB4) >> 4) | (BIT_CHECK(~PINB,PB5) >> 4) )];
}

// 0x08 = CW, 0x10, CCW
uint8_t checkEncoderOutputs(void)
{
  uint8_t ret = state;
  state &= 0x7;
  return ret;
}

ISR(PCINT0_vect) 
{
   state = state_table [state & 0x7][( (BIT_CHECK(~PINB,PB4) >> 4) | (BIT_CHECK(~PINB,PB5) >> 4) )];
}
