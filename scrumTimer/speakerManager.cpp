
#include <Arduino.h>
#include "speakerManager.h"
#include "pitches.h"
#include "utils.h"


//Mario main theme intro
static const int speakerManagerClass::marioIntroNotes[] = { 0,
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0
};

static const int speakerManagerClass::marioIntroTempo[] = {0,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};



//char *song = "Looney:d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f";

#define MICKEY_QUARTER 8

static const int speakerManagerClass::mickeyMouseNotes[] = { 
  NOTE_C7, // Whos
  NOTE_C7, // The
  NOTE_C7, // lea-
  NOTE_C7, // der
  NOTE_C7, // of
  NOTE_C7, // the
  NOTE_C7, // club
  NOTE_C7, // thats
  NOTE_D7, // known
  NOTE_C7, // to
  NOTE_B6, // you
  NOTE_A6, // and
  NOTE_G6, // me
  0, // Rest
  NOTE_C7, // M
  NOTE_C7, // I
  NOTE_C7, // C
  NOTE_C7, // K
  NOTE_C7, // E
  NOTE_C7, // Y
  NOTE_E7, // M
  NOTE_C7, // O
  NOTE_D7, // U
  NOTE_B6, // S
  NOTE_C7, // E
//  NOTE_C7, // Mic-
//  NOTE_C7, // key
//  NOTE_A6, // Mouse!
};

static const int speakerManagerClass::mickeyMouseTempo[] = {
  MICKEY_QUARTER, // Whos
  MICKEY_QUARTER, // The
  MICKEY_QUARTER, // lea-
  MICKEY_QUARTER, // der
  MICKEY_QUARTER, // of
  MICKEY_QUARTER, // the
  MICKEY_QUARTER, // club
  MICKEY_QUARTER, // thats
  MICKEY_QUARTER, // known
  MICKEY_QUARTER, // to
  MICKEY_QUARTER, // you
  MICKEY_QUARTER, // and
  MICKEY_QUARTER/3, // me
  MICKEY_QUARTER, // Rest
  MICKEY_QUARTER, // M
  MICKEY_QUARTER, // I
  MICKEY_QUARTER/2, // C
  MICKEY_QUARTER, // K
  MICKEY_QUARTER, // E
  MICKEY_QUARTER/2, // Y
  MICKEY_QUARTER, // M
  MICKEY_QUARTER, // O
  MICKEY_QUARTER, // U
  MICKEY_QUARTER, // S
  MICKEY_QUARTER/4, // E
//  MICKEY_QUARTER/(1.5), // Mic-
//  MICKEY_QUARTER*2, // key
//  MICKEY_QUARTER/4, // Mouse
};

#define TWINKLE_QUARTER 8
#define TWINKLE_HALF (TWINKLE_QUARTER/2)


static const int speakerManagerClass::twinkleTwinkleNotes[] = { 
  NOTE_C6, // twin
  NOTE_C6, // kle
  NOTE_G6, // twin
  NOTE_G6, // klw
  NOTE_A6, // lit
  NOTE_A6, // tle
  NOTE_G6, // star
  NOTE_F6, // how
  NOTE_F6, // I
  NOTE_E6, // won
  NOTE_E6, // der
  NOTE_D6, // what
  NOTE_D6, // you
  NOTE_C6, // are
  NOTE_G6, // up
  NOTE_G6, // a
  NOTE_F6, // bove
  NOTE_F6, // the
  NOTE_E6, // world
  NOTE_E6, // so
  NOTE_D6, // high
  NOTE_G6, // like
  NOTE_G6, // a
  NOTE_F6, // dia
  NOTE_F6, // mond
  NOTE_E6, // in
  NOTE_E6, // the
  NOTE_D6, // sky
};

static const int speakerManagerClass::twinkleTwinkleTempo[] = { 
  TWINKLE_QUARTER, // twin
  TWINKLE_QUARTER, // kle
  TWINKLE_QUARTER, // twin
  TWINKLE_QUARTER, // klw
  TWINKLE_QUARTER, // lit
  TWINKLE_QUARTER, // tle
  TWINKLE_HALF, // star
  TWINKLE_QUARTER, // how
  TWINKLE_QUARTER, // I
  TWINKLE_QUARTER, // won
  TWINKLE_QUARTER, // der
  TWINKLE_QUARTER, // what
  TWINKLE_QUARTER, // you
  TWINKLE_HALF, // are
  TWINKLE_QUARTER, // up
  TWINKLE_QUARTER, // a
  TWINKLE_QUARTER, // bove
  TWINKLE_QUARTER, // the
  TWINKLE_QUARTER, // world
  TWINKLE_QUARTER, // so
  TWINKLE_HALF, // high
  TWINKLE_QUARTER, // like
  TWINKLE_QUARTER, // a
  TWINKLE_QUARTER, // dia
  TWINKLE_QUARTER, // mond
  TWINKLE_QUARTER, // in
  TWINKLE_QUARTER, // the
  TWINKLE_HALF, // sky
};


