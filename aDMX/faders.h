#ifndef __FADERS_H_
#define __FADERS_H_

#include <inttypes.h>

class AutoFaderRGB {
  float faderHoldRatio;
  uint32_t faderStartTime;
  float faderPeriod;
  float faderRGBPeriod;
  uint32_t faderRGBPeriodMillis;
  float faderFadeTime;
  float faderHoldTime;

private:
  uint32_t periodTime( void );

public:
  float BPM;
  float R, G, B;
  AutoFaderRGB( float bpm, float holdRatio );
  void timing( float bpm, float holdRatio );
  void update( void );

};


#endif // __FADERS_H_


