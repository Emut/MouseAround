//#define _DEBUG
//#define _PURE_DEBUG

#include "Keyboard.h"
#include "Mouse.h"
#include "SoftwareSerial.h"
#include "CArduinoCommandParser.cpp"

SoftwareSerial mySerial(8, 9); // RX, TX
CArduinoCommandParser myParser(64);

void setup() { // initialize the buttons' inputs:


  Serial.begin(9600);
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

 
  myParser.setMouseMoveCallback(ArduMouseMoveCallBack);
  myParser.setKeyActionCallback(ArduKeyActionCallBack);
  myParser.setReleaseAllCallback(ArduReleaseAllCallBack);
  #ifdef _DEBUG
  Serial.println("Lets Go");
  #endif
}

void loop() {
  if(mySerial.available() > 0){
    
    int nRead = mySerial.read();
    //Serial.println(nRead);
    myParser.InsertInput(nRead);  
  }
  delay(1);


}

void ArduMouseMoveCallBack(char par_cX, char par_cY, char par_cScroll){
  #ifdef _DEBUG
  Serial.print("ArduMouseMoveCallBack ");
  Serial.print((int)par_cX);
  Serial.print(",");
  Serial.print((int)par_cY);
  Serial.print(",");
  Serial.println((int)par_cScroll);
  #endif
  #ifndef _PURE_DEBUG
  Mouse.move(par_cX, par_cY, par_cScroll);
  #endif
}

void ArduKeyActionCallBack(char par_cX, bool par_bPressed){
  #ifdef _DEBUG
  Serial.println("ArduKeyActionCallBack");
  if(par_bPressed)
    Serial.print("pressed ");
  else
    Serial.print("released ");
  Serial.println((int)par_cX);
  #endif
  #ifndef _PURE_DEBUG
  if(par_bPressed){
    switch(par_cX){
      case LM_BUTTON:
        Mouse.click(MOUSE_LEFT);
      break; 
      case RM_BUTTON:
        Mouse.click(MOUSE_RIGHT);
      break;
      case MM_BUTTON:
        Mouse.click(MOUSE_MIDDLE);
      break;
      default:
        Keyboard.press(par_cX);
      break;
    }
  }
  else{
    switch(par_cX){
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
        Keyboard.release(par_cX);
      break;
    }
  }
  #endif
}

void ArduReleaseAllCallBack(){
  #ifdef _DEBUG
  Serial.println("ArduReleaseAllCallBack");
  #endif
  #ifndef _PURE_DEBUG
  Keyboard.releaseAll();
  Mouse.release(MOUSE_LEFT);
  Mouse.release(MOUSE_RIGHT);
  Mouse.release(MOUSE_MIDDLE);
  #endif
}
