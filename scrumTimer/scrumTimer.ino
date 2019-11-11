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

/* For normal operation, there should be 1000 milliseconds per second.
 *  However, for debugging purposes, this can be reduced to make the
 *  clock run faster.
 */
const unsigned long millisPerSecond = 100;

const unsigned long greenTimeMillis = 20 * millisPerSecond;
const unsigned long amberTimeMillis = 10 * millisPerSecond;

// MCP23017 device hanging off the i2c bus.
mcp23017 *buttons;
sevenSegDisplay *sevenSeg;

void setup() {
  // Initialize the serial bus
  Serial.begin (115200);

  buttons = new mcp23017(MCP23017_BUTTON_ADDR);

  // Enable the pull-up resistors on the MCP23017 I2C device
  buttons->writeI2c(MCP23017_GP_PU_A, 0xFF);
  buttons->writeI2c(MCP23017_GP_PU_B, 0xFF);

  sevenSeg = new sevenSegDisplay();
}

int count=0;

unsigned long startTimeMillis = 0;
unsigned long maxTimeMillis = 2 * 60 * millisPerSecond;

void loop() {
  buttons->update();


  sevenSeg->setRedLed((count & 0x1) > 0);
  sevenSeg->setAmberLed((count & 0x2) > 0);
  sevenSeg->setGreenLed((count & 0x4) > 0);
  sevenSeg->setAux1Led((count & 0x1) > 0);
  sevenSeg->setAux2Led((count & 0x1) > 0);

  /* Update flashing colon @ 2Hz */
  sevenSeg->setColonLed((millis() % 1000) > 500);

  unsigned long remainingTimeMillis = 0;
  unsigned long elapsedTimeMillis = (millis() - startTimeMillis);
  if(elapsedTimeMillis <= maxTimeMillis)
  {
    sevenSeg->displaySeconds((maxTimeMillis - elapsedTimeMillis)/millisPerSecond);
    /* Only update the remaining amount of time if it is greater than zero) */
    remainingTimeMillis = (maxTimeMillis - elapsedTimeMillis);
  }

  if (remainingTimeMillis > greenTimeMillis)
  {
    sevenSeg->setGreenLed(true);
    sevenSeg->setAmberLed(false);
    sevenSeg->setRedLed(false);
  }
  else if(remainingTimeMillis > amberTimeMillis)
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


  

}
