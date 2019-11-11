#include "sevenSegDisplay.h"
#include "utils.h"

#define LED_TOP_LEFT    0x00000020
#define LED_BOTTOM_LEFT 0x00000040
#define LED_COLON       0x00008000
#define LED_GREEN_LIGHT 0x00000001
#define LED_AMBER_LIGHT 0x01000000
#define LED_RED_LIGHT   0x00010000

#define LED_CHAR1_SEG_B 0x00000002
#define LED_CHAR1_SEG_C 0x00000004

#define LED_CHAR2_SEG_A 0x00000800
#define LED_CHAR2_SEG_B 0x00000200
#define LED_CHAR2_SEG_C 0x00002000
#define LED_CHAR2_SEG_D 0x00000100
#define LED_CHAR2_SEG_E 0x00000400
#define LED_CHAR2_SEG_F 0x00004000
#define LED_CHAR2_SEG_G 0x00001000

#define LED_CHAR3_SEG_A 0x08000000
#define LED_CHAR3_SEG_B 0x02000000
#define LED_CHAR3_SEG_C 0x20000000
#define LED_CHAR3_SEG_D 0x04000000
#define LED_CHAR3_SEG_E 0x80000000
#define LED_CHAR3_SEG_F 0x40000000
#define LED_CHAR3_SEG_G 0x10000000
#define LED_CHAR4_SEG_A 0x00080000
#define LED_CHAR4_SEG_B 0x00020000
#define LED_CHAR4_SEG_C 0x00040000
#define LED_CHAR4_SEG_D 0x00200000
#define LED_CHAR4_SEG_E 0x00800000
#define LED_CHAR4_SEG_F 0x00400000
#define LED_CHAR4_SEG_G 0x00100000

const unsigned long sevenSegDisplay::allCharacters = (LED_CHAR1_SEG_B | LED_CHAR1_SEG_C |  
  LED_CHAR2_SEG_A | LED_CHAR2_SEG_B | LED_CHAR2_SEG_C | LED_CHAR2_SEG_D | LED_CHAR2_SEG_E | LED_CHAR2_SEG_F | LED_CHAR2_SEG_G |
  LED_CHAR3_SEG_A | LED_CHAR3_SEG_B | LED_CHAR3_SEG_C | LED_CHAR3_SEG_D | LED_CHAR3_SEG_E | LED_CHAR3_SEG_F | LED_CHAR3_SEG_G |
  LED_CHAR4_SEG_A | LED_CHAR4_SEG_B | LED_CHAR4_SEG_C | LED_CHAR4_SEG_D | LED_CHAR4_SEG_E | LED_CHAR4_SEG_F | LED_CHAR4_SEG_G);

const unsigned long sevenSegDisplay::char1_7_seg[] = 
{ /* 0 */ 0, /* If the first digit is "0", leave it blank */
  /* 1 */ LED_CHAR1_SEG_B + LED_CHAR1_SEG_C,
};


const unsigned long sevenSegDisplay::char2_7_seg[] = 
{ /* 0 */ LED_CHAR2_SEG_A + LED_CHAR2_SEG_B + LED_CHAR2_SEG_C + LED_CHAR2_SEG_D + LED_CHAR2_SEG_E + LED_CHAR2_SEG_F,
  /* 1 */ LED_CHAR2_SEG_B + LED_CHAR2_SEG_C,
  /* 2 */ LED_CHAR2_SEG_A + LED_CHAR2_SEG_B + LED_CHAR2_SEG_G + LED_CHAR2_SEG_E + LED_CHAR2_SEG_D,
  /* 3 */ LED_CHAR2_SEG_A + LED_CHAR2_SEG_B + LED_CHAR2_SEG_G + LED_CHAR2_SEG_C + LED_CHAR2_SEG_D,
  /* 4 */ LED_CHAR2_SEG_F + LED_CHAR2_SEG_B + LED_CHAR2_SEG_G + LED_CHAR2_SEG_C,
  /* 5 */ LED_CHAR2_SEG_A + LED_CHAR2_SEG_F + LED_CHAR2_SEG_G + LED_CHAR2_SEG_C + LED_CHAR2_SEG_D,
  /* 6 */ LED_CHAR2_SEG_A + LED_CHAR2_SEG_F + LED_CHAR2_SEG_G + LED_CHAR2_SEG_C + LED_CHAR2_SEG_D + LED_CHAR2_SEG_E,
  /* 7 */ LED_CHAR2_SEG_A + LED_CHAR2_SEG_B + LED_CHAR2_SEG_C,
  /* 8 */ LED_CHAR2_SEG_A + LED_CHAR2_SEG_B + LED_CHAR2_SEG_C + LED_CHAR2_SEG_D + LED_CHAR2_SEG_E + LED_CHAR2_SEG_F + LED_CHAR2_SEG_G,
  /* 9 */ LED_CHAR2_SEG_A + LED_CHAR2_SEG_B + LED_CHAR2_SEG_C + LED_CHAR2_SEG_D + LED_CHAR2_SEG_F + LED_CHAR2_SEG_G,
};

