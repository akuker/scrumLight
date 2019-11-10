//**************************************************************//
//  Name    : shiftOutCode, Dual One By One                           //
//  Author  : Carlyn Maw, Tom Igoe                               //
//  Date    : 25 Oct, 2006                                       //
//  Version : 1.0                                                //
//  Notes   : Code for using a 74HC595 Shift Register            //
//          : to count from 0 to 255                             //
//**************************************************************//
//Pin connected to ST_CP of 74HC595
const int latchPin = 3;
//Pin connected to SH_CP of 74HC595
const int clockPin = 4;
//Pin connected to DS of 74HC595
const int dataPin = 5;
// Pin connected to the speaker output
const int speakerPin=6;

const int buttons[] = {5,14,15,16,17,18,19,7,8};
const int button_count = sizeof(buttons) / sizeof(buttons[0]);


#define GET_BYTE(n,val) ((val >> (8*n)) & 0xff)

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



unsigned long char2_7_seg[] = 
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


unsigned long char3_7_seg[] = 
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


unsigned long char4_7_seg[] = 
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

int z=0;

void updateShiftRegister(unsigned long leds)
{
  int arduinoShift = 0;
  if(arduinoShift){
     digitalWrite(latchPin, LOW);
     shiftOut(dataPin, clockPin, LSBFIRST, GET_BYTE(3,leds));
     shiftOut(dataPin, clockPin, LSBFIRST, GET_BYTE(2,leds));
     shiftOut(dataPin, clockPin, LSBFIRST, GET_BYTE(1,leds));
     shiftOut(dataPin, clockPin, LSBFIRST, GET_BYTE(0,leds));
     digitalWrite(latchPin, HIGH);
  }
  else
  {
     digitalWrite(latchPin, LOW);
     shiftOutLocal(dataPin, clockPin, GET_BYTE(3,leds));
     shiftOutLocal(dataPin, clockPin, GET_BYTE(2,leds));
     shiftOutLocal(dataPin, clockPin, GET_BYTE(1,leds));
     shiftOutLocal(dataPin, clockPin, GET_BYTE(0,leds));
     digitalWrite(latchPin, HIGH);    
  }


}


void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  Serial.begin(9600); // open the serial port at 9600 bps:

}

void loop() {



  Serial.println("In Loop");
  updateShiftRegister(0xAAAAAAAA);
  delay(500);
  updateShiftRegister(0x55555555);
  delay(500);

  unsigned long value = 0;
  for(int x=0; x<2; x++)
  {
    for(int i=0; i<32; i++)
    {
      value = value ^ ((unsigned long)2 << i);
      Serial.println(value,HEX);
      updateShiftRegister(value);
      delay(50);
    }
  }

  for(int i=0; i<10; i++)
  {
    updateShiftRegister(char2_7_seg[i] + char3_7_seg[i] + char4_7_seg[i]);
    delay(200);

    
  }


  if (!z)
{
  tone(speakerPin,530);
  Serial.println("tone");
  z=1;
}
else
{
  noTone(speakerPin);
  z=0;
}

  
  return;

//  //function that blinks all the LEDs
//  //gets passed the number of blinks and the pause time
//  blinkAll_2Bytes(1,500); 
//
//  // light each pin one by one using a function A
//  for (int j = 0; j < 8; j++) {
//    //ground latchPin and hold low for as long as you are transmitting
//    digitalWrite(latchPin, 0);
//    //red LEDs
//    lightShiftPinA(7-j);
//    //green LEDs
//    lightShiftPinA(j);
//    //return the latch pin high to signal chip that it 
//    //no longer needs to listen for information
//    digitalWrite(latchPin, 1);
//    delay(1000);
//  }
//
//  // light each pin one by one using a function A
//  for (int j = 0; j < 8; j++) {
//    //ground latchPin and hold low for as long as you are transmitting
//    digitalWrite(latchPin, 0);
//    //red LEDs
//    lightShiftPinB(j);
//    //green LEDs
//    lightShiftPinB(7-j);
//    //return the latch pin high to signal chip that it 
//    //no longer needs to listen for information
//    digitalWrite(latchPin, 1);
//    delay(1000);
//  } 

}

////This function uses bitwise math to move the pins up
//void lightShiftPinA(int p) {
//  //defines a local variable
//  int pin;
//
//  //this is line uses a bitwise operator
//  //shifting a bit left using << is the same
//  //as multiplying the decimal number by two. 
//  pin = 1<< p;
//
//  //move 'em out
//  shiftOut(dataPin, clockPin, pin);   
//  shiftOut(dataPin, clockPin, pin);   
//  shiftOut(dataPin, clockPin, pin);   
//    shiftOut(dataPin, clockPin, pin);   
//
//}

////This function uses that fact that each bit in a byte
////is 2 times greater than the one before it to
////shift the bits higher
//void lightShiftPinB(int p) {
//  //defines a local variable
//  int pin;
//
//  //start with the pin = 1 so that if 0 is passed to this
//  //function pin 0 will light. 
//  pin = 1;
//
//  for (int x = 0; x < p; x++) {
//    pin = pin * 2; 
//  }
//  //move 'em out
//  shiftOut(dataPin, clockPin, pin);   
//  shiftOut(dataPin, clockPin, pin);   
//  shiftOut(dataPin, clockPin, pin);   
//  shiftOut(dataPin, clockPin, pin);   
//
//}


// the heart of the program
void shiftOutLocal(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}

//
////blinks both registers based on the number of times you want to 
////blink "n" and the pause between them "d"
////starts with a moment of darkness to make sure the first blink
////has its full visual effect.
//void blinkAll_2Bytes(int n, int d) {
//  digitalWrite(latchPin, 0);
//  shiftOut(dataPin, clockPin, 0);
//  shiftOut(dataPin, clockPin, 0);
//  digitalWrite(latchPin, 1);
//  delay(200);
//  for (int x = 0; x < n; x++) {
//    digitalWrite(latchPin, 0);
//    shiftOut(dataPin, clockPin, 255);
//    shiftOut(dataPin, clockPin, 255);
//    digitalWrite(latchPin, 1);
//    delay(d);
//    digitalWrite(latchPin, 0);
//    shiftOut(dataPin, clockPin, 0);
//    shiftOut(dataPin, clockPin, 0);
//    digitalWrite(latchPin, 1);
//    delay(d);
//  }
//}
