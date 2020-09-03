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
  setupGButtons();

  for (;;) {
    gameplayButtonState(PROGRAM_EXECUTION_TIME);
  }

  return 1;
}

ISR(TIMER1_COMPA_vect)
{
  PROGRAM_EXECUTION_TIME++;
}
