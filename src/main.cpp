#include <Arduino.h>
#include <Servo_ATTinyCore.h>
#include <Wire.h>

// https://learn.sparkfun.com/tutorials/tiny-avr-programmer-hookup-guide/attiny85-use-hints
// https://cdn.sparkfun.com/assets/2/8/b/a/a/Tiny_QuickRef_v2_2.pdf

// TODO: According to documentation, we should move the Servo PIN to PB1, pin 6
// Usable
// PB1  AIO, DIO
// PB3  AI, DIO
// PB4  AI, DIO

// 1  NC Reset
// 2  PB3 Analog Input, Digital Input, Digital Output
// 3  PB4 Analog Input, Digital Input, Digital Output
// 4  GND
// 5  PB0 I2C
// 6  PB1 Analog output, Analog Input, Digital Input, Digital Output
// 7  PB2 I2C
// 8  Power


// PWM pins are: PB0, PB1, PB3, PB4
// digital input: PB0, PB1, PB2, PB3, PB4
// digital output: PB0, PB1, PB2, PB3, PB4
// analog input: PB2, PB3, PB4
// analog output: PB0, PB1



#define SERVO_ANGLE  0x20
#define SERVO_ATTACH 0x21
#define SERVO_DETACH 0x23

#define RELAY_ATTACH 0x24
#define RELAY_DETACH 0x25
#define RELAY_HIGH   0x26
#define RELAY_LOW    0x27

volatile byte myArray[10];
volatile bool flag = false;

int servoPin = 0;
int relayPin = 0;
Servo myservo;

void handleEvent();

bool isPinAllowed(int pin) {
  return true;
  // switch ( pin ) {
  //   case PB1:
  //     return true;
  //     break;
  //   case PB3:
  //     return true;
  //     break;
  //   case PB4:
  //     return true;
  //     break;
  // }
  // return false;
}

void receiveEvent(int n) {
  for(int i=0; i<n; i++)
  {
    myArray[i] = Wire.read();
  }
  flag = true;
}

void handleEvent() {
  switch ( myArray[0] ) {
    case SERVO_ANGLE:
      {
        //if ( servoPin != 0 ) {
          myservo.write(myArray[1]);        
        //}
      }
      break;
    case SERVO_ATTACH:
      servoPin = myArray[1];
      myservo.attach(myArray[1]);
      break;
    case SERVO_DETACH:
      {
        myservo.detach();
        servoPin = 0;
      }
      break;
    case RELAY_ATTACH:
      if ( isPinAllowed(myArray[1]) ) {
        relayPin = myArray[1];
        pinMode(relayPin,OUTPUT);
      }
      break;
    case RELAY_DETACH:
      {
        relayPin = 0;
      }
      break;
    case RELAY_HIGH:
      if ( relayPin != 0 ) {        
        digitalWrite(relayPin,HIGH);
      }
      break;
    case RELAY_LOW:
      if ( relayPin != 0 ) {
        digitalWrite(relayPin,LOW);
      }
      break;
    default:
      break;
  }

}

void setup() {
  // myservo.attach(4);
  // for(int i=0;(i<5);i++) {
  //   myservo.write(0);
  //   delay(500);
  //   myservo.write(30);
  //   delay(500);
  // }

  Wire.begin(21);
  Wire.onReceive(receiveEvent);
}

void loop() {
  if ( flag == true ) {
    handleEvent();
    flag = false;
  }
}