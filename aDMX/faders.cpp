#include "faders.h"
#include <WProgram.h>


float fadeLinear( float t, float tFadeIn, float tUp, float tFadeOut, float tDown ) {
  if( t < tFadeIn ) {
    return t / tFadeIn ;
  } else if( t < tFadeIn + tUp ) {
    return 1.0;
  } else if( t < tFadeIn + tUp + tFadeOut ) {
    return 1.0 - ( t - tFadeIn - tUp ) / tFadeOut;
  } else {
    return 0.0;
  }
}

AutoFaderRGB::AutoFaderRGB( float bpm, float holdRatio ) {
  faderStartTime = millis();
  faderRGBPeriod = 1.0;
  faderRGBPeriodMillis = 1000;
  timing( bpm, holdRatio );
}

uint32_t AutoFaderRGB::periodTime( void ) {
  return ( millis() - faderStartTime ) % faderRGBPeriodMillis;
}

void AutoFaderRGB::timing( float bpm, float holdRatio ) {
  // keep relative timing position
  uint32_t now = millis();
  uint32_t relativeTime = periodTime();
  float periodPercent = ( (float) relativeTime / 1000.0 ) / faderRGBPeriod;
  
  BPM = bpm;
  faderHoldRatio = holdRatio;
  faderPeriod = 60.0 / BPM;
  faderRGBPeriod = 3.0 * faderPeriod;
  faderRGBPeriodMillis = (uint32_t) ( 1000.0 * faderRGBPeriod );
  faderFadeTime = faderPeriod / ( faderHoldRatio + 1.0 );
  faderHoldTime = faderHoldRatio * faderFadeTime;

  faderStartTime = now - (uint32_t) (1000.0*periodPercent*faderRGBPeriod);
}

void AutoFaderRGB::update( void ) {
  uint32_t now = periodTime();
  float t = ( float ) now / 1000.0;
  R = fadeLinear( t, faderFadeTime, faderHoldTime, faderFadeTime, 2 * faderHoldTime + faderFadeTime );
  t -= faderPeriod;
  if( t < 0.0 ) t += faderRGBPeriod;
  G = fadeLinear( t, faderFadeTime, faderHoldTime, faderFadeTime, 2 * faderHoldTime + faderFadeTime );
  t -= faderPeriod;
  if( t < 0.0 ) t += faderRGBPeriod;
  B = fadeLinear( t, faderFadeTime, faderHoldTime, faderFadeTime, 2 * faderHoldTime + faderFadeTime );
  
}

