#include <button.h>
#include <iidx.h>

#define TIMER_VAL 23

#define CHECK_STATE(PortLetter, pinNum) \
if (BIT_CHECK(buttons.isDebouncing,index)) { \
  if ( (currentTime - buttons.last_pressed[index]) >= TIMER_VAL ) { \
    if (!BIT_CHECK(PIN##PortLetter ,pinNum)) { \
      BIT_SET(buttons.state,index); \
    } \
    else { \
      BIT_CLEAR(buttons.state,index); \
    } \
    BIT_CLEAR(buttons.isDebouncing,index); \
  } \
} \
else if ( BIT_CHECK(~PIN##PortLetter , pinNum) != BIT_CHECK(buttons.state,index) ) { \
  BIT_SET(buttons.isDebouncing,index); \
  buttons.last_pressed[index] = currentTime; \
} \
index++;


static GAMEPLAY_BUTTONS buttons = {
  .state =  0,
  .isDebouncing = 0,
  .last_pressed = { 0 }
};

void setupGameplayButtons (void) 
{
  DDRD &= ~(GBUTTON_D); // PD 2,3,4,6,7
  PORTD |= GBUTTON_D; 

  DDRB &= ~(GBUTTON_B); // PB 4,5
  PORTB |= GBUTTON_B;
}

void setupGameplayLEDs (void) 
{
  DDRF |= GLED_F_MASK;
  DDRB |= GLED_B_MASK;
}

uint8_t gameplayButtonState(uint64_t currentTime) 
{
  uint8_t index = 0;
  CHECK_STATE (D, 2);
  CHECK_STATE (D, 3);
  CHECK_STATE (D, 4);
  CHECK_STATE (D, 6);
  CHECK_STATE (D, 7);
  CHECK_STATE (B, 4);
  CHECK_STATE (B, 5);
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
