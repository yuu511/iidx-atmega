#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <button.h>
 
uint64_t PROGRAM_EXECUTION_TIME = 0;

// timer 1 : Increments every 0.1 ms.
void setup_timer() 
{
  TCCR1B |= ( 1 << WGM12) ; 
  OCR1A = 24;
  TIMSK1 |= ( 1 << OCIE1A);
  TCCR1B |= ( 1 << CS10) | ( 1 << CS11);
}

int main(void) 
{
  setup_timer();
  sei();

  PORTD_BUTTONS buttons = {
    .mask = 0b11011111,
    .state =  0,
    .isDebouncing = 0,
    .B_LEDS = { PB4, PB5, PB6, PB7 },
    .F_LEDS = { PF0, PF1, PF4, PF5 },
    .last_pressed = { 0 }
  };
  setupDButtons(&buttons);

  for (;;) {
    getDState(&buttons,PROGRAM_EXECUTION_TIME);
    LED_D_toggle(&buttons);
  }

  return 1;
}

ISR(TIMER1_COMPA_vect)
{
  PROGRAM_EXECUTION_TIME++;
}
