#include "mcp23017.h"

mcp23017::mcp23017(uint8_t i2cAddr) {
  m_i2cAddr = i2cAddr;

  // Enable the pull-up resistors on the MCP23017 I2C device
  writeI2c(MCP23017_GP_PU_A, 0xFF);
  writeI2c(MCP23017_GP_PU_B, 0xFF);

  
}

void mcp23017::update() {
  m_currentValue = 0;





//    Serial.print("Addr: ");
//    Serial.print(m_i2cAddr,HEX);
//    int val = readI2c(MCP23017_IO_DIR_A);
//    Serial.print(" MCP23017_IO_DIR_A:");
//    Serial.print(val,HEX);
//    val = readI2c(MCP23017_IO_DIR_B);
//    Serial.print(" MCP23017_IO_DIR_B:");
//    Serial.print(val,HEX);
//    val = readI2c(MCP23017_GP_IO_A);
//    Serial.print(" MCP23017_GP_IO_A:");
//    Serial.print(val,HEX);
//    val = readI2c(MCP23017_GP_IO_B);
//    Serial.print(" MCP23017_GP_IO_B:");
//    Serial.print(val,HEX);
//    val = readI2c(MCP23017_GP_PU_A);
//    Serial.print(" MCP23017_GP_PU_A:");
//    Serial.print(val,HEX);
//    val = readI2c(MCP23017_GP_PU_B);
//    Serial.print(" MCP23017_GP_PU_B:");
//    Serial.println(val,HEX);



//  Serial.print(readI2c(MCP23017_GP_IO_A),HEX);
//  Serial.println(readI2c(MCP23017_GP_IO_B),HEX);

  uint16_t readValue = readI2c(MCP23017_GP_IO_A);
  readValue |= (readI2c(MCP23017_GP_IO_B) << 8);

    Serial.print(" Buttons:");
    Serial.println(readValue,HEX);

//
//  m_debounceCounter
//
//      val = readI2c(MCP23017_BUTTON_ADDR, MCP23017_GP_IO_A);
//    val = readI2c(MCP23017_BUTTON_ADDR, MCP23017_GP_IO_B);
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
