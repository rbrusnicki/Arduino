/*
  SD card datalogger

  This example shows how to log data from three analog sensors
  to an SD card using the SD library.

  The circuit:
   analog sensors on analog ins 0, 1, and 2
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created  24 Nov 2010
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;

const int S0 = A4;
const int S1 = A3;
const int S2 = A2;
const int S3 = A1;
const int SIG = A0;

void setup() {

  pinMode(4, INPUT);
  pinMode(5, INPUT);
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");
  delay(100);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
   // while (1);
  }
  Serial.println("card initialized.");

  pinMode(SIG, INPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
}

void log_data(int a, int b, int c, int d){
           
      if(a == 0)
          digitalWrite(S0, LOW);
      else
          digitalWrite(S0, HIGH);
          
      if(b == 0)
          digitalWrite(S1, LOW);
      else
          digitalWrite(S1, HIGH);
    
      if(c == 0)
          digitalWrite(S2, LOW);
      else
          digitalWrite(S2, HIGH);
    
      if(d == 0)
          digitalWrite(S3, LOW);
      else
          digitalWrite(S3, HIGH);
      
      delay(10);
      
}

void loop() {
  
//  File dataFile = SD.open("BMS_datalog.txt", FILE_WRITE);
  
//  if (dataFile) {
 
      Serial.print(micros());                           //dataFile.print(micros());
      Serial.print(", ");                               //dataFile.print("\t");
 
      log_data(0,0,0,0); Serial.print(analogRead(SIG)); Serial.print(", "); //dataFile.print(analogRead(SIG)); dataFile.print(",");
      log_data(1,0,0,0); Serial.print(analogRead(SIG)); Serial.print(", "); //dataFile.print(analogRead(SIG)); dataFile.print(",");
      log_data(0,1,0,0); Serial.print(analogRead(SIG)); Serial.print(", "); //dataFile.print(analogRead(SIG)); dataFile.print(",");
      log_data(1,1,0,0); Serial.print(analogRead(SIG)); Serial.print(", "); //dataFile.print(analogRead(SIG)); dataFile.print(",");
      log_data(0,0,1,0); Serial.print(analogRead(SIG)); Serial.print(", "); //dataFile.print(analogRead(SIG)); dataFile.print(",");
      log_data(1,0,1,0); Serial.print(analogRead(SIG)); Serial.print(", "); //dataFile.print(analogRead(SIG)); dataFile.print(",");
      log_data(0,1,1,0); Serial.print(analogRead(SIG)); Serial.print(", "); //dataFile.print(analogRead(SIG)); dataFile.print(",");
      log_data(1,1,1,0); Serial.print(analogRead(SIG)); Serial.print(", "); //dataFile.print(analogRead(SIG)); dataFile.print(",");
      log_data(0,0,0,1); Serial.print(analogRead(SIG)); Serial.print(", "); //dataFile.print(analogRead(SIG)); dataFile.print(",");
      log_data(1,0,0,1); Serial.print(analogRead(SIG)); Serial.print(", "); //dataFile.print(analogRead(SIG)); dataFile.print(",");
      log_data(0,1,0,1); Serial.print(analogRead(SIG)); Serial.print(", "); //dataFile.print(analogRead(SIG)); dataFile.print(",");
      log_data(1,1,0,1); Serial.print(analogRead(SIG)); Serial.print(", "); //dataFile.print(analogRead(SIG)); dataFile.print(",");
      log_data(0,0,1,1); Serial.print(analogRead(SIG)); Serial.print(", "); //dataFile.print(analogRead(SIG)); dataFile.print(",");
      log_data(1,0,1,1); Serial.print(analogRead(SIG)); Serial.print(", "); //dataFile.print(analogRead(SIG)); dataFile.print(",");
      log_data(0,1,1,1); Serial.print(analogRead(SIG)); Serial.print(", "); //dataFile.print(analogRead(SIG)); dataFile.print(",");
      log_data(1,1,1,1); Serial.print(analogRead(SIG));                     //dataFile.print(analogRead(SIG)); 

      Serial.println("");                               //dataFile.println("");
 
/*      dataFile.close();
      
  }
  else
      Serial.println("error opening txt");
*/     
}
