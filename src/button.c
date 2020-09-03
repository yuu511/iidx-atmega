#include <button.h>

#define TIMER_VAL 23
#define BIT_CHECK(variable,pos)      ((variable) & (1 << pos))
#define BIT_CLEAR(variable,pos)      variable &= ~(1 << pos) 
#define BIT_SET(variable,pos)        variable |= (1 << pos)


void setupDButtons ( PORTD_BUTTONS *b ) 
{
  DDRD &= ~(b->mask);
  PORTD |= b->mask;

  for (int i = 0; i < 4; ++i) {
    if ( BIT_CHECK(b->mask,i) )
      BIT_SET(DDRB,b->B_LEDS[i]);
  }

  for (int i = 0; i < 4; ++i) {
    if ( BIT_CHECK(b->mask,i+4) ) 
      BIT_SET(DDRF,b->F_LEDS[i]);
  }

}

uint8_t getDState( PORTD_BUTTONS *b, uint64_t currentTime ) 
{
  uint8_t currentState = PIND & b->mask;
  
  for ( int i = 0; i < 8; ++i ) {
    if ( b->isDebouncing & (1<<i) ) {
      if ( (currentTime - b->last_pressed[i]) >= TIMER_VAL ) {
       if ( BIT_CHECK(currentState,i) ) {
         BIT_CLEAR (b->state,i);
       }
       else {
         BIT_SET (b->state,i);
       }
        b->isDebouncing &= ~(1<<i);
      }
    }
    else if ( (currentState & ( 1<< i)) != ((b->state) & ( 1 <<i))  ) {
      b->isDebouncing |= ( 1 << i );
      b->last_pressed[i] = currentTime;
    }
  }

  return b->state;
}

void LED_D_toggle ( PORTD_BUTTONS *b ) 
{
  for (int i = 0; i < 4; ++i) {
    if ( b->state & ( 1<<i) ){
      PORTB |= (1 << b->B_LEDS[i]);
    }
    else {
      PORTB &= ~(1 << b->B_LEDS[i]);
    }
  }

  for (int i = 0; i < 4; ++i) {
    if ( b->state & ( 1<< (i+4)) ){
      PORTF |= (1 << b->F_LEDS[i]);
    }
    else {
      PORTF &= ~(1 << b->F_LEDS[i]);
    }
  }

}
