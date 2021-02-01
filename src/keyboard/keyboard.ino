#include <usbhub.h>

#include "NeoReportParser.h"


class MouseRptParser : public MouseReportParser
{
protected:
    void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
};

void MouseRptParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
  // Run parent class method.
  MouseReportParser::Parse(hid, is_rpt_id, len, buf);

  if (buf[0] == 1) {
    if (len == 6) {
      uint8_t mouseRpt[4];
      mouseRpt[0] = buf[1];
      mouseRpt[1] = buf[2];
      mouseRpt[2] = buf[3];
      mouseRpt[3] = buf[4];
      HID().SendReport(1,mouseRpt,sizeof(mouseRpt));
    }
    else
    {
      // Serial.println("Unrecognized length");
    }
  }
  else
  {
    // when [buf[0] == 16h then it means we are scrolling horrizontally
    // and all numbers have different meaning
  }
}

USB     Usb;
USBHub     Hub(&Usb);

HIDBoot < USB_HID_PROTOCOL_KEYBOARD | USB_HID_PROTOCOL_MOUSE > HidComposite(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE>    HidMouse(&Usb);

MouseRptParser PrsMouse;
NeoReportParser PrsKbd;


void setup()
{
	BootKeyboard.begin();
	Consumer.begin();

	// Flash LED rapidly in case USB Shield could not be initialized
	if (Usb.Init() == -1) {
		while (true) {
			RXLED0;
			delay(100);
			RXLED1;
			delay(100);
		}
	}
	
	//The USB needs some time
	delay(200);

	HidComposite.SetReportParser(0, &PrsKbd);
  HidComposite.SetReportParser(1, &PrsMouse);
	HidKeyboard.SetReportParser(0, &PrsKbd);
  HidMouse.SetReportParser(0, &PrsMouse);
}

void loop()
{
  Usb.Task();
}
