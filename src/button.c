#include <button.h>
#include <iidx.h>

#define TIMER_VAL 23
#define BIT_CHECK(variable,pos)      ((variable) & (1 << pos))
#define BIT_CLEAR(variable,pos)      variable &= ~(1 << pos) 
#define BIT_SET(variable,pos)        variable |= (1 << pos)

static GAMEPLAY_BUTTONS buttons = {
  .mask = 0b11011111, // all gameplay buttons are on port D.
  .state =  0,
  .isDebouncing = 0,
  .B_LEDS = { PB4, PB5, PB6, PB7 }, // PD0-3
  .F_LEDS = { PF0, PF1, PF4, PF5 }, // PD4-7
  .last_pressed = { 0 }
};

void setupGameplayButtons (void) 
{
  DDRD &= ~(buttons.mask);
  PORTD |= buttons.mask;
}

void setupGameplayLEDs (void) 
{
  for (int i = 0; i < 4; ++i) {
    if ( BIT_CHECK(buttons.mask,i) )
      BIT_SET(DDRB,buttons.B_LEDS[i]);
  }

  for (int i = 0; i < 4; ++i) {
    if ( BIT_CHECK(buttons.mask,(i+4)) ) 
      BIT_SET(DDRF,buttons.F_LEDS[i]);
  }
}

uint8_t gameplayButtonState(uint64_t currentTime) 
{
  uint8_t currentState = ~PIND & buttons.mask;
  
  for ( int i = 0; i < 4; ++i ) {
    if ( BIT_CHECK(buttons.isDebouncing,i) ) {
      if ( (currentTime - buttons.last_pressed[i]) >= TIMER_VAL ) {
       buttons.state = ( (buttons.state & ~(1<<i)) | (BIT_CHECK(currentState,i)) ); 
       BIT_CLEAR(buttons.isDebouncing,i);
      }
    }
    else if ( BIT_CHECK(currentState,i) != BIT_CHECK(buttons.state,i) ) {
      BIT_SET(buttons.isDebouncing,i);
      buttons.last_pressed[i] = currentTime;
    }

    if ( BIT_CHECK(buttons.state,i) ) {
      BIT_SET(PORTB,buttons.B_LEDS[i]);
    }
    else {
      BIT_CLEAR(PORTB,buttons.B_LEDS[i]);
    }
  }


  for ( int i = 4; i < 8; ++i ) {
    if ( BIT_CHECK(buttons.isDebouncing,i) ) {
      if ( (currentTime - buttons.last_pressed[i]) >= TIMER_VAL ) {
       buttons.state = ( (buttons.state & ~(1<<i)) | (BIT_CHECK(currentState,i)) ); 
       BIT_CLEAR(buttons.isDebouncing,i);
      }
    }
    else if ( BIT_CHECK(currentState,i) != BIT_CHECK(buttons.state,i) ) {
      BIT_SET(buttons.isDebouncing,i);
      buttons.last_pressed[i] = currentTime;
    }

    if ( BIT_CHECK(buttons.state,(i)) ) {
      BIT_SET(PORTF,buttons.F_LEDS[i-4]);
    }
    else {
      BIT_CLEAR(PORTF,buttons.F_LEDS[i-4]);
    }
  }

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
