#include <iidx.h>
#include <button.h>
 
uint64_t PROGRAM_EXECUTION_TIME = 0;

/** Buffer to hold the previously generated Keyboard HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevKeyboardHIDReportBuffer[sizeof(USB_KeyboardReport_Data_t)];

/** Buffer to hold the previously generated Mouse HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevMouseHIDReportBuffer[sizeof(USB_MouseReport_Data_t)];

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another. This is for the keyboard HID
 *  interface within the device.
 */
USB_ClassInfo_HID_Device_t Keyboard_HID_Interface =
	{
		.Config =
			{
				.InterfaceNumber              = INTERFACE_ID_Keyboard,
				.ReportINEndpoint             =
					{
						.Address              = KEYBOARD_IN_EPADDR,
						.Size                 = HID_EPSIZE,
						.Banks                = 1,
					},
				.PrevReportINBuffer           = PrevKeyboardHIDReportBuffer,
				.PrevReportINBufferSize       = sizeof(PrevKeyboardHIDReportBuffer),
			},
	};

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another. This is for the mouse HID
 *  interface within the device.
 */
USB_ClassInfo_HID_Device_t Mouse_HID_Interface =
    {
    	.Config =
    		{
    			.InterfaceNumber              = INTERFACE_ID_Mouse,
    			.ReportINEndpoint             =
    				{
    					.Address              = MOUSE_IN_EPADDR,
    					.Size                 = HID_EPSIZE,
    					.Banks                = 1,
    				},
    			.PrevReportINBuffer           = PrevMouseHIDReportBuffer,
    			.PrevReportINBufferSize       = sizeof(PrevMouseHIDReportBuffer),
    		},
    };

// timer 1 : Increments every 0.1 ms.
void SetupTimer(void) 
{
  TCCR1B |= ( 1 << WGM12) ; 
  OCR1A = 24;
  TIMSK1 |= ( 1 << OCIE1A);
  TCCR1B |= ( 1 << CS10) | ( 1 << CS11);
}

void SetupHardware(void) 
{
  /* Disable watchdog if enabled by bootloader/fuses */
  MCUSR &= ~(1 << WDRF);
  wdt_disable();
  
  /* Disable clock division */
  clock_prescale_set(clock_div_1);
  
  /* Hardware Initialization */
  SetupTimer();
  setupGameplayButtons();
  setupGameplayLEDs();
  setupMetaButtons();
  USB_Init();
}

int main(void) 
{
  SetupHardware();

  GlobalInterruptEnable();

  for (;;) {
    HID_Device_USBTask(&Keyboard_HID_Interface);
    HID_Device_USBTask(&Mouse_HID_Interface);
    USB_USBTask();
  }

  return 1;
}

ISR(TIMER1_COMPA_vect)
{
  PROGRAM_EXECUTION_TIME++;
}

void EVENT_USB_Device_Connect(void) {}
void EVENT_USB_Device_Disconnect(void){}
void EVENT_USB_Device_ConfigurationChanged(void)
{
  bool ConfigSuccess = true;
  
  ConfigSuccess &= HID_Device_ConfigureEndpoints(&Keyboard_HID_Interface);
  
  USB_Device_EnableSOFEvents();
}

void EVENT_USB_Device_ControlRequest(void)
{
  HID_Device_ProcessControlRequest(&Keyboard_HID_Interface);
}

void EVENT_USB_Device_StartOfFrame(void)
{
  HID_Device_MillisecondElapsed(&Keyboard_HID_Interface);
}

bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize)
{
  if (HIDInterfaceInfo == &Keyboard_HID_Interface) {
    USB_KeyboardReport_Data_t* KeyboardReport = (USB_KeyboardReport_Data_t*)ReportData;
    uint8_t gButtons = gameplayButtonState(PROGRAM_EXECUTION_TIME);
    uint8_t mButtons = metaButtonState();
    uint8_t UsedKeyCodes = 0;

    if (gButtons & GBUTTON1) {
      KeyboardReport->KeyCode[UsedKeyCodes++] = HID_KEYBOARD_SC_S;
    }

    if (gButtons & GBUTTON2) {
      KeyboardReport->KeyCode[UsedKeyCodes++] = HID_KEYBOARD_SC_Y;
    }

    if (gButtons & GBUTTON3) {
      KeyboardReport->KeyCode[UsedKeyCodes++] = HID_KEYBOARD_SC_A;
    }

    if (gButtons & GBUTTON4) {
      KeyboardReport->KeyCode[UsedKeyCodes++] = HID_KEYBOARD_SC_R;
    }

    if (gButtons & GBUTTON5) {
      KeyboardReport->KeyCode[UsedKeyCodes++] = HID_KEYBOARD_SC_O;
    }

    if (gButtons & GBUTTON6) {
      KeyboardReport->KeyCode[UsedKeyCodes++] = HID_KEYBOARD_SC_C;
    }

    if (gButtons & GBUTTON7) {
      KeyboardReport->KeyCode[UsedKeyCodes++] = HID_KEYBOARD_SC_H;
    }

    if (mButtons & MBUTTONSTART) {
      KeyboardReport->KeyCode[UsedKeyCodes++] = HID_KEYBOARD_SC_I;
    }

    if (mButtons & MBUTTONVEFX) {
      KeyboardReport->KeyCode[UsedKeyCodes++] = HID_KEYBOARD_SC_Y;
    }

    if (UsedKeyCodes)
      KeyboardReport->Modifier = HID_KEYBOARD_MODIFIER_LEFTSHIFT;

    *ReportSize = sizeof(USB_KeyboardReport_Data_t);
    return false;
  }
  else {
    return true;
  }
}

void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize){}


