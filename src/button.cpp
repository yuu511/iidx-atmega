#include <button.h>

#define DEBOUNCE_TIME 0.0001 // 0.0001 // Seconds.
#define CYCLE_COUNT DEBOUNCE_TIME * F_CPU

void SetupButtons(Button *b, int NumButtons)
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

bool Button::checkAndDebounce() {
  if (!CheckPressed) {
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

// void ReadButtons()
// {
//   if ( CheckAndDebounceBtn1() ) {
//     TurnLedOnBtn1();
//   }
//   else {
//     TurnLedOffBtn1();
//   }
// 
//   if ( CheckAndDebounceBtn2() ) {
//     TurnLedOnBtn2();
//   }
//   else {
//     TurnLedOffBtn2();
//   }
// }
