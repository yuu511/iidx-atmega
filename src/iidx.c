#include <iidx.h>
#include <button.h>
 
uint64_t PROGRAM_EXECUTION_TIME = 0;

typedef struct {
  uint8_t Modifier;
  uint8_t Reserved;
  uint8_t KeyCode[10];
} USB_ExtendedKeyboardReport_Data_t;

/** Global structure to hold the current keyboard interface HID report, for transmission to the host */
static USB_ExtendedKeyboardReport_Data_t KeyboardReportData;

/** Global structure to hold the current mouse interface HID report, for transmission to the host */
static USB_MouseReport_Data_t MouseReportData;

// timer 1 : Increments every 0.1 ms.
void SetupTimer(void) 
{
  TCCR1B |= ( 1 << WGM12) ; 
  OCR1A = 24;
  TIMSK1 |= ( 1 << OCIE1A);
  TCCR1B |= ( 1 << CS10) | ( 1 << CS11);
}

int main(void) 
{
  SetupHardware();

  GlobalInterruptEnable();

  for (;;) {
    Keyboard_HID_Task();
    Mouse_HID_Task();
    USB_USBTask();
  }

  return 1;
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

ISR(TIMER1_COMPA_vect)
{
  PROGRAM_EXECUTION_TIME++;
}

void EVENT_USB_Device_Connect(void){}

void EVENT_USB_Device_Disconnect(void){}

/** Event handler for the USB_ConfigurationChanged event. This is fired when the host sets the current configuration
 *  of the USB device after enumeration, and configures the keyboard and mouse device endpoints.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	/* Setup Keyboard HID Report Endpoints */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(KEYBOARD_IN_EPADDR, EP_TYPE_INTERRUPT, HID_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(KEYBOARD_OUT_EPADDR, EP_TYPE_INTERRUPT, HID_EPSIZE, 1);

	/* Setup Mouse HID Report Endpoint */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(MOUSE_IN_EPADDR, EP_TYPE_INTERRUPT, HID_EPSIZE, 1);
}

/** Event handler for the USB_ControlRequest event. This is used to catch and process control requests sent to
 *  the device from the USB host before passing along unhandled control requests to the library for processing
 *  internally.
 */
void EVENT_USB_Device_ControlRequest(void)
{
	uint8_t* ReportData;
	uint8_t  ReportSize;

	/* Handle HID Class specific requests */
	switch (USB_ControlRequest.bRequest)
	{
		case HID_REQ_GetReport:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				/* Determine if it is the mouse or the keyboard data that is being requested */
				if (!(USB_ControlRequest.wIndex))
				{
					ReportData = (uint8_t*)&KeyboardReportData;
					ReportSize = sizeof(KeyboardReportData);
				}
				else
				{
					ReportData = (uint8_t*)&MouseReportData;
					ReportSize = sizeof(MouseReportData);
				}

				/* Write the report data to the control endpoint */
				Endpoint_Write_Control_Stream_LE(ReportData, ReportSize);
				Endpoint_ClearOUT();

				/* Clear the report data afterwards */
				memset(ReportData, 0, ReportSize);
			}

			break;
		case HID_REQ_SetReport:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				/* Wait until the LED report has been sent by the host */
				while (!(Endpoint_IsOUTReceived()))
				{
					if (USB_DeviceState == DEVICE_STATE_Unattached)
					  return;
				}

				Endpoint_ClearOUT();
				Endpoint_ClearStatusStage();
			}

			break;
	}
}

/** Keyboard task. This generates the next keyboard HID report for the host, and transmits it via the
 *  keyboard IN endpoint when the host is ready for more data. Additionally, it processes host LED status
 *  reports sent to the device via the keyboard OUT reporting endpoint.
 */