const unsigned long sevenSegDisplay::char3_7_seg[] = 
{ /* 0 */ LED_CHAR3_SEG_A + LED_CHAR3_SEG_B + LED_CHAR3_SEG_C + LED_CHAR3_SEG_D + LED_CHAR3_SEG_E + LED_CHAR3_SEG_F,
  /* 1 */ LED_CHAR3_SEG_B + LED_CHAR3_SEG_C, 
  /* 2 */ LED_CHAR3_SEG_A + LED_CHAR3_SEG_B + LED_CHAR3_SEG_G + LED_CHAR3_SEG_E + LED_CHAR3_SEG_D,
  /* 3 */ LED_CHAR3_SEG_A + LED_CHAR3_SEG_B + LED_CHAR3_SEG_G + LED_CHAR3_SEG_C + LED_CHAR3_SEG_D,
  /* 4 */ LED_CHAR3_SEG_F + LED_CHAR3_SEG_B + LED_CHAR3_SEG_G + LED_CHAR3_SEG_C,
  /* 5 */ LED_CHAR3_SEG_A + LED_CHAR3_SEG_F + LED_CHAR3_SEG_G + LED_CHAR3_SEG_C + LED_CHAR3_SEG_D,
  /* 6 */ LED_CHAR3_SEG_A + LED_CHAR3_SEG_F + LED_CHAR3_SEG_G + LED_CHAR3_SEG_C + LED_CHAR3_SEG_D + LED_CHAR3_SEG_E,
  /* 7 */ LED_CHAR3_SEG_A + LED_CHAR3_SEG_B + LED_CHAR3_SEG_C,
  /* 8 */ LED_CHAR3_SEG_A + LED_CHAR3_SEG_B + LED_CHAR3_SEG_C + LED_CHAR3_SEG_D + LED_CHAR3_SEG_E + LED_CHAR3_SEG_F + LED_CHAR3_SEG_G,
  /* 9 */ LED_CHAR3_SEG_A + LED_CHAR3_SEG_B + LED_CHAR3_SEG_C + LED_CHAR3_SEG_D + LED_CHAR3_SEG_F + LED_CHAR3_SEG_G,
};


