#include <avr/io.h>
#include <util/delay.h>
#include <button.h>

// Button Port / Pin , LED Port / Pin 
#ifndef DEBUG_MODE 
DEFINE_BTN (Btn1, D, 2, D, 3 );
#else 
DEFINE_BTN (Btn1, F, 0, F, 3 );
#endif

DEFINE_BTN (Btn2, D, 1, D, 0 );

Button buttons[2] {
  { Button(ConfigureInputBtn1, ConfigureLedBtn1, CheckPressedBtn1, TurnLedOnBtn1, TurnLedOffBtn1) }, 
  { Button(ConfigureInputBtn2, ConfigureLedBtn2, CheckPressedBtn2, TurnLedOnBtn2, TurnLedOffBtn2) }
};

uint8_t button_size = sizeof(buttons) / sizeof(Button);

int main()
{
  SetupButtons (buttons,button_size);
  for (;;) {
    if (buttons[0].CheckPressed()) {
     buttons[0].TurnLedOn();
    }
    else {
     buttons[0].TurnLedOff();
    }
  }
}
