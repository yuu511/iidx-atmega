#include "encoder.h"

#define START 0x0
#define CW_START 0x1
#define CCW_START 0x02
#define CW_MID 0x03
#define CCW_MID 0x04
#define CW_END  0x05
#define CCW_END  0x06

#define EMIT_CW 0x08
#define EMIT_CCW 0x10

static uint8_t state_table[7][4] = {
  { START            , EMIT_CW  | CW_START , EMIT_CCW | CCW_START , START                }, //START
  { EMIT_CCW | START , CW_START            , START                , EMIT_CW  | CW_MID    }, //CW_START
  { EMIT_CW  | START , START               , CCW_START            , EMIT_CCW | CCW_MID   }, //CCW_START
  { START            , EMIT_CCW | CCW_END  , EMIT_CW | CW_END     , CW_MID               }, //CW_MID
  { START            , EMIT_CCW | CCW_END  , EMIT_CW | CW_END     , CCW_MID              }, //CCW_MID
  { EMIT_CW  | START , START               , CW_END               , EMIT_CCW | CCW_MID   }, //CW_END
  { EMIT_CCW | START , CCW_END             , START                , EMIT_CW  | CW_MID    } //CCW_END
};

static uint8_t state = 0;
static uint8_t index = 0;

void setupEncoder(void)
{
  BIT_CLEAR(DDRB,PB4);
  BIT_SET(PORTB,PB4);

  BIT_CLEAR(DDRB,PB5);
  BIT_SET(PORTB,PB5);
  
  BIT_SET(PCICR, PCIE0);
  PCMSK0 |= (PCINT4 | PCINT5);
}

// 0 = no change, 0x08 = CW, 0x10, CCW
uint8_t checkEncoderOutputs(void)
{
  return state;
}

ISR(PCINT0_vect) {
  uint8_t new_state = state_table[index][( (BIT_CHECK(~PINB,PB4) >> 4) | (BIT_CHECK(~PINB,PB5) >> 4) )];
  index = new_state & 0x7;
  state = new_state & ~(0x7);
}