const unsigned long sevenSegDisplay::char4_7_seg[] = 
{ /* 0 */ LED_CHAR4_SEG_A + LED_CHAR4_SEG_B + LED_CHAR4_SEG_C + LED_CHAR4_SEG_D + LED_CHAR4_SEG_E + LED_CHAR4_SEG_F,
  /* 1 */ LED_CHAR4_SEG_B + LED_CHAR4_SEG_C, 
  /* 2 */ LED_CHAR4_SEG_A + LED_CHAR4_SEG_B + LED_CHAR4_SEG_G + LED_CHAR4_SEG_E + LED_CHAR4_SEG_D,
  /* 3 */ LED_CHAR4_SEG_A + LED_CHAR4_SEG_B + LED_CHAR4_SEG_G + LED_CHAR4_SEG_C + LED_CHAR4_SEG_D,
  /* 4 */ LED_CHAR4_SEG_F + LED_CHAR4_SEG_B + LED_CHAR4_SEG_G + LED_CHAR4_SEG_C,
  /* 5 */ LED_CHAR4_SEG_A + LED_CHAR4_SEG_F + LED_CHAR4_SEG_G + LED_CHAR4_SEG_C + LED_CHAR4_SEG_D,
  /* 6 */ LED_CHAR4_SEG_A + LED_CHAR4_SEG_F + LED_CHAR4_SEG_G + LED_CHAR4_SEG_C + LED_CHAR4_SEG_D + LED_CHAR4_SEG_E,
  /* 7 */ LED_CHAR4_SEG_A + LED_CHAR4_SEG_B + LED_CHAR4_SEG_C,
  /* 8 */ LED_CHAR4_SEG_A + LED_CHAR4_SEG_B + LED_CHAR4_SEG_C + LED_CHAR4_SEG_D + LED_CHAR4_SEG_E + LED_CHAR4_SEG_F + LED_CHAR4_SEG_G,
  /* 9 */ LED_CHAR4_SEG_A + LED_CHAR4_SEG_B + LED_CHAR4_SEG_C + LED_CHAR4_SEG_D + LED_CHAR4_SEG_F + LED_CHAR4_SEG_G,
};


    void sevenSegDisplay::updateDisplay(unsigned long newPattern)
    {
      // If we're already displaying the desired pattern, just return
      if (newPattern == m_currentDisplayedPattern)
        return;

      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, GET_BYTE(3,newPattern));
      shiftOut(dataPin, clockPin, MSBFIRST, GET_BYTE(2,newPattern));
      shiftOut(dataPin, clockPin, MSBFIRST, GET_BYTE(1,newPattern));
      shiftOut(dataPin, clockPin, MSBFIRST, GET_BYTE(0,newPattern));
      digitalWrite(latchPin, HIGH);      

      m_currentDisplayedPattern = newPattern;
    }

    sevenSegDisplay::sevenSegDisplay()
    {
      m_redLed = false;
      m_amberLed = false;
      m_greenLed = false;
      pinMode(latchPin, OUTPUT);
      pinMode(dataPin, OUTPUT);
      pinMode(clockPin, OUTPUT);
      // Set the current displayed pattern to something non-zero, so when we
      // call updateDisplay, it will initialize the shift registers
      m_currentDisplayedPattern = 0xFFFFFFFF;
      updateDisplay(0);
    }

    void sevenSegDisplay::setLedState(bool val, unsigned long flag)
    {
      unsigned long newPattern = m_currentDisplayedPattern;
      if (val){
        newPattern |= flag;
      }
      else{
        newPattern &= ~flag;
      }
      updateDisplay(newPattern);
      
    }
    
    void sevenSegDisplay::setRedLed(bool val){
      setLedState(val,LED_RED_LIGHT);
    }
    void sevenSegDisplay::setAmberLed(bool val){
      setLedState(val,LED_AMBER_LIGHT);
    }
    void sevenSegDisplay::setGreenLed(bool val){
      setLedState(val,LED_GREEN_LIGHT);
    }
    void sevenSegDisplay::setAux1Led(bool val){
      setLedState(val,LED_TOP_LEFT);
    }
    void sevenSegDisplay::setAux2Led(bool val){
      setLedState(val,LED_BOTTOM_LEFT);
    }
    void sevenSegDisplay::setColonLed(bool val){
      setLedState(val,LED_COLON);
    }


    void sevenSegDisplay::displaySeconds(int newSeconds)
    {
      int minutes = newSeconds / 60;
      int seconds = newSeconds % 60;

      if (minutes > 19)
      {
        Serial.print("Warning: time set to more than 19 minutes!! ");
        Serial.println(minutes);
        minutes = 19;
      }
      
      /* Clear any bits that are currently set */
      unsigned long led_val = m_currentDisplayedPattern & ~allCharacters;

      led_val |= char1_7_seg[minutes / 10];
      led_val |= char2_7_seg[minutes % 10];
      led_val |= char3_7_seg[seconds / 10];
      led_val |= char4_7_seg[seconds % 10];
//      Serial.print(newSeconds);
//      Serial.print(" seconds decoded as ");
//      Serial.print(minutes);
//      Serial.print(":");
//      Serial.print(seconds);
//      Serial.print(" = ");
//      Serial.print(minutes / 10);
//      Serial.print(" ");
//      Serial.print(minutes % 10);
//      Serial.print(":");
//      Serial.print(seconds / 10);
//      Serial.print(" ");
//      Serial.print(seconds % 10);
//      Serial.print(" ");
//      Serial.print(led_val,HEX);
//      Serial.print("  allCharacters=");
//      Serial.print(allCharacters,HEX);
//      Serial.print("  ");
//      Serial.print(char1_7_seg[minutes / 10],HEX);
//      Serial.print("  ");
//      Serial.print(char2_7_seg[minutes % 10],HEX);
//      Serial.print("  ");
//      Serial.print(char3_7_seg[seconds / 10],HEX);
//      Serial.print("  ");
//      Serial.print(char4_7_seg[seconds % 10],HEX);
//      Serial.println("  ");

      updateDisplay(led_val);
    }

    
