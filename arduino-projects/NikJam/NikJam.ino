#include <SPI.h>
#include "LedMatrix.h"

//LED Matrix Pins
#define NUMBER_OF_DEVICES 4 
#define CS_PIN 15
#define CLK_PIN 14
#define MISO_PIN 2 //Not Used
#define MOSI_PIN 12

LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);

bool ledSetup[32][7];
bool pressed[8];
int buttons[8] = {4, 5, 16, 17, 18, 19, 21, 23};
int utilTime = 0;
bool playMode = false;
int timePosition = 0;
  
void setup() {
  ledMatrix.init();
  ledMatrix.setText("I<3U");

  pinMode(4, INPUT_PULLDOWN);
  pinMode(5, INPUT_PULLDOWN);
  pinMode(16, INPUT_PULLDOWN);
  pinMode(17, INPUT_PULLDOWN);
  pinMode(18, INPUT_PULLDOWN);
  pinMode(19, INPUT_PULLDOWN);
  pinMode(21, INPUT_PULLDOWN);
  pinMode(23, INPUT_PULLDOWN);

  analogReadResolution(10);
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(36, INPUT);

  for (int i = 0; i < 7; i++)
  {
    pressed[i] = false;
  }

  for (int i = 0; i < 7; i++)
  {
    for (int j = 0; j < 7; j++)
    {
      ledSetup[i][j] = false;
    }
  }
}

void loop() {
  ledMatrix.clear();

  if (playMode)
  {
    ledMatrix.setPixel(timePosition,7);
    int timeDiff = 600 - (analogRead(33)/2);
    handleButtons(timePosition, 10);

    for (int i = 0; i < timeDiff; i += 10)
    {
      delay(10);  
      handleButtons(timePosition, 10);
    }

    //TODO: do something per lit light at timePosition
    
    timePosition = (timePosition + 1) % 32;

  }
  else
  {
    int potVal = analogRead(32)/33;
    ledMatrix.setPixel(potVal,7);
    handleButtons(potVal, 10);
  }
  
  setleds();
  ledMatrix.commit();
  delay(10);
}

void handleButtons(int col, int uTime)
{
  for (int i = 0; i < 7; i++)
  {
    if (digitalRead(buttons[i]) == HIGH)
    {
      pressed[i] = true;
    }
    else if (pressed[i] == true)
    {
      pressed[i] = false;
      setArrayVal(col, i);
    }
  }

  //Utility button
  if (digitalRead(buttons[7]) == HIGH)
  {
    pressed[7] = true;
    utilTime += uTime;
  }
  else if (pressed[7] == true)
  {
    pressed[7] = false;
    if (utilTime > 300)
      resetAllLed();
    else
      switchState();
      
    utilTime = 0;
  }
}

//Turn off all Leds
void resetAllLed()
{
  for (int i = 0; i < 32; i++)
  {
    for (int j = 0; j < 7; j++)
    {
      ledSetup[i][j] = false;
    }
  }
}

//Set value for led array
void setArrayVal(int row, int col)
{
  if (ledSetup[row][col])
    ledSetup[row][col] = false;
  else
    ledSetup[row][col] = true;
}

//Go through all column values for leds
void setleds()
{
  for (int i = 0; i < 32; i++)
  {
    for (int j = 0; j < 7; j++)
    {
      if (ledSetup[i][j])
        ledMatrix.setPixel(i, j);
    }
  }
}

//Go from play to setup and vice versa
void switchState()
{
  playMode = !playMode;
  timePosition = 0;
}
