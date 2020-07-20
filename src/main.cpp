#include <avr/io.h>
#include <util/delay.h>
#include "iidx_state.h"

uint16_t button_state = 0;

#define DEFINE_PIN_AS_OUTPUT(DeviceName,PortNum,PinNum) inline void ConfigureOutput##DeviceName() { DDR##PortNum |=  ( 1 << DD##PortNum##PinNum ); } \
                                                        inline bool CheckHigh##DeviceName() { return (PIN##PortNum & ( 1 << PIN##PortNum##PinNum )); } \
                                                        inline void DriveHigh##DeviceName()  { PORT##PortNum |= ( 1 << PORT##PortNum##PinNum );  } \
                                                        inline void DriveLow##DeviceName()   { PORT##PortNum &= ~( 1 << PORT##PortNum##PinNum );  } 


#define DEFINE_PIN_AS_INPUT(DeviceName,PortNum,PinNum) inline void ConfigureInput##DeviceName() { DDR##PortNum &= ~( 1 << DD##PortNum##PinNum ); } \
                                                       inline bool CheckHigh##DeviceName() { return PIN##PortNum & ( 1 << (PIN##PortNum##PinNum )) ; } 


DEFINE_PIN_AS_INPUT (Btn1,     D, 1);
DEFINE_PIN_AS_INPUT (Btn2,     D, 0);
DEFINE_PIN_AS_INPUT (Btn3,     D, 4);
DEFINE_PIN_AS_INPUT (Btn4,     C, 6);
DEFINE_PIN_AS_INPUT (Btn5,     D, 7);
DEFINE_PIN_AS_INPUT (Btn6,     B, 4);
DEFINE_PIN_AS_INPUT (Btn7,     B, 5);
DEFINE_PIN_AS_INPUT (BtnStart, B, 6);
DEFINE_PIN_AS_OUTPUT(Led,      C, 7);

inline void SetPinModes()
{
  ConfigureInputBtn1();
  ConfigureInputBtn2();
  ConfigureInputBtn3();
  ConfigureInputBtn4();
  ConfigureInputBtn5();
  ConfigureInputBtn6();
  ConfigureInputBtn7();
  ConfigureOutputLed();
}

 
int main()
{
  SetPinModes();
  for (;;) {
    if ( CheckHighBtn1() || CheckHighBtn2() ) {
      DriveHighLed();  
    }
    else {
      DriveLowLed();  
    }
  }
}
