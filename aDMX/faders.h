#ifndef __FADERS_H_
#define __FADERS_H_

#include <inttypes.h>

class AutoFader {
  float faderHoldRatio;
  uint32_t faderStartTime;
  float faderBeatPeriod;
  uint32_t faderBeatPeriodMillis;
  float faderFadeTime;
  float faderHoldTime;
  uint8_t faderColorTable[16][3];
  uint8_t faderColorCount;

private:
  uint32_t periodTime( void );
  uint32_t beatTime( void );
  uint8_t beatColor( void );
  uint8_t linearFade( uint8_t start_value, uint8_t end_value, float t, float length );

public:
  float BPM;
  uint8_t R, G, B;
  AutoFader( float bpm, float hold_ratio );
  void setTiming( float bpm, float hold_ratio );
  void setColorCount( uint8_t count );
  void setColor( uint8_t nr, uint8_t r, uint8_t g, uint8_t b );
  void update( void );

};

#endif // __FADERS_H_

