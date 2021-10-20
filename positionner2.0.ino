#include <SoftwareSerial.h> // Software serial port
#define droite 14   //PIN D5
#define gauche 12   //PIN  D6
#define haut 5     //PIN D1
#define bas 4      //PIN D2

int pinOut=0;
int pinActive=0;
SoftwareSerial mySerial(13, 0); // RX, TX   //D7 D3
const int analogInPin = A0;   //lecture potentiometre
int sensorValue = 0;

byte outArray[7]; // Serial buffer
byte tab[7];
int stopcheck; // For checking when a STOP command is received (0)
int checksum; // For Calculating Checksum. Sum of the payload bytes (bytes 2 through 6) in the message
int ByteNumber; // Counter for reading the serial buffer
int lockHaut;
int lockBas;

void setup(){

  Serial.begin(9600); // Visca Baudrate 9600
  mySerial.begin(9600); // Pelco baud rate 9600 can be 1200,2400 or 4800  
  pinMode(droite, OUTPUT); 
  pinMode(gauche, OUTPUT); 
  pinMode(haut, OUTPUT); 
  pinMode(bas, OUTPUT); 
  digitalWrite(droite, LOW);
  digitalWrite(gauche, LOW);
  digitalWrite(haut, LOW);
  digitalWrite(bas, LOW);
}

void loop(){

  if ( mySerial.available () > 0) {
        sensorValue = analogRead(analogInPin);
        outArray[ByteNumber ++] = mySerial.read();
    if ( outArray[0] != 0xFF ) { 
        ByteNumber = 0;
        }
  } 
  if ( ByteNumber > 6) { // process it
    sensorValue = analogRead(analogInPin);
    Serial.print("sensor = ");
        Serial.println(sensorValue);
    checksum = outArray[1] + outArray[2] + outArray[3] + outArray[4] + outArray[5] ;
    ByteNumber = 0; // ready for next time 
    
      //  if(sensorValue > 258 && sensorValue < 280)
      if (sensorValue >250 && sensorValue <275)
        {
              digitalWrite(haut, LOW);
              lockHaut = 1;
              lockBas = 0;
         }
       // if (sensorValue >880 && sensorValue <900)
        if(sensorValue > 890 && sensorValue < 910)
        {
               digitalWrite(bas, LOW);
               lockBas = 1;
               lockHaut = 0;
         }
          
      if ((checksum == 750 || checksum == 622)  && pinActive==0 && lockHaut == 0){
        digitalWrite(haut, HIGH);
        Serial.println("haut");
        pinActive=1;
        lockBas = 0;
      }
      if ((checksum == 734 || checksum == 606) && pinActive==0 && lockBas == 0){
        digitalWrite(bas, HIGH);
        Serial.println("bas");
        pinActive=1;
        lockHaut = 0;
      }
      if ((checksum == 758 || checksum == 748) && pinActive==0){
        digitalWrite(gauche, HIGH);
        Serial.println("gauche");
        pinActive=1;
      }
      if ((checksum == 762 || checksum == 756) && pinActive==0){
        digitalWrite(droite, HIGH);
        Serial.println("droite");
        //digitalWrite(haut, LOW);
        pinActive=1;
      }
      
      checksum = outArray[1] + outArray[2] + outArray[3] + outArray[4] + outArray[5] ;
      
      if (checksum  != 750 && checksum  != 622 && checksum  != 734 && checksum  != 606 && checksum  != 762 && checksum  != 756 && checksum  != 758 && checksum  != 748){
        digitalWrite(droite, LOW);
        digitalWrite(gauche, LOW);
        digitalWrite(haut, LOW);
        digitalWrite(bas, LOW);
        pinActive=0;
        Serial.print("sensor = ");
        Serial.println(sensorValue);
      }
     
    
  }
}          
