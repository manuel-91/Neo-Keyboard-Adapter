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
  
  if (len == 3) {
    uint8_t mouseRpt[4];
    mouseRpt[0] = buf[0];
    mouseRpt[1] = buf[1];
    mouseRpt[2] = buf[2];
    mouseRpt[3] = 0;  // somehow this byte is missing with this lib
    HID().SendReport(1,mouseRpt,sizeof(mouseRpt));
  }
  for (uint8_t i = 0; i < len; i++) {
    Serial.print(' '); Serial.print(buf[i], HEX);
  }
  Serial.println();
}

USB     Usb;

HIDBoot < USB_HID_PROTOCOL_KEYBOARD | USB_HID_PROTOCOL_MOUSE > HidComposite(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE>    HidMouse(&Usb);

MouseRptParser PrsMouse;
NeoReportParser PrsKbd;


void setup()
{
	BootKeyboard.begin();
	Consumer.begin();
  Serial.begin(115200); 
  Serial.println("Start"  ) ; 
  Mouse.begin();
    
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
