
#define BUTTON_PRESS 0xC1
#define BUTTON_RELEASE 0xC2
#define MOUSE_MOVE_DHDV 0xC3
#define BUTTON_RLS_ALL 0xC4
#define MOUSE_SCROLL 0xC5

#define LM_BUTTON 0x1
#define RM_BUTTON 0x2
#define MM_BUTTON 0x4

#include "Keyboard.h"
#include "Mouse.h"
#include "SoftwareSerial.h"

SoftwareSerial mySerial(8, 9); // RX, TX

// set pin numbers for the five buttons:
const int upButton = 2;
const int downButton = 3;
const int leftButton = 4;
const int rightButton = 5;
const int mouseButton = 6;

void setup() { // initialize the buttons' inputs:


  Serial.begin(9600);
  // initialize mouse control:
  Mouse.begin();
  Keyboard.begin();
  mySerial.begin(115200);

  Mouse.move(0, -40);
  delay(1000);
  Mouse.move(0, -40);
  delay(1000);
  Mouse.move(0, -40);
  delay(1000);
  Mouse.move(0, -40);
  delay(1000);
}

void loop() {
  // use serial input to control the mouse:
  short sX = 0;
  short sY = 0;
  unsigned char ucKey = 0;
  char cScroll = 0;
  if (mySerial.available() > 0) {
    int nRead = mySerial.read();
    switch(nRead){
      case MOUSE_MOVE_DHDV:
        sX = 0;
        sY = 0;
        while(true){
            if(mySerial.available()>1){
              nRead = mySerial.read();
              sX = nRead<<8;
              nRead = mySerial.read();
              sX |= nRead & 0xFF;
              break; 
            }
        }
          while(true){
            if(mySerial.available()>1){
              nRead = mySerial.read();
              sY = nRead<<8;
              nRead = mySerial.read();
              sY |= nRead & 0xFF;
              break; 
            }
          }
          //Serial.print("MouseMove");
          //Serial.print(sX);  
          //Serial.print(",");  
          //Serial.println(sY);
          Mouse.move(sX,sY);
      break;
      case BUTTON_PRESS:
          ucKey = 0;
          while(true){
            if(mySerial.available()>0){
              ucKey = mySerial.read();
              break; 
            }
          }
          //Serial.print("KeyPress");
          //Serial.println(ucKey);
          switch(ucKey){
            case LM_BUTTON:
              Mouse.press(MOUSE_LEFT);
            break;
            case RM_BUTTON:
              Mouse.press(MOUSE_RIGHT);
            break;
            case MM_BUTTON:
              Mouse.press(MOUSE_MIDDLE);
            break;
            default:
              Keyboard.press(ucKey);
            break;
          }
          
      break;
      case BUTTON_RELEASE:
          ucKey = 0;
          while(true){
            if(mySerial.available()>0){
              ucKey = mySerial.read();
              break; 
            }
          }
          //Serial.print("KeyRelease");
          //Serial.println(ucKey);  
          switch(ucKey){
            case LM_BUTTON:
              Mouse.release(MOUSE_LEFT);
            break;
            case RM_BUTTON:
              Mouse.release(MOUSE_RIGHT);
            break;
            case MM_BUTTON:
              Mouse.release(MOUSE_MIDDLE);
            break;
            default:
              Keyboard.release(ucKey);
            break;
          }
      break;

      case MOUSE_SCROLL:
        cScroll = 0;
          while(true){
            if(mySerial.available()>0){
              cScroll = mySerial.read();
              break; 
            }
          }
          //Serial.print("Scroll");
          //Serial.println(cScroll);  
          Mouse.move(0, 0, cScroll);
      break;

      case BUTTON_RLS_ALL:
        Keyboard.releaseAll();
        Mouse.release(MOUSE_LEFT);
        Mouse.release(MOUSE_RIGHT);
        Mouse.release(MOUSE_MIDDLE);
      break;
      
      default:
      break;
    }
  }


}