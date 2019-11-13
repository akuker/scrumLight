#include "mcp23017.h"

mcp23017::mcp23017(uint8_t i2cAddr) {
  m_i2cAddr = i2cAddr;

  // Enable the pull-up resistors on the MCP23017 I2C device
  writeI2c(MCP23017_GP_PU_A, 0xFF);
  writeI2c(MCP23017_GP_PU_B, 0xFF);
}

/***********************************
 * Every time update() is called, it will read
 * all of the button inputs, then store those results
 * into a circular queue. The results from the last
 * N samples are "ORed" together so that the button
 * will only be "active" (voltage low, aka false)
 * after it has been pressed for N samples.
 * 
 * N is based upon the size of m_debounceValues
 */
void mcp23017::update() {
  // Store the current button states so we can look for transitions
  m_prevValue = m_currentValue;
  
  uint16_t readValue = readI2c(MCP23017_GP_IO_A);
  readValue |= (readI2c(MCP23017_GP_IO_B) << 8);

  m_debounceValues[m_debounceCounter] = readValue;
  m_debounceCounter = (m_debounceCounter + 1) % ELEMENTCNT(m_debounceValues);

  m_currentValue = 0;
  // We will debounce the buttons by requiring it to be pressed for N
  // cycles before we report it as true (voltage low)
  for (int i=0; i< ELEMENTCNT(m_debounceValues); i++)
  {
    m_currentValue |= m_debounceValues[i];
  }
}

bool mcp23017::active(int buttonMask)
{
  return ((m_currentValue & buttonMask) == 0);
}

bool mcp23017::pushed(int buttonMask)
{
  return ((m_currentValue & buttonMask) == 0) && ((m_prevValue & buttonMask) != 0);  
}

bool mcp23017::released(int buttonMask)
{
  return ((m_currentValue & buttonMask) != 0) && ((m_prevValue & buttonMask) == 0);  
}


void mcp23017::writeI2c(uint8_t regOffset, uint8_t value)
{
  if (!m_i2cWokeUp)
  {
    Wire.begin(); // wake up I2C bus
    m_i2cWokeUp = true;
  }
  Wire.beginTransmission(m_i2cAddr);
  Wire.write(regOffset); // register
  Wire.write(value);
  Wire.endTransmission();
}

uint8_t mcp23017::readI2c(uint8_t regOffset)
{
  if (!m_i2cWokeUp)
  {
    Wire.begin(); // wake up I2C bus
    m_i2cWokeUp = true;
  }
  byte readValue;
  Wire.beginTransmission(m_i2cAddr);
  Wire.write(regOffset);
  Wire.endTransmission();
  Wire.requestFrom((int)m_i2cAddr, 1); // request one byte of data from MCP20317
  readValue = Wire.read(); // store the incoming byte into "inputs"
  return readValue;
}
