"GND to GND,RST to 9,SDA to 10,MOSI to 11,MISO to 12,SCK to 13,3V3 to 3.3V"

#include "SPI.h"
#include "MFRC522.h"
#include <SoftwareSerial.h>
#include <TinyGPS.h> 
#include <String.h>

float lat,lon; 

#define SS_PIN 10
#define RST_PIN 9

#define SIM800_TX_PIN 0
#define SIM800_RX_PIN 1


TinyGPS gps; 


SoftwareSerial serialSIM800(SIM800_TX_PIN,SIM800_RX_PIN);
SoftwareSerial gpsSerial(0,1);//rx,tx
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void setup() {
    Serial.begin(9600);
    SPI.begin();
    gpsSerial.begin(9600);
    rfid.PCD_Init();
}
void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);


  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  strID.toUpperCase();

  Serial.print("Tap card key: ");
  Serial.println(strID);


  if (strID.indexOf("5B:E5:23:D9") >= 0) {
    
   Serial.println("Access Granted");
   
Serial.println("The GPS Received Signal:"); 


  while(gpsSerial.available()){ 
  if(gps.encode(gpsSerial.read()))
  {  
  gps.f_get_position(&lat,&lon); 
  // display position 
  Serial.print("GPS Signal");
  Serial.print("Position: "); 
  Serial.print("Latitude:"); 
  Serial.print(lat,6); 
  Serial.print(";"); 
  Serial.print("Longitude:"); 
  Serial.println(lon,6);   
  Serial.print(lon); 
  Serial.print(" "); 
  

 } 
  serialSIM800.begin(9600);
  delay(1000);
//#define TWICE for(int_index=0;_index<2;_index++)
  {
  Serial.println("Setup Complete!");
  Serial.println("Sending SMS...");

  
  delay(300);

 
  serialSIM800.print("AT+CMGF=1\r\n");
  delay(300);

  
  serialSIM800.print("AT+CMGS=\"+919492671016\"\r\n");//PH.NO
  delay(300);
  

  serialSIM800.print("long:");
  serialSIM800.print(lon);
  serialSIM800.print(",");
  serialSIM800.print("lat:");
  serialSIM800.print(lat);//SMS CONTENT
  delay(500);

  
  serialSIM800.write((char)26);
  delay(500);
     
  Serial.println("SMS Sent!"); 
  }
}
 
  } else {
    Serial.println("Nothing to show");
  }


  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  setup();
}
