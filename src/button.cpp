#include <button.h>

#define TIMER_VAL 23

Button::Button
( Configure_Input _configureInput, 
  Configure_Led   _configureLed,  
  Check_Pressed   _checkPressed,
  Turn_Led_On     _turnLedOn,
  Turn_Led_Off    _turnLedOff ) 

: lastTime(0),
  lastState(false),
  isChecking(true),
  checkPressed(_checkPressed),
  turnLedOn(_turnLedOn),
  turnLedOff(_turnLedOff)
{
  _configureInput(); 
  _configureLed();
}

void dummyLedFunction(){};

Button::Button ( Configure_Input _configureInput, Check_Pressed _checkPressed )
: lastTime(0), 
  lastState(false),
  isChecking(true),
  checkPressed(_checkPressed),
  turnLedOn(dummyLedFunction),
  turnLedOff(dummyLedFunction)
{ _configureInput(); }



bool Button::checkAndDebounce(uint64_t currentTime) {
  bool currentState = checkPressed();
  
  if (isChecking) {
    if ((currentTime - lastTime) >=  TIMER_VAL ) {
      lastState = currentState; 
      lastTime = currentTime;
      isChecking = false;
      return currentState;
    }
  }
  else if ( currentState != lastState )  {
    lastTime = currentTime;
    isChecking = true; 
  }
  return lastState;
}
