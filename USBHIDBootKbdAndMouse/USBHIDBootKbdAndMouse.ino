#include <hidboot.h>
#include <usbhub.h>
#include <Mouse.h>

// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

 USB     Usb;
 USBHub     Hub(& Usb);

int f = 0;
bool spraying = false, backing = false;
long t0;
long t;
float k = 0.67;
int V = 0, U = 0, DV, DU; 
int countX = 0;
int countY = 0;
int currentX, currentY, targetX, targetY;
int DX, DY;
int sensor = 1;

double PX1[] = { 0.0, -1.25, 10.5, -27.25, 22.0, 0.0};
double PY1[] = { 0.0, -0.0833333333333337, 0.000000000000004,    6.58333333333332,    0.500000000000032,      0.0            };
double PX2[] = { 0.0,  0.270833333333305 ,-5.05092592592517 ,   34.6597222222148 , -115.220899470868   ,    163.996031745982 };
double PY2[] = { 0.0, -0.416666666666694 , 10.0185185185192 ,  -90.4444444444508 ,  385.810846560871   ,   -544.849206349239 };
double PX3[] = { 0.0, -1.95833333333621  , 90.1203703705068 ,-1540.56944444685   ,11601.43121695       , -32446.6349206896   };
double PY3[] = { 0.0,  0.33333333333382  ,-15.1296296296526 ,  254.138888889295  ,-1868.60449735766    ,   5245.65079366001  };
double PX4[] = { 0.0,  0.208333333330866 ,-15.4722222220576 ,  424.04166666256   ,-5115.7777777324     ,  23042.3333331459   };
double PY4[] = { 0.0, -0.354166666663977 , 22.5879629627835 , -538.437499995522  , 5689.67989413041    , -22311.4682537638   };
double PX5[] = { 0.0, -1.20833333331268  ,103.527777775979  ,-3320.70833327474   ,47263.5555547097     ,-251909.499995435    };
double PY5[] = { 0.0, -0.562499999989831 , 47.7546296287446 ,-1516.17361108230   ,21339.1521159865     ,-112156.654759663    };

double QX1[] = { 0.0,              -0.750,    5.33333333333333,            -10.75,    5.16666666666657,                0.0 };
double QY1[] = { 0.0,               0.875,   -8.24999999999999,            22.625,  -24.2499999999998 ,                0.0 };
double QX2[] = { 0.0, 0.0625             ,  -1.39351851851772 ,  8.17361111110346,   -5.02513227510041, -39.7579365079847  };
double QY2[] = { 0.0, -0.0208333333333109,   0.828703703703111, -8.57638888888318,   15.0621693121456 ,  -9.51984126980543 };
double QX3[] = { 0.0,  0.0416666666668233,   -2.91666666667403, 68.7083333334623 , -654.761904762902  ,2151.42857143144    };
double QY3[] = { 0.0, -0.145833333333571 ,   6.38425925927043 ,-101.673611111307 ,  694.351851853374  ,-1811.94444444885   };
double QX4[] = { 0.0, -0.666666666661837 ,  43.0740740737515  ,-1037.49999999195,    11042.8544972654 , -43785.3492059808  };
double QY4[] = { 0.0, -0.3125            ,  20.1342592590944  , -483.145833329221,    5115.99074069531, -20273.7222220346  };
double QX5[] = { 0.0,  0.520833333327152 , -45.0787037031624  , 1462.49305553783 ,  -21089.1216928645 , 114108.273808129   };
double QY5[] = { 0.0, -0.0416666666646699,   3.87962962945706 , -133.819444438866,    2027.54497346503, -11504.9761900478  };


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
};
void MouseRptParser::OnMouseMove(MOUSEINFO *mi)
{ 
  Mouse.move(mi->dX, mi->dY, 0);

/*  Serial.print("dx=");
  Serial.print(mi->dX, DEC);
  Serial.print(" dy=");
  Serial.print(mi->dY, DEC);
  
  Serial.print(" t=");
  Serial.print(millis()-t0, DEC);
  
  countX = countX + mi->dX;
  countY = countY + mi->dY;
  
  Serial.print(" CountX=");
  Serial.print(countX, DEC);
  Serial.print(" CountY=");
  Serial.println(countY, DEC);  
  */
};
void MouseRptParser::OnLeftButtonUp	(MOUSEINFO *mi)
{
  Serial.println("L ButtUp");
  Mouse.release(MOUSE_LEFT);
  spraying = false;
  backing = true;
  t0 = millis();
  f = -32;
};
void MouseRptParser::OnLeftButtonDown	(MOUSEINFO *mi)
{ 
  if(!spraying){
    Mouse.press(MOUSE_LEFT);
    spraying = true;
    t0 = millis();
    f = 1;
    currentX = 0;
    currentY = 0;
  }
};

void MouseRptParser::OnRightButtonUp	(MOUSEINFO *mi)
{
  //Serial.println("R ButtUp");
  Mouse.release(MOUSE_RIGHT);
};
void MouseRptParser::OnRightButtonDown	(MOUSEINFO *mi)
{
  //Serial.println("R Butt Dn");
  Mouse.press(MOUSE_RIGHT);
  countX = 0;
  countY = 0;
};
void MouseRptParser::OnMiddleButtonUp	(MOUSEINFO *mi)
{
  //Serial.println("M ButtUp");
  Mouse.release(MOUSE_MIDDLE);
};
void MouseRptParser::OnMiddleButtonDown	(MOUSEINFO *mi)
{
  //Serial.println("M Butt Dn");
  Mouse.press(MOUSE_MIDDLE);
  if(sensor == 0){
    sensor = 1;
    Serial.println("Ak-47");
  }
  else{
    sensor = 0;
    Serial.println("M4A1-S");
  }
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
  Serial.print(" VP ");
  PrintKey(mod, key);
}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
  Serial.print("ASCII: ");
  Serial.println((char)key);
};



