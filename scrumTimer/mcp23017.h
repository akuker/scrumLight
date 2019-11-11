#include <Arduino.h>
#include <Wire.h>
#include "utils.h"

#pragma once

#define MCP23017_IO_DIR_A 0x00
#define MCP23017_IO_DIR_B 0x01
#define MCP23017_GP_PU_A 0x0C
#define MCP23017_GP_PU_B 0x0D
#define MCP23017_GP_IO_A 0x12
#define MCP23017_GP_IO_B 0x13
#define MCP23017_OLAT_A 0x14
#define MCP23017_OLAT_B 0x15
#define MCP23017_BANK_A MCP23017_IO_DIR_A
#define MCP23017_BANK_B MCP23017_IO_DIR_B

#define MCP23017_BUTTON_ADDR 0x20


class mcp23017 {
  public:
    mcp23017(uint8_t i2cAddr);
    void update();
    void writeI2c(uint8_t regOffset, uint8_t value);
    uint8_t readI2c(uint8_t regOffset);
  private:
    uint8_t m_i2cAddr;
    bool m_i2cWokeUp = false;
    uint16_t m_currentValue;
    uint16_t m_debounceValues[5];
    int m_debounceCounter = 0;

};