static const int speakerManagerClass::marioDeathTempo[] = {0,
  17,                                                  // Array for Death sound effect & song
  32, 32, 16, 4, 2, 8, 8, 8, 8, 6, 
  6, 6, 8, 8, 8, 8, 8};
static const int speakerManagerClass::marioDeathNotes[] = {0,                                                  // Array for Death sound effect & song
  NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_H, NOTE_H, NOTE_B3, NOTE_F4, NOTE_H, NOTE_F4, NOTE_F4, 
  NOTE_E4, NOTE_D4,  NOTE_C4, NOTE_E3, NOTE_H, NOTE_E3, NOTE_C3 };
const int speakerManagerClass::marioGameoverNotes[] = {0,                                               // Array for Game over song
  NOTE_C4, NOTE_H, NOTE_H, NOTE_G3, NOTE_H, NOTE_E3, NOTE_A3, NOTE_B3, NOTE_A3, NOTE_GS3, NOTE_AS3, 
  NOTE_GS3, NOTE_G3, NOTE_F3, NOTE_G3};
const int speakerManagerClass::marioGameoverTempo[] = {0,                                               // Array for Game over song
   8*2, 8*2, 8*2, 8*2, 4*2, 4*2, 6*2, 6*2, 6*2, 6*2, 6*2, 
  6*2, 8*2, 8*2, 4*2};


const int speakerManagerClass::marioCoinNotes[] = {0, NOTE_B5, NOTE_E6, 0};
const int speakerManagerClass::marioCoinTempo[] = {0, 8*2, 4, 0};

void speakerManagerClass::playMario()
{ 
  Serial.println("Start playing Mario!");
  m_activeSongPos = 0;
  m_lastNoteChangeMillis = ULONG_MAX;
  m_activeSongCount = ELEMENTCNT(marioIntroTempo);
  m_activeSongNotes = marioIntroNotes;
  m_activeSongTempo = marioIntroTempo;
}

void speakerManagerClass::playGameOver()
{ 
  Serial.println("Game Over Man!!");
  m_activeSongPos = 0;
  m_lastNoteChangeMillis = ULONG_MAX;
  m_activeSongCount = ELEMENTCNT(marioGameoverNotes);
  m_activeSongNotes = marioGameoverNotes;
  m_activeSongTempo = marioGameoverTempo;
}

void speakerManagerClass::playMickeyMouse()
{ 
    Serial.println("Start playing Mickey!");
    m_activeSongPos = 1;
    m_lastNoteChangeMillis = ULONG_MAX;
    m_activeSongCount = ELEMENTCNT(mickeyMouseTempo);
    m_activeSongNotes = mickeyMouseNotes;
    m_activeSongTempo = mickeyMouseTempo;
}

void speakerManagerClass::playTwinkelTwinkle()
{ 
    Serial.println("Start playing Twinkle!");
    m_lastNoteChangeMillis = ULONG_MAX;
    m_activeSongPos = -1;
    m_activeSongCount = ELEMENTCNT(twinkleTwinkleTempo);
    m_activeSongNotes = twinkleTwinkleNotes;
    m_activeSongTempo = twinkleTwinkleTempo;
}

void speakerManagerClass::playCoin()
{ 
  Serial.println("Got a coin!!");
  m_activeSongPos = 0;
  m_lastNoteChangeMillis = ULONG_MAX;
  m_activeSongCount = ELEMENTCNT(marioCoinNotes);
  m_activeSongNotes = marioCoinNotes;
  m_activeSongTempo = marioCoinTempo;
}

void speakerManagerClass::stop()
{
  noTone(m_pinNumber);
  m_currentTone = 0;
  m_activeSongCount = 0;
  m_activeSongPos = 0;
}

void speakerManagerClass::update(bool muted)
{

  if(muted || (m_activeSongPos >= m_activeSongCount))
  {
      stop();
  }
  else if (m_activeSongPos < m_activeSongCount)
  {
    // to calculate the note duration, take one second divided by the note type.
    //       e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    m_noteDurationMillis = (1000 / m_activeSongTempo[m_activeSongPos])*2;

    // If the elapsed time since last note change > note duration
    if( (m_lastNoteChangeMillis == ULONG_MAX) || ((millis() - m_lastNoteChangeMillis) > m_noteDurationMillis))
    {
      // Advance to the next note
      m_activeSongPos++;
      m_lastNoteChangeMillis = millis();
      m_currentTone = m_activeSongNotes[m_activeSongPos];
      if(m_currentTone == 0)
      {
              noTone(m_pinNumber);
      }
      else
      {
      tone(m_pinNumber, m_currentTone);
      }
    }
    if((millis() - m_lastNoteChangeMillis) > (2*m_noteDurationMillis)/3)
    {
      noTone(m_pinNumber);
    }
  }
  else
  {
    m_activeSongPos = 0;
  }
}

speakerManagerClass::speakerManagerClass(int pwmPinNum)
{
  m_pinNumber = pwmPinNum;
  pinMode(m_pinNumber, OUTPUT);
  noTone(m_pinNumber);
  m_currentTone = 0;
}
