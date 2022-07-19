/// NESTE CÓDIGO O SCROLL NÃO FUNCIONA DIREITO - VERIFICAR NO TERMINAL


// USB host mouse from USB Host Shield Library. Install using Library Manager
#include <hidboot.h>
#include <usbhub.h>

// USB device mouse library included with Arduino IDE 1.8.5
#include <Mouse.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

class MouseRptParser : public MouseReportParser
{
protected:
    void OnMouseMove(MOUSEINFO *mi);
    void OnLeftButtonUp(MOUSEINFO *mi);
    void OnLeftButtonDown(MOUSEINFO *mi);
    void OnRightButtonUp(MOUSEINFO *mi);
    void OnRightButtonDown(MOUSEINFO *mi);
    void OnMiddleButtonUp(MOUSEINFO *mi);
    void OnMiddleButtonDown(MOUSEINFO *mi);
//    void OnWheelMove(MOUSEINFO *mi);
    void OnButton4Up(MOUSEINFO *mi);
    void OnButton4Down(MOUSEINFO *mi);
    void OnButton5Up(MOUSEINFO *mi);
    void OnButton5Down(MOUSEINFO *mi);
    void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
};

void MouseRptParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
  // Run parent class method.
  MouseReportParser::Parse(hid, is_rpt_id, len, buf);

  Serial.print("MouseRptParser::Parse");
  // Show USB HID mouse report
  for (uint8_t i = 0; i < len ; i++) {
    Serial.print(' '); Serial.print(buf[i], HEX);
  }
  Serial.println();

  if (len > 2) {
    uint8_t mouseRpt[4];
    mouseRpt[0] = buf[0];
    mouseRpt[1] = buf[1];
    mouseRpt[2] = buf[2];
    // If the mouse/trackball has a scroll wheel, send the value
    if (len > 3) {
      mouseRpt[3] = buf[3];
    }
    else {
      mouseRpt[3] = 0;
    }
    //HID().SendReport(1,mouseRpt,sizeof(mouseRpt));
  }
};
void MouseRptParser::OnMouseMove(MOUSEINFO *mi)
{
  Serial.print("dx=");
  Serial.print(mi->dX, DEC);
  Serial.print(" dy=");
  Serial.println(mi->dY, DEC);
};
void MouseRptParser::OnLeftButtonUp  (MOUSEINFO *mi)
{
  Serial.println("L Butt Up");
};
void MouseRptParser::OnLeftButtonDown (MOUSEINFO *mi)
{
  Serial.println("L Butt Dn");
};
void MouseRptParser::OnRightButtonUp  (MOUSEINFO *mi)
{
  Serial.println("R Butt Up");
};
void MouseRptParser::OnRightButtonDown  (MOUSEINFO *mi)
{
  Serial.println("R Butt Dn");
};
void MouseRptParser::OnMiddleButtonUp (MOUSEINFO *mi)
{
  Serial.println("M Butt Up");
};
void MouseRptParser::OnMiddleButtonDown (MOUSEINFO *mi)
{
  Serial.println("M Butt Dn");
};
/*
void MouseRptParser::OnWheelMove(MOUSEINFO *mi)
{
  Serial.print("dz=");
  Serial.println(mi->dZ, DEC);
};
*/
void MouseRptParser::OnButton4Up  (MOUSEINFO *mi)
{
  Serial.println("Butt 4 Up");
};
void MouseRptParser::OnButton4Down  (MOUSEINFO *mi)
{
  Serial.println("Butt 4 Dn");
};
void MouseRptParser::OnButton5Up (MOUSEINFO *mi)
{
  Serial.println("Butt 5 Up");
};
void MouseRptParser::OnButton5Down (MOUSEINFO *mi)
{
  Serial.println("Butt 5 Dn");
};




class KbdRptParser : public KeyboardReportParser
{
    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);
    void OnKeyDown	(uint8_t mod, uint8_t key);
    void OnKeyUp	(uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
};

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
  MODIFIERKEYS mod;
  *((uint8_t*)&mod) = m;
  Serial.print((mod.bmLeftCtrl   == 1) ? "C" : " ");
  Serial.print((mod.bmLeftShift  == 1) ? "S" : " ");
  Serial.print((mod.bmLeftAlt    == 1) ? "A" : " ");
  Serial.print((mod.bmLeftGUI    == 1) ? "G" : " ");

  Serial.print(" >");
  PrintHex<uint8_t>(key, 0x80);
  Serial.print("< ");

  Serial.print((mod.bmRightCtrl   == 1) ? "C" : " ");
  Serial.print((mod.bmRightShift  == 1) ? "S" : " ");
  Serial.print((mod.bmRightAlt    == 1) ? "A" : " ");
  Serial.println((mod.bmRightGUI    == 1) ? "G" : " ");
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  Serial.print("DN ");
  PrintKey(mod, key);
  uint8_t c = OemToAscii(mod, key);

  if (c)
    OnKeyPressed(c);
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {

  MODIFIERKEYS beforeMod;
  *((uint8_t*)&beforeMod) = before;

  MODIFIERKEYS afterMod;
  *((uint8_t*)&afterMod) = after;

  if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) {
    Serial.println("LeftCtrl changed");
  }
  if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
    Serial.println("LeftShift changed");
  }
  if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) {
    Serial.println("LeftAlt changed");
  }
  if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI) {
    Serial.println("LeftGUI changed");
  }

  if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) {
    Serial.println("RightCtrl changed");
  }
  if (beforeMod.bmRightShift != afterMod.bmRightShift) {
    Serial.println("RightShift changed");
  }
  if (beforeMod.bmRightAlt != afterMod.bmRightAlt) {
    Serial.println("RightAlt changed");
  }
  if (beforeMod.bmRightGUI != afterMod.bmRightGUI) {
    Serial.println("RightGUI changed");
  }

}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  Serial.print("UP ");
  PrintKey(mod, key);
}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
  Serial.print("ASCII: ");
  Serial.println((char)key);
};



// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% do começo até aqui é o mesmo código

USB     Usb;
USBHub     Hub(&Usb);

HIDBoot < USB_HID_PROTOCOL_KEYBOARD | USB_HID_PROTOCOL_MOUSE > HidComposite(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE>    HidMouse(&Usb);

KbdRptParser KbdPrs;
MouseRptParser MousePrs;

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Daqui...

void setup()
{
 Serial.begin( 115200 );
  uint8_t attempts = 30;
  while (!Serial && attempts--) {
    delay(100); // Wait for serial port to connect for up to 3 seconds
  }
  Serial.println("Start");

  if (Usb.Init() == -1) {
    Serial.println("USB host shield did not start.");
  }
  delay( 200 );

  HidMouse.SetReportParser(0, &MousePrs);


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% até aqui é o mesmo código

  HidComposite.SetReportParser(0, &KbdPrs);
  HidComposite.SetReportParser(1, &MousePrs);
  HidKeyboard.SetReportParser(0, &KbdPrs);
  
}

void loop()
{
  Usb.Task();
  
}
