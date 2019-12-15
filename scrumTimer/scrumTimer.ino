//////////////////////////////////////
// Simple Aruduino program that reads button presses from
// a MCP23017 to control the timer. Then uses a series of
// shift registers to control a series of 7 segment
// displays. When the time runs out, will play a sound to
// let everyone know they're out of time!
//////////////////////////////////////

#include <Wire.h>
#include <EEPROM.h>
#include "mcp23017.h"
#include "sevenSegDisplay.h"
#include "utils.h"
#include "speakerManager.h"

/* For normal operation, there should be 1000 milliseconds per second.
 *  However, for debugging purposes, this can be reduced to make the
 *  clock run faster.
 */
const unsigned long millisPerSecond = 100;

const int relayPinRed = 11;
const int relayPinAmber = 12;
const int relayPinGreen = 13;

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
const unsigned long amberTimeMillis = 30 * secondsToMillis;

const unsigned int eepromMutedAddr = 5;
const unsigned int eepromMaxTimeAddr = 10;

// Pin connected to the speaker output
const int speakerPin=6;
const int blinkyLed = 8;
const int resetInput = 9;

// Flag to disable the sound output
bool muteSound = false;
bool prevResetButtonState = false;

void setup() {
  char nvmValue = 0;
  // Initialize the serial bus
  Serial.begin (115200);
  while(!Serial){}

  buttons = new mcp23017(MCP23017_BUTTON_ADDR);

  // Enable the pull-up resistors on the MCP23017 I2C device
  buttons->writeI2c(MCP23017_GP_PU_A, 0xFF);
  buttons->writeI2c(MCP23017_GP_PU_B, 0xFF);

  speaker = new speakerManagerClass(speakerPin);
  sevenSeg = new sevenSegDisplay();
  startTimeMillis = millis();

  pinMode(relayPinRed, OUTPUT);
  pinMode(relayPinAmber, OUTPUT);
  pinMode(relayPinGreen, OUTPUT);

  // initialize the digital pin as an output.
  pinMode(blinkyLed, OUTPUT);
  digitalWrite(blinkyLed, LOW);
  pinMode(resetInput, INPUT);
  digitalWrite(resetInput, LOW);

  // Read the EEPROM Values
  EEPROM.get(eepromMutedAddr,nvmValue);
  // Convert to a boolean, in case we get some random garbage;
  muteSound = (nvmValue != 0);
  EEPROM.get(eepromMaxTimeAddr, maxTimeMillis);
  if(maxTimeMillis > maxDisplayableTime)
  {
    maxTimeMillis = 2 * 60 * secondsToMillis;
  }

  Serial.print("Eeprom Muted Value: ");
  Serial.print(muteSound);
  Serial.print(" Eeprom Max Time: ");
  Serial.println(maxTimeMillis);

  // The following can be used for debugging to set the timeout to a very short time
  // and to force mute to be disabled.
  //  muteSound = 0;
  //  maxTimeMillis = 2 * 60 * 15;
}

bool previousLoopTimedOut = false;

void updateStopLight(unsigned long remainingTimeMillis)
{


  if (remainingTimeMillis > (maxTimeMillis/4))
  {
    sevenSeg->setRedLed(false);
    sevenSeg->setAmberLed(false);
    sevenSeg->setGreenLed(true);
    digitalWrite(relayPinRed, HIGH);
    digitalWrite(relayPinAmber, HIGH);
    digitalWrite(relayPinGreen, LOW );
    previousLoopTimedOut = false;
  }
  else if(remainingTimeMillis > 0)
  {
    sevenSeg->setRedLed(false);
    sevenSeg->setAmberLed(true);
    sevenSeg->setGreenLed(false);
    digitalWrite(relayPinRed, HIGH);
    digitalWrite(relayPinAmber, LOW);
    digitalWrite(relayPinGreen, HIGH);
    previousLoopTimedOut = false;
  }
  else
  {
    sevenSeg->setRedLed(true);
    sevenSeg->setAmberLed(false);
    sevenSeg->setGreenLed(false); 
    digitalWrite(relayPinRed, LOW);
    digitalWrite(relayPinAmber, HIGH);
    digitalWrite(relayPinGreen, HIGH);
    // We only want to play the song once after the timeout.
    if(!previousLoopTimedOut)
    {
      speaker->playGameOver();
      previousLoopTimedOut = true;
    }
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
    // This function uses EEPROM.update() to perform the write, so does 
    // not rewrite the value if it didn't change.
    EEPROM.put(eepromMaxTimeAddr, maxTimeMillis);
}

void blinkLed()
{
  unsigned long curMillis = millis() % 2000;
  if(curMillis > 1000)
  {
    Serial.print(curMillis);
    Serial.println("on");
    digitalWrite(blinkyLed, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else
  {
    Serial.print(curMillis);
    Serial.println("off");
    digitalWrite(blinkyLed, LOW);    // turn the LED off by making the voltage LOW
  }
}

void loop() {
  buttons->update();
  speaker->update(muteSound);

  // Debug
  //blinkLed();

  // Check the state of the remote reset button. if it transitions from high to low
  // that's a reset!
  int resetButtonState = digitalRead(resetInput);

  // If the reset button is pressed, reset the start time
  if(buttons->pushed(BUTTON_RESET) || (!resetButtonState && prevResetButtonState))
  {
    speaker->stop();
    speaker->playCoin();
    startTimeMillis = millis();
  }
  prevResetButtonState = resetButtonState;
  
  // Toggle the mute state when the mute button is pressed
  if(buttons->pushed(BUTTON_MUTE))
  {
    muteSound = !muteSound;
    // This function uses EEPROM.update() to perform the write, so does 
    // not rewrite the value if it didn't change.
    EEPROM.put(eepromMutedAddr, muteSound);
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
    // Math trickery to always round the time up. Otherwise 400ms shows up as "0" on
    // the display
    sevenSeg->displaySeconds((remainingTimeMillis + (secondsToMillis/2))/secondsToMillis);
  }
}
