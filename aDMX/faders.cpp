#include "faders.h"
#include <WProgram.h>


AutoFader::AutoFader( float bpm, float hold_ratio ) {
  setColorCount( 3 );
  setColor( 0, 255, 0, 0 );
  setColor( 1, 0, 255, 0 );
  setColor( 2, 0, 0, 255 );
  faderStartTime = millis();
  faderBeatPeriod = 1.0;
  faderBeatPeriodMillis = 1000;
  setTiming( bpm, hold_ratio );
}

void AutoFader::setColorCount( uint8_t count ) {
  faderColorCount = count;
}

void AutoFader::setColor( uint8_t nr, uint8_t r, uint8_t g, uint8_t b ) {
  faderColorTable[nr][0] = r;
  faderColorTable[nr][1] = g;
  faderColorTable[nr][2] = b;
}

uint32_t AutoFader::beatTime( void ) {
  return ( millis() - faderStartTime ) % faderBeatPeriodMillis;
}

uint8_t AutoFader::beatColor( void ) {
  return ( ( millis() - faderStartTime ) / faderBeatPeriodMillis ) % faderColorCount;
}

void AutoFader::setTiming( float bpm, float hold_ratio ) {
  // keep relative timing position
  uint32_t now = millis();
  uint32_t relative_time = beatTime();
  uint8_t current_color = beatColor();

  float period_fraction = (float) relative_time / ( float ) faderBeatPeriodMillis;

  BPM = bpm;
  faderBeatPeriod = 60.0 / BPM;
  faderBeatPeriodMillis = (uint32_t) ( 1000.0 * faderBeatPeriod );
  faderHoldRatio = hold_ratio;
  faderFadeTime = faderBeatPeriod / ( hold_ratio + 1.0 );
  faderHoldTime = faderBeatPeriod - faderFadeTime;

  faderStartTime = now - (uint32_t) ( (period_fraction + (float)current_color) * (float)faderBeatPeriodMillis );
}

uint8_t AutoFader::linearFade( uint8_t start_value, uint8_t end_value, float t, float length ) { 
  if( t < 0.0 ) {
    return start_value;
  } 
  else if( t < length ) {
    return start_value + ( end_value - start_value ) * t / length;
  } 
  else {
    return end_value;
  }
}

void AutoFader::update( void ) {
  uint32_t relative_time = beatTime();
  uint8_t current_color = beatColor();
  uint8_t next_color = ( current_color + 1 ) % faderColorCount;

  float fade_time = ( float ) ( relative_time / 1000.0) - faderHoldTime;

  R = linearFade( faderColorTable[current_color][0], faderColorTable[next_color][0], fade_time, faderFadeTime ); 
  G = linearFade( faderColorTable[current_color][1], faderColorTable[next_color][1], fade_time, faderFadeTime ); 
  B = linearFade( faderColorTable[current_color][2], faderColorTable[next_color][2], fade_time, faderFadeTime ); 
}


