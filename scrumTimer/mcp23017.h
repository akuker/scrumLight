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

#define BUTTON_UP       0x0040u
#define BUTTON_DOWN     0x0020u
#define BUTTON_RESET    0x8000u
#define BUTTON_MAX_TIME 0x0080u
#define BUTTON_MUTE     0x0001u

class mcp23017 {
  public:
    mcp23017(uint8_t i2cAddr);
    void update();
    bool active(int buttonMask);
    bool pushed(int buttonMask);
    bool released(int buttonMask);
    void writeI2c(uint8_t regOffset, uint8_t value);
    uint8_t readI2c(uint8_t regOffset);
  private:
    uint8_t m_i2cAddr;
    bool m_i2cWokeUp = false;
    uint16_t m_currentValue = 0xFFFF;
    uint16_t m_prevValue    = 0xFFFF;
    uint16_t m_debounceValues[5] = {0,0,0,0,0};
    int m_debounceCounter = 0;

};
