#ifndef __BUTTON_H_
#define __BUTTON_H_

#include <avr/io.h>

typedef unsigned char bool;
#define TRUE 1
#define FALSE 0

// Debounce delay

typedef void (*Configure_Input)();
typedef void (*Configure_Led)();
typedef bool (*Check_Pressed)();
typedef void (*Turn_Led_On)();
typedef void (*Turn_Led_Off)();


typedef struct {
  uint64_t            lastTime;
  bool                lastState;
  bool                isDebouncing;
  Check_Pressed       checkPressed;
  Turn_Led_On         turnLedOn;
  Turn_Led_Off        turnLedOff;
} Button;


void initButton ( Button           *b,
                  Configure_Input _configureInput, 
                  Configure_Led   _configureLed, 
                  Check_Pressed   _checkPressed,
                  Turn_Led_On     _turnLedOn,
                  Turn_Led_Off    _turnLedOff);

// helper function for buttons with no LED.
void dummyLedFunction();

bool checkAndDebounce (Button *b, uint64_t currentTime);

#define DEFINE_BTN_FUNCTIONS(DeviceName, BtnPort, BtnPin, LedPort, LedPin) \
  void ConfigureInput##DeviceName()     { DDR##BtnPort &= ~( 1 << DD##BtnPort##BtnPin ); \
                                          PORT##BtnPort |= ( 1 << PORT##BtnPort##BtnPin);   } \
  void ConfigureLed##DeviceName()       { DDR##LedPort |=  ( 1 << DD##LedPort##LedPin ); } \
  bool CheckPressed##DeviceName()         { return !(PIN##BtnPort & ( 1 << (PIN##BtnPort##BtnPin ))) ; } \
  void TurnLedOn##DeviceName()          { PORT##LedPort |= ( 1 << PORT##LedPort##LedPin );  } \
  void TurnLedOff##DeviceName()         { PORT##LedPort &= ~( 1 << PORT##LedPort##LedPin );  } \


#define DEFINE_BTN_FUNCTIONS_NO_LED(DeviceName, BtnPort, BtnPin) \
  void ConfigureInput##DeviceName()     { DDR##BtnPort &= ~( 1 << DD##BtnPort##BtnPin ); \
                                          PORT##BtnPort |= ( 1 << PORT##BtnPort##BtnPin);   } \
  bool CheckPressed##DeviceName()       { return !(PIN##BtnPort & ( 1 << (PIN##BtnPort##BtnPin ))) ; } \

#endif
