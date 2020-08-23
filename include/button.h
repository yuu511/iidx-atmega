#ifndef __BUTTON_H_
#define __BUTTON_H_

#include <avr/io.h>

// checkAndDebounce expects timer to be set up.

typedef void (*Configure_Input)();
typedef void (*Configure_Led)();
typedef bool (*Check_Pressed)();
typedef void (*Turn_Led_On)();
typedef void (*Turn_Led_Off)();

struct Button {
  Button( Configure_Input _ConfigureInput, 
          Configure_Led   _ConfigureLed, 
          Check_Pressed   _CheckPressed,
          Turn_Led_On     _TurnLedOn,
          Turn_Led_Off    _TurnLedOff);


  uint32_t            debounceCounter;   
  uint8_t             lastTimeRead;
  Configure_Input     ConfigureInput;
  Configure_Led       ConfigureLed;
  Check_Pressed       CheckPressed;
  Turn_Led_On         TurnLedOn;
  Turn_Led_Off        TurnLedOff;
  bool checkAndDebounce(); 
};

// compiler plz inline :) ty
#define DEFINE_BTN(DeviceName, BtnPort, BtnPin, LedPort, LedPin) \
  inline void ConfigureInput##DeviceName()     { DDR##BtnPort &= ~( 1 << DD##BtnPort##BtnPin ); \
                                                 PORT##BtnPort |= ( 1 << PORT##BtnPort##BtnPin);   } \
  inline void ConfigureLed##DeviceName()       { DDR##LedPort |=  ( 1 << DD##LedPort##LedPin ); } \
  inline bool CheckPressed##DeviceName()         { return !(PIN##BtnPort & ( 1 << (PIN##BtnPort##BtnPin ))) ; } \
  inline void TurnLedOn##DeviceName()          { PORT##LedPort |= ( 1 << PORT##LedPort##LedPin );  } \
  inline void TurnLedOff##DeviceName()         { PORT##LedPort &= ~( 1 << PORT##LedPort##LedPin );  } \

void SetupButtons(Button *b, int NumButtons);

#endif