HIDBoot < USB_HID_PROTOCOL_KEYBOARD | USB_HID_PROTOCOL_MOUSE > HidComposite(& Usb);
HIDBoot< USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(& Usb);
HIDBoot< USB_HID_PROTOCOL_MOUSE>    HidMouse(& Usb);

KbdRptParser KbdPrs;
MouseRptParser MousePrs;

void setup()
{
  sensor = digitalRead(8);
  Mouse.begin();
  Serial.begin( 115200 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - Used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("Start");

  if ( Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay( 200 );

  HidComposite.SetReportParser(0, &KbdPrs);
  HidComposite.SetReportParser(1, &MousePrs);
  HidKeyboard.SetReportParser(0, &KbdPrs);
  HidMouse.SetReportParser(0, &MousePrs);

 pinMode(8, INPUT);
}
double p(double P[]){
  double t_ds = (double)t * 0.01;
  double value = 0.0;

  for(int i=1; i<6; i++){
    value = value * t_ds + P[i];
  }
  return value;
}

void loop()
{
  Usb.Task();
    
  if(spraying){
    t = millis() - t0;
    if(sensor == 1){
      if(t >=   0 && t < 400){ targetX = (int)p(PX1); targetY = (int)p(PY1);}
      if(t >= 400 && t < 900){ targetX = (int)p(PX2); targetY = (int)p(PY2);}
      if(t >= 900 && t <1400){ targetX = (int)p(PX3); targetY = (int)p(PY3);}
      if(t >=1400 && t <1900){ targetX = (int)p(PX4); targetY = (int)p(PY4);}
      if(t >=1900 && t <2400){ targetX = (int)p(PX5); targetY = (int)p(PY5);}
    }
    else{
      if(t >=   0 && t < 400){ targetX = (int)p(QX1); targetY = -(int)p(QY1);}
      if(t >= 400 && t < 900){ targetX = (int)p(QX2); targetY = -(int)p(QY2);}
      if(t >= 900 && t <1400){ targetX = (int)p(QX3); targetY = -(int)p(QY3);}
      if(t >=1400 && t <1900){ targetX = (int)p(QX4); targetY = -(int)p(QY4);}
      if(t >=1900 && t <2400){ targetX = (int)p(QX5); targetY = -(int)p(QY5);}
    }
    if(t >= 0 && t < 2400){
      DX = -targetX + currentX;
      DY =  targetY - currentY;
      if(DX != 0 && DY != 0){
        Mouse.move( DX, DY, 0);
        currentX = targetX;
        currentY = targetY;
      }
    }
    U = -currentX;
    V =  currentY;
    
  }  
  
  if(backing){
    if( V > 0 ){ Mouse.move( 0,-1, 0); V = V - 1; }
    if( V < 0 ){ Mouse.move( 0, 1, 0); V = V + 1; }
    if( U > 0 ){ Mouse.move(-1, 0, 0); U = U - 1; }
    if( U < 0 ){ Mouse.move( 1, 0, 0); U = U + 1; }
    if( U == 0 && V == 0 ) 
      backing=false;
  }
  
    /*
     t = millis() - t0;
     if(t >=   0 && t <  25 && f == -32){ DV = V/31; DU = U/31;  f++; V= V-DV; U=U-DU; }
     if(t >=  25 && t <  50 && f == -31){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     
     if(t >=  50 && t < 75 && f == -30){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     if(t >=  75 && t < 100 && f == -29){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     
     if(t >= 100 && t < 125 && f == -28){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     if(t >= 125 && t < 150 && f == -27){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     
     if(t >= 150 && t < 175 && f == -26){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     if(t >= 175 && t < 200 && f == -25){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     
     if(t >= 200 && t < 225 && f == -24){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     if(t >= 225 && t < 250 && f == -23){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     
     if(t >= 250 && t < 275 && f == -22){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     if(t >= 275 && t < 300 && f == -21){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     
     if(t >= 300 && t < 325 && f == -20){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     if(t >= 325 && t < 350 && f == -19){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     
     if(t >= 350 && t < 375 && f == -18){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     if(t >= 375 && t < 400 && f == -17){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     
     if(t >= 400 && t < 425 && f == -16){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     if(t >= 425 && t < 450 && f == -15){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     
     if(t >= 450 && t < 475 && f == -14){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     if(t >= 475 && t < 500 && f == -13){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     
     if(t >= 500 && t < 525 && f == -12){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     if(t >= 525 && t < 550 && f == -11){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     
     if(t >= 550 && t < 575 && f == -10){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     if(t >= 575 && t < 600 && f == -9){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     
     if(t >= 600 && t < 625 && f == -8){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     if(t >= 625 && t < 650 && f == -7){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     
     if(t >= 650 && t < 675 && f == -6){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     if(t >= 675 && t < 700 && f == -5){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     
     if(t >= 700 && t < 725 && f == -4){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     if(t >= 725 && t < 750 && f == -3){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     
     if(t >= 750 && t < 775 && f == -2){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     if(t >= 775 && t < 800 && f == -1){ Mouse.move(-DU, -DV, 0); f++; V= V-DV; U=U-DU; }
     
     if(t >= 825 && t < 850 && f ==  0){ backing=false; V=0; U=0;}
  } */
 
   
  
}