void Keyboard_HID_Task(void)
{

	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;

     uint8_t gButtonStatus = gameplayButtonState(PROGRAM_EXECUTION_TIME);
     uint8_t mButtonStatus = metaButtonState();
     uint8_t keycodeCount = 0;

	/* Check if board button is not pressed, if so mouse mode enabled */
    if ( gButtonStatus & GBUTTON1 ) 
      KeyboardReportData.KeyCode[keycodeCount++] = HID_KEYBOARD_SC_S;

    if ( gButtonStatus & GBUTTON2 ) 
      KeyboardReportData.KeyCode[keycodeCount++] = HID_KEYBOARD_SC_Y;

    if ( gButtonStatus & GBUTTON3 ) 
      KeyboardReportData.KeyCode[keycodeCount++] = HID_KEYBOARD_SC_A;

    if ( gButtonStatus & GBUTTON4 ) 
      KeyboardReportData.KeyCode[keycodeCount++] = HID_KEYBOARD_SC_R;

    if ( gButtonStatus & GBUTTON5 ) 
      KeyboardReportData.KeyCode[keycodeCount++] = HID_KEYBOARD_SC_O;

    if ( gButtonStatus & GBUTTON6 ) 
      KeyboardReportData.KeyCode[keycodeCount++] = HID_KEYBOARD_SC_C;

    if ( gButtonStatus & GBUTTON7 ) 
      KeyboardReportData.KeyCode[keycodeCount++] = HID_KEYBOARD_SC_H;

    if ( mButtonStatus & MBUTTONSTART ) 
      KeyboardReportData.KeyCode[keycodeCount++] = HID_KEYBOARD_SC_N;

    if ( mButtonStatus & MBUTTONVEFX ) 
      KeyboardReportData.KeyCode[keycodeCount++] = HID_KEYBOARD_SC_Z;

	/* Select the Keyboard Report Endpoint */
	Endpoint_SelectEndpoint(KEYBOARD_IN_EPADDR);

	/* Check if Keyboard Endpoint Ready for Read/Write */
	if (Endpoint_IsReadWriteAllowed())
	{
		/* Write Keyboard Report Data */
		Endpoint_Write_Stream_LE(&KeyboardReportData, sizeof(KeyboardReportData), NULL);

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();

		/* Clear the report data afterwards */
		memset(&KeyboardReportData, 0, sizeof(KeyboardReportData));
	}

	/* Select the Keyboard LED Report Endpoint */
	Endpoint_SelectEndpoint(KEYBOARD_OUT_EPADDR);

	/* Check if Keyboard LED Endpoint Ready for Read/Write */
	if (Endpoint_IsReadWriteAllowed())
	{
		/* Handshake the OUT Endpoint - clear endpoint and ready for next report */
		Endpoint_ClearOUT();
	}
}

/** Mouse task. This generates the next mouse HID report for the host, and transmits it via the
 *  mouse IN endpoint when the host is ready for more data.
 */
void Mouse_HID_Task(void)
{
	// uint8_t JoyStatus_LCL = Joystick_GetStatus();

	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;

	/* Check if board button is pressed, if so mouse mode enabled */
    MouseReportData.Y = 0;
    MouseReportData.Y = 0;
    MouseReportData.Button = 0;
	//if (Buttons_GetStatus() & BUTTONS_BUTTON1)
	//{
	//	if (JoyStatus_LCL & JOY_UP)
	//	  MouseReportData.Y =  1;
	//	else if (JoyStatus_LCL & JOY_DOWN)
	//	  MouseReportData.Y = -1;

	//	if (JoyStatus_LCL & JOY_RIGHT)
	//	  MouseReportData.X =  1;
	//	else if (JoyStatus_LCL & JOY_LEFT)
	//	  MouseReportData.X = -1;

	//	if (JoyStatus_LCL & JOY_PRESS)
	//	  MouseReportData.Button |= (1 << 0);
	//}

	/* Select the Mouse Report Endpoint */
	Endpoint_SelectEndpoint(MOUSE_IN_EPADDR);

	/* Check if Mouse Endpoint Ready for Read/Write */
	if (Endpoint_IsReadWriteAllowed())
	{
		/* Write Mouse Report Data */
		Endpoint_Write_Stream_LE(&MouseReportData, sizeof(MouseReportData), NULL);

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();

		/* Clear the report data afterwards */
		memset(&MouseReportData, 0, sizeof(MouseReportData));
	}
}

