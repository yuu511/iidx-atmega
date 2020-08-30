#include <button.h>

#define TIMER_VAL 23

void initButton (   Button *b,
                    Configure_Input _configureInput, 
                    Configure_Led   _configureLed, 
                    Check_Pressed   _checkPressed,
                    Turn_Led_On     _turnLedOn,
                    Turn_Led_Off    _turnLedOff)
{
  _configureInput();
  _configureLed();
  b->lastTime = 0;
  b->lastState =  FALSE;
  b->isDebouncing = FALSE;
  b->checkPressed = _checkPressed;
  b->turnLedOn = _turnLedOn;
  b->turnLedOff = _turnLedOff;
}

bool checkAndDebounce(Button *b, uint64_t currentTime) 
{
  bool currentState = b->checkPressed();

  if (b->isDebouncing) {
    if (( currentTime - b->lastTime ) >= TIMER_VAL ) {
      b->lastState = currentState;
      b->lastTime = currentTime;
      b->isDebouncing = FALSE;
      return currentState;
    }
  }
  else if (currentState != b->lastState) {
    b->lastTime = currentTime;
    b->isDebouncing = TRUE;
  }

  return b->lastState;
}

void dummyLedFunction(){};
