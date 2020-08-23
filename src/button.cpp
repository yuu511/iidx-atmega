#include <button.h>

// Debounce delay
#define DELAY_SECONDS 0.0000001
#define CYCLE_COUNT DELAY_SECONDS * F_CPU

void setupButtons(Button *b, int NumButtons)
{
  for (int i = 0; i < NumButtons; ++i) {
    b[i].ConfigureInput();
    b[i].ConfigureLed();
  }
}

Button::Button
( Configure_Input _ConfigureInput, 
  Configure_Led   _ConfigureLed,  
  Check_Pressed   _CheckPressed,
  Turn_Led_On     _TurnLedOn,
  Turn_Led_Off    _TurnLedOff ) 

: ConfigureInput(_ConfigureInput),  
  ConfigureLed(_ConfigureLed),
  CheckPressed(_CheckPressed),
  TurnLedOn(_TurnLedOn),
  TurnLedOff(_TurnLedOff)
{
  debounceCounter = 0 ;  
  lastTimeRead = 0;
}

bool Button::checkPressedAndDebounce() {
  if (!CheckPressed()) {
    if (debounceCounter > 0) {
      debounceCounter--;
    }
  }
  else if (debounceCounter < CYCLE_COUNT) {
    debounceCounter++;
  }

  if  (debounceCounter >= CYCLE_COUNT) {
    debounceCounter = CYCLE_COUNT;
    return true;
  }
  return false;
}
