#ifndef __BUTTON_H_
#define __BUTTON_H_

// CheckAndDebounce expects timer to be set up.

#define DEBOUNCE_TIME 0.0001 // 0.0001 // Seconds.
#define CYCLE_COUNT DEBOUNCE_TIME * F_CPU

#define DEFINE_BTN(DeviceName, BtnPort, BtnPin, LedPort, LedPin) \
  uint32_t debounceCounter##DeviceName = 0; \
  uint8_t  lastTimeRead##DeviceName = 0;    \
  inline void ConfigureInput##DeviceName()     { DDR##BtnPort &= ~( 1 << DD##BtnPort##BtnPin ); \
                                                 PORT##BtnPort |= ( 1 << PORT##BtnPort##BtnPin);   } \
  inline bool CheckInput##DeviceName()         { return !(PIN##BtnPort & ( 1 << (PIN##BtnPort##BtnPin ))) ; } \
  inline void ConfigureLed##DeviceName()       { DDR##LedPort |=  ( 1 << DD##LedPort##LedPin ); } \
  inline void TurnLedOn##DeviceName()          { PORT##LedPort |= ( 1 << PORT##LedPort##LedPin );  } \
  inline void TurnLedOff##DeviceName()         { PORT##LedPort &= ~( 1 << PORT##LedPort##LedPin );  } \
  inline bool CheckAndDebounce##DeviceName() { \
    if (!CheckInput##DeviceName()) { \
      if (debounceCounter##DeviceName > 0 ) { \
        debounceCounter##DeviceName--; \
      } \
    } \
    else if (debounceCounter##DeviceName < CYCLE_COUNT) { \
      debounceCounter##DeviceName++; \
    } \
    \
    if (debounceCounter##DeviceName >= CYCLE_COUNT) { \
      debounceCounter##DeviceName = CYCLE_COUNT; \
      return true; \
    } \
    return false; \
  }

#endif
