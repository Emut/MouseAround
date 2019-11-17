#define _DEBUG
#define _PURE_DEBUG

#define BUTTON_PRESS 0xC1
#define BUTTON_RELEASE 0xC2
#define MOUSE_MOVE_HOR 0xC3
#define BUTTON_RLS_ALL 0xC4
#define MOUSE_SCROLL 0xC5
#define MOUSE_MOVE_VER 0xC6

#define LM_BUTTON 0x1
#define RM_BUTTON 0x2
#define MM_BUTTON 0x4

#include "Keyboard.h"
#include "Mouse.h"
#include "SoftwareSerial.h"

SoftwareSerial mySerial(8, 9); // RX, TX

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
  char sX = 0;
  char sY = 0;
  char checkSum = 0;
  unsigned char ucKey = 0;
  char cScroll = 0;
  if(mySerial.overflow())
    Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAA");
    
  if (mySerial.available() > 0) {
    int nRead = mySerial.read();
    #ifdef _DEBUG
      Serial.println(nRead);
    #endif
    switch(nRead){
      case MOUSE_MOVE_HOR:
        sX = 0;
        checkSum = 0;
        while(true){
            if(mySerial.available()>0){
              nRead = mySerial.read();
              sX = nRead;
              #ifdef _DEBUG
                Serial.println(nRead);
              #endif
              break; 
            }
        }
        while(true){
            if(mySerial.available()>0){
              nRead = mySerial.read();
              checkSum = nRead;
              #ifdef _DEBUG
                Serial.println(nRead);
              #endif
              break; 
            }
        }
        if((char)0xC3 + sX != checkSum)
          Serial.println("Invalid CheckSum");
        #ifdef _DEBUG
            Serial.print("MouseMoveH ");
            Serial.println((int)sX);  
        #endif
        #ifndef _PURE_DEBUG
          Mouse.move(sX,0);
        #endif
      break;

      case MOUSE_MOVE_VER:
        sY = 0;
        while(true){
            if(mySerial.available()>0){
              nRead = mySerial.read();
              sY = nRead;
              #ifdef _DEBUG
                Serial.println(nRead);
              #endif
              break; 
            }
        }
        while(true){
            if(mySerial.available()>0){
              nRead = mySerial.read();
              checkSum = nRead;
              #ifdef _DEBUG
                Serial.println(nRead);
              #endif
              break; 
            }
        }
        if((char)MOUSE_MOVE_VER + sY != checkSum)
          Serial.println("Invalid CheckSum");
        #ifdef _DEBUG
            Serial.print("MouseMoveV ");
            Serial.println((int)sY);  
        #endif
        #ifndef _PURE_DEBUG
          Mouse.move(0,sY);
        #endif
      break;
      
      case BUTTON_PRESS:
          ucKey = 0;
          while(true){
            if(mySerial.available()>0){
              ucKey = mySerial.read();
              break; 
            }
          }
          #ifdef _DEBUG
            Serial.print("KeyPress");
            Serial.println(ucKey);
          #endif
          switch(ucKey){
            case LM_BUTTON:
            #ifndef _PURE_DEBUG
              Mouse.press(MOUSE_LEFT);
            #endif
            break;
            case RM_BUTTON:
            #ifndef _PURE_DEBUG
              Mouse.press(MOUSE_RIGHT);
            #endif
            break;
            case MM_BUTTON:
            #ifndef _PURE_DEBUG
              Mouse.press(MOUSE_MIDDLE);
            #endif
            break;
            default:
            #ifndef _PURE_DEBUG
              Keyboard.press(ucKey);
            #endif
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
          #ifdef _DEBUG
            Serial.print("KeyRelease");
            Serial.println(ucKey);  
          #endif
          #ifndef _PURE_DEBUG
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
          #endif
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
          #ifndef _PURE_DEBUG  
            Mouse.move(0, 0, cScroll);
          #endif
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
