#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <button.h>
 

// Button Port / Pin , LED Port / Pin 
#ifndef DEBUG_MODE 
DEFINE_BTN_FUNCTIONS (Btn1, D, 2, D, 3 );
#else 
DEFINE_BTN_FUNCTIONS (Btn1, F, 0, F, 3 );
#endif

DEFINE_BTN_FUNCTIONS (Btn2, D, 1, D, 0 );
DEFINE_BTN_FUNCTIONS (Btn3, D, 4, C, 6 );
DEFINE_BTN_FUNCTIONS (Btn4, D, 7, E, 6 );
DEFINE_BTN_FUNCTIONS (Btn5, B, 4, B, 5 );
DEFINE_BTN_FUNCTIONS (Btn6, B, 6, B, 7 );
DEFINE_BTN_FUNCTIONS (Btn7, D, 6, C, 7 );

uint64_t PROGRAM_EXECUTION_TIME = 0;


// timer 1
void setup_timer() {
  TCCR1B |= ( 1 << WGM12) ; 
  OCR1A = 24;
  TIMSK1 |= ( 1 << OCIE1A);
  TCCR1B |= ( 1 << CS10) | ( 1 << CS11);
}

int main(void) {

  Button buttons[7] = {0};

  initButton( &buttons[0], ConfigureInputBtn1, ConfigureLedBtn1, CheckPressedBtn1, TurnLedOnBtn1, TurnLedOffBtn1 );   
  initButton( &buttons[1], ConfigureInputBtn2, ConfigureLedBtn2, CheckPressedBtn2, TurnLedOnBtn2, TurnLedOffBtn2 );  
  initButton( &buttons[2], ConfigureInputBtn3, ConfigureLedBtn3, CheckPressedBtn3, TurnLedOnBtn3, TurnLedOffBtn3 );  
  initButton( &buttons[3], ConfigureInputBtn4, ConfigureLedBtn4, CheckPressedBtn4, TurnLedOnBtn4, TurnLedOffBtn4 );  
  initButton( &buttons[4], ConfigureInputBtn5, ConfigureLedBtn5, CheckPressedBtn5, TurnLedOnBtn5, TurnLedOffBtn5 );  
  initButton( &buttons[5], ConfigureInputBtn6, ConfigureLedBtn6, CheckPressedBtn6, TurnLedOnBtn6, TurnLedOffBtn6 );  
  initButton( &buttons[6], ConfigureInputBtn7, ConfigureLedBtn7, CheckPressedBtn7, TurnLedOnBtn7, TurnLedOffBtn7 );  
  
  int buttons_size = sizeof(buttons) / sizeof(Button);

  USBCON = 0; 

  setup_timer();
  sei();
  
  for (;;) {
    for (int i = 0; i < buttons_size; ++i) {
      if (checkAndDebounce(&buttons[i],PROGRAM_EXECUTION_TIME)) {
       buttons[i].turnLedOn();
      }
      else {
       buttons[i].turnLedOff();
      }
    }
  }

  return 1;
}

ISR(TIMER1_COMPA_vect)
{
  PROGRAM_EXECUTION_TIME++;
}
