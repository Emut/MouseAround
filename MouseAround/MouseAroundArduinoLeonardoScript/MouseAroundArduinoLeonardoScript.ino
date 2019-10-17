#include <Mouse.h>
#include <Keyboard.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 9); // RX, TX

#define BUTTON_PRESS 0xC1
#define BUTTON_RELEASE 0xC2
#define MOUSE_MOVE_DHDV 0xC3
#define BUTTON_RELEASE_ALL 0xC4

bool bLedOn = false;
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  Mouse.begin();
  Keyboard.begin();
  mySerial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  
  
  // print out the value you read:
  int nRead = 0;
  unsigned ucKey = 0;
  while(mySerial.available()>0){
    if(bLedOn){
      digitalWrite(LED_BUILTIN, LOW);
      bLedOn = false;
    }
    else{
      digitalWrite(LED_BUILTIN, HIGH);
      bLedOn = true;
    }
      
      nRead = mySerial.read();
      switch(nRead){
        case MOUSE_MOVE_DHDV:
        {
          short sX = 0;
          short sY = 0;
          while(true)
            if(mySerial.available()>1){
              nRead = mySerial.read();
              //mySerial.write(nRead);
              sX = nRead<<8;
              nRead = mySerial.read();
              //mySerial.write(nRead);
              sX |= nRead & 0xFF;
              break; 
            }
          while(true)
            if(mySerial.available()>1){
              nRead = mySerial.read();
              //mySerial.write(nRead);
              sY = nRead<<8;
              nRead = mySerial.read();
              //mySerial.write(nRead);
              sY |= nRead & 0xFF;
              break; 
            }
          Mouse.move(sX,sY);
        }
        case BUTTON_PRESS:
          ucKey = 0;
          while(true){
            if(mySerial.available()>0){
              ucKey = mySerial.read();
              break; 
            }
          }
          Keyboard.press(ucKey);
        break;

        case BUTTON_RELEASE:
          ucKey = 0;
          while(true){
            if(mySerial.available()>0){
              ucKey = mySerial.read();
              break; 
            }
          }
          Keyboard.release(ucKey);
        break;

        case BUTTON_RELEASE_ALL:
          Keyboard.releaseAll();
        break;
        
        default:
        break;
      }
        
      
  }
  delay(1);        // delay in between reads for stability
}
