#include <avr/io.h>
#include <util/delay.h>
#include <button.h>

// Button Port / Pin , LED Port / Pin 
DEFINE_BTN (Btn1, D, 1, D, 4 );
DEFINE_BTN (Btn2, D, 0, C, 6 );

inline void SetPinModes()
{
  ConfigureInputBtn1();
  ConfigureLedBtn1();
  ConfigureInputBtn2();
  ConfigureLedBtn2();
}

 
int main()
{
  SetPinModes();
  for (;;) {
    if ( debounceInputCheckBtn1() ) {
      TurnLedOnBtn1();
    }
    else {
      TurnLedOffBtn1();
    }

    if (debounceInputCheckBtn2() ) {
      TurnLedOnBtn2();
    }
    else {
      TurnLedOffBtn2();
    }
  }
}
