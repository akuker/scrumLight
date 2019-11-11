//////////////////////////////////////
// Simple Aruduino program that reads button presses from
// a MCP23017 to control the timer. Then uses a series of
// shift registers to control a series of 7 segment
// displays. When the time runs out, will play a sound to
// let everyone know they're out of time!
//////////////////////////////////////

#include<Wire.h>
#include "mcp23017.h"
#include "sevenSegDisplay.h"
#include "utils.h"
#include "speakerManager.h"

/* For normal operation, there should be 1000 milliseconds per second.
 *  However, for debugging purposes, this can be reduced to make the
 *  clock run faster.
 */
const unsigned long millisPerSecond = 100;


// MCP23017 device hanging off the i2c bus.
mcp23017 *buttons;
sevenSegDisplay *sevenSeg;
speakerManagerClass *speaker;

// This is to be used when we're adding/subtracting time
// from the fixed values (ex max time)
const unsigned long secondsToMillis = 1000;
// Start time of the interval (last time the reset was pressed)
unsigned long startTimeMillis = 0;
// Default to a 2 minute timeout
unsigned long maxTimeMillis = 2 * 60 * secondsToMillis;
unsigned long maxDisplayableTime = ((20 * 60) * secondsToMillis) - (10 * secondsToMillis);
const unsigned long amberTimeMillis = 10 * secondsToMillis;

// Pin connected to the speaker output
const int speakerPin=6;

// Flag to disable the sound output
bool muteSound = false;

void setup() {
  // Initialize the serial bus
  Serial.begin (115200);

  buttons = new mcp23017(MCP23017_BUTTON_ADDR);

  // Enable the pull-up resistors on the MCP23017 I2C device
  buttons->writeI2c(MCP23017_GP_PU_A, 0xFF);
  buttons->writeI2c(MCP23017_GP_PU_B, 0xFF);

  speaker = new speakerManagerClass(speakerPin);
  sevenSeg = new sevenSegDisplay();
  startTimeMillis = millis();
}


void updateStopLight(unsigned long remainingTimeMillis)
{
  if (remainingTimeMillis > amberTimeMillis)
  {
    sevenSeg->setGreenLed(true);
    sevenSeg->setAmberLed(false);
    sevenSeg->setRedLed(false);
  }
  else if(remainingTimeMillis > 0)
  {
    sevenSeg->setGreenLed(false);
    sevenSeg->setAmberLed(true);
    sevenSeg->setRedLed(false);
  }
  else
  {
    sevenSeg->setGreenLed(false);
    sevenSeg->setAmberLed(false);
    sevenSeg->setRedLed(true);
  }

  sevenSeg->setAux2Led(muteSound);
}

void incrementDecrementMaxTime()
{
    if((buttons->pushed(BUTTON_UP)) && (maxTimeMillis < maxDisplayableTime))
    {
      maxTimeMillis += (secondsToMillis * 10);
    }
    if((buttons->pushed(BUTTON_DOWN)) && (maxTimeMillis > 0))
    {
      maxTimeMillis -= (secondsToMillis * 10);
    }
}

void loop() {
  buttons->update();
  speaker->update(muteSound);

  // If the reset button is pressed, reset the start time
  if(buttons->pushed(BUTTON_RESET))
  {
    startTimeMillis = millis();
  }
  // Toggle the mute state when the mute button is pressed
  if(buttons->pushed(BUTTON_MUTE))
  {
    muteSound = !muteSound;
    if(!muteSound)
    {
      speaker->playMario();
    }
  }
  /* Update flashing colon @ 2Hz */
  sevenSeg->setColonLed((millis() % 1000) < 500);

  unsigned long remainingTimeMillis = 0;
  unsigned long elapsedTimeMillis = (millis() - startTimeMillis);
  if(elapsedTimeMillis <= maxTimeMillis)
  {
    /* Only update the remaining amount of time if it is greater than zero) */
    remainingTimeMillis = (maxTimeMillis - elapsedTimeMillis);
  }


  updateStopLight(remainingTimeMillis);
  
  
  // If the "MAX" button is held down, we are displaying
  // or adjusting the maximum amount of time
  if(buttons->active(BUTTON_MAX_TIME))
  {
    sevenSeg->displaySeconds(maxTimeMillis/secondsToMillis);
    incrementDecrementMaxTime();
  }
  else
  {
    sevenSeg->displaySeconds(remainingTimeMillis/secondsToMillis);
  }

//  if((millis() % 100) == 99)
////  {
//Serial.print("Elapsed: ");
//Serial.print(elapsedTimeMillis);
//    Serial.print(" Max: ");
//    Serial.print(maxTimeMillis);
//    Serial.print(" remaining ");
//    Serial.print(remainingTimeMillis);
//    Serial.print(" maxdisplayable: ");
//    Serial.println(maxDisplayableTime);
//    

    
//  }

}
