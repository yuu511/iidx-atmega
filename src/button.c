#include <button.h>
#include <iidx.h>

#define TIMER_VAL 23

#define CHECK_STATE(pinNum) \
if (BIT_CHECK(buttons.isDebouncing,pinNum)) { \
  if ( (currentTime - buttons.last_pressed[pinNum]) >= TIMER_VAL ) { \
    if (BIT_CHECK(state ,pinNum)) { \
      BIT_SET(buttons.state,pinNum); \
    } \
    else { \
      BIT_CLEAR(buttons.state,pinNum); \
    } \
    BIT_CLEAR(buttons.isDebouncing,pinNum); \
  } \
} \
else if ( BIT_CHECK(state , pinNum) != BIT_CHECK(buttons.state,pinNum) ) { \
  BIT_SET(buttons.isDebouncing,pinNum); \
  buttons.last_pressed[pinNum] = currentTime; \
} \

static GAMEPLAY_BUTTONS buttons = {
  .state =  0,
  .isDebouncing = 0,
  .last_pressed = { 0 }
};

void setupGameplayButtons (void) 
{
  DDRD &= ~(GBUTTON_D); // PD 0,1,2,3,4,6,7
  PORTD |= GBUTTON_D; 
}

void setupGameplayLEDs (void) 
{
  DDRF |= GLED_F_MASK;
  DDRB |= GLED_B_MASK;
}

uint8_t gameplayButtonState(uint64_t currentTime) 
{
  uint8_t state = ~PIND; //could of put in a loop but :^)
  CHECK_STATE (0); 
  CHECK_STATE (1);
  CHECK_STATE (2);
  CHECK_STATE (3);
  CHECK_STATE (4);
  CHECK_STATE (6);
  CHECK_STATE (7);
  return buttons.state;
}

// do not debounce START, VEFX 
void setupMetaButtons(void)
{
  DDRE &= ~(1 << PE6);
  PORTE |= (1 << PE6);

  DDRC &= ~(1 << PC7);
  PORTC |= (1 << PC7);
}

uint8_t metaButtonState(void)
{
  uint8_t metaButtonState = 0; 
  if (!BIT_CHECK(PINE,6)) {
    metaButtonState |= 1;  
  }
  if (!BIT_CHECK(PINC,7)) {
    metaButtonState |= 1 << 1;  
  }
  return metaButtonState;
}
