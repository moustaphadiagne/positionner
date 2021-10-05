#include <SoftwareSerial.h> // Software serial port
#define DirectionControl 5  //pin D1
#define droite 14   //PIN D5
#define gauche 12   //PIN  D6
#define haut 13     //PIN D7
#define bas 15      //PIN D8

int pinOut=0;
int pinActive=0;
//SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin); // activate software serial port
SoftwareSerial mySerial(4, 0); // RX, TX   //D2 D3

byte outArray[7]; // Serial buffer
byte tab[7];
int stopcheck; // For checking when a STOP command is received (0)
int checksum; // For Calculating Checksum. Sum of the payload bytes (bytes 2 through 6) in the message
int ByteNumber; // Counter for reading the serial buffer

void setup(){

  Serial.begin(9600); // Visca Baudrate 9600
  mySerial.begin(9600); // Pelco baud rate 9600 can be 1200,2400 or 4800
  pinMode(DirectionControl, OUTPUT);  
  pinMode(droite, OUTPUT); 
  pinMode(gauche, OUTPUT); 
  pinMode(haut, OUTPUT); 
  pinMode(bas, OUTPUT); 
  digitalWrite(DirectionControl, LOW); //preparing for receiption

  }

void loop(){

  if ( mySerial.available () > 0) {

    outArray[ByteNumber ++] = mySerial.read();

    if ( outArray[0] != 0xFF ) { 

      ByteNumber = 0;

      }
  } // When Byte 0 isn’t 0xFF (Pelco 1st Byte) reset Bytenumber to 0 preventing the serial port being blocked.

  if ( ByteNumber > 6) { // process it
    ByteNumber = 0; // ready for next time 
    for (int i = 0; i < 7; i++) { //Affection des valeurs 

      tab[i]=outArray[i];
      
      } 

    for (int n = 0; n < 7; n++) { //lecture des valeurs et comparaison
      if (tab[n] == 239 && pinActive==0){
        digitalWrite(haut, HIGH);
        Serial.println("haut");
        pinActive=1;
      }
      if (tab[n] == 247 && pinActive==0){
        digitalWrite(gauche, HIGH);
        Serial.println("gauche");
        pinActive=1;
      }
      if (tab[n] == 251 && pinActive==0){
        digitalWrite(droite, HIGH);
        Serial.println("droite");
        pinActive=1;
      }
      if (tab[n] == 223 && pinActive==0){
        digitalWrite(bas, HIGH);
        Serial.println("bas");
        pinActive=1;
      }
      checksum = outArray[1] + outArray[2] + outArray[3] + outArray[4] + outArray[5] ;
      Serial.print("checksum : ");
      Serial.println(checksum);
      
      if (checksum  > 770){
        digitalWrite(droite, LOW);
        digitalWrite(gauche, LOW);
        digitalWrite(haut, LOW);
        digitalWrite(bas, LOW);
        digitalWrite(LED_BUILTIN, HIGH);
        pinActive=0;
      }
     }
    }

}          
