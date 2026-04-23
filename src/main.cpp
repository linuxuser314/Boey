#include "myiolib.h"

constexpr PinStruct BUTTON = PIN_3;
constexpr PinStruct LED = PIN_4;


static bool checkForButtonPress(const PinStruct pin) {

  //Returns 1 when it detects the button change from on to off
  //The static keyword makes this variable state persistent across all function calls.
  static bool buttonIsPressed = false;

  bool buttonVal = !myDigitalRead(pin);
    //This checks if the button is pressed and it was not previously pressed, update buttonIsPressed and returns 1.
    if (buttonVal && !buttonIsPressed) {
      buttonIsPressed = true;
      return 1;
    }
    //If the button is not pressed but it was previously being pressed, reset the buttonIsPressed for the next press.
    else if (!buttonVal && buttonIsPressed == true) {
      buttonIsPressed = false;
    }
    else{
      //This is for the case where the button state has not changed, so we just return 0.
      return 0;
    }
  //End the function and return 0 if the button was not being pressed.
  return 0;

}

int main(void){
  initTimer2Millis();
  initSerial9600();
  myPinMode(BUTTON, IN);
  myPinMode(LED, OUT);
  bool buttonIsPressed = false;
  msgStruct tempMsg = {'\0', 0};

  while(true){
    
    if(checkForButtonPress(BUTTON)){
      buttonIsPressed = !buttonIsPressed;
      if(buttonIsPressed){
        serialTransmitMsg('B', 1);
      }
      else{
        serialTransmitMsg('B', 0);
      }
    }

    tempMsg = readSerialMsg();
    if(tempMsg.id == 'B' && tempMsg.val == 1){
      myDigitalWrite(LED, ON);
    }else if(tempMsg.id == 'B' && tempMsg.val == 0){
      myDigitalWrite(LED, OFF);
    }
    myDelay(100);
  }
	
}