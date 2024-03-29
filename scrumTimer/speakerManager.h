#pragma once
#include <Arduino.h>
#include "limits.h"

class speakerManagerClass {
  public:
    speakerManagerClass(int pwmPinNum);
    void update(bool muted);
    void playMario();
    void playTwinkelTwinkle();
    void playMickeyMouse();
    void playGameOver();
    void playCoin();
    void stop();

  private:
    static const int marioIntroNotes[];
    static const int marioIntroTempo[];
    static const int mickeyMouseNotes[];
    static const int mickeyMouseTempo[];
    static const int twinkleTwinkleNotes[];
    static const int twinkleTwinkleTempo[];
    static const int marioDeathNotes[];
    static const int marioDeathTempo[];
    static const int marioGameoverNotes[];
    static const int marioGameoverTempo[];
    static const int marioCoinNotes[];
    static const int marioCoinTempo[];
    
    
    int m_pinNumber = 0;
    int m_currentTone = 0;

    unsigned long m_lastNoteChangeMillis = ULONG_MAX;
    unsigned long m_noteDurationMillis = 0;
    int m_activeSongPos = 0;
    int m_activeSongCount = 0;
    int *m_activeSongNotes;
    int *m_activeSongTempo;
};

extern speakerManagerClass *speaker;
