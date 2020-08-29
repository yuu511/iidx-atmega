#ifndef __BUTTON_H_
#define __BUTTON_H_

#include <avr/io.h>

// Debounce delay

typedef void (*Configure_Input)();
typedef void (*Configure_Led)();
typedef bool (*Check_Pressed)();
typedef void (*Turn_Led_On)();
typedef void (*Turn_Led_Off)();

struct Button {

  // LED
  Button( Configure_Input _configureInput, 
          Configure_Led   _configureLed, 
          Check_Pressed   _checkPressed,
          Turn_Led_On     _turnLedOn,
          Turn_Led_Off    _turnLedOff);

  // No LED
  Button( Configure_Input _configureInput, 
          Check_Pressed   _checkPressed );

  bool checkAndDebounce(uint64_t currentTime);

  uint64_t            lastTime;
  bool                lastState;
  bool                isChecking;
  Check_Pressed       checkPressed;
  Turn_Led_On         turnLedOn;
  Turn_Led_Off        turnLedOff;
};

// helper function for buttons with no LED.
void dummyLedFunction();

// compiler plz inline :) ty
#define DEFINE_BTN_FUNCTIONS(DeviceName, BtnPort, BtnPin, LedPort, LedPin) \
  inline void ConfigureInput##DeviceName()     { DDR##BtnPort &= ~( 1 << DD##BtnPort##BtnPin ); \
                                                 PORT##BtnPort |= ( 1 << PORT##BtnPort##BtnPin);   } \
  inline void ConfigureLed##DeviceName()       { DDR##LedPort |=  ( 1 << DD##LedPort##LedPin ); } \
  inline bool CheckPressed##DeviceName()         { return !(PIN##BtnPort & ( 1 << (PIN##BtnPort##BtnPin ))) ; } \
  inline void TurnLedOn##DeviceName()          { PORT##LedPort |= ( 1 << PORT##LedPort##LedPin );  } \
  inline void TurnLedOff##DeviceName()         { PORT##LedPort &= ~( 1 << PORT##LedPort##LedPin );  } \


#define DEFINE_BTN_FUNCTIONS_NO_LED(DeviceName, BtnPort, BtnPin) \
  inline void ConfigureInput##DeviceName()     { DDR##BtnPort &= ~( 1 << DD##BtnPort##BtnPin ); \
                                                 PORT##BtnPort |= ( 1 << PORT##BtnPort##BtnPin);   } \
  inline bool CheckPressed##DeviceName()       { return !(PIN##BtnPort & ( 1 << (PIN##BtnPort##BtnPin ))) ; } \

#endif
