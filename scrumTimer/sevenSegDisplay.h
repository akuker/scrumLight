#include <Arduino.h>
#include <Wire.h>
#include "utils.h"

#pragma once

//Pin connected to ST_CP of 74HC595
const int latchPin = 3;
//Pin connected to SH_CP of 74HC595
const int clockPin = 4;
//Pin connected to DS of 74HC595
const int dataPin = 5;


class sevenSegDisplay {
  public:
    sevenSegDisplay();
    void setRedLed(bool val);
    void setAmberLed(bool val);
    void setGreenLed(bool val);
    void setAux1Led(bool val);
    void setAux2Led(bool val);
    void setColonLed(bool val);

    void displaySeconds(int newSeconds);
    
  private:
    void updateDisplay(unsigned long newPattern);
    void setLedState(bool val, unsigned long flag);
    

    bool m_redLed;
    bool m_amberLed;
    bool m_greenLed;
    unsigned long m_currentDisplayedPattern;
    static const unsigned long char1_7_seg[]; 
    static const unsigned long char2_7_seg[]; 
    static const unsigned long char3_7_seg[]; 
    static const unsigned long char4_7_seg[]; 
    static const unsigned long allCharacters;

};
