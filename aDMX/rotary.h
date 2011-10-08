#ifndef __ROTARY_H_
#define __ROTARY_H_

#include <inttypes.h>
#include "button.h"

class Rotary {
  uint8_t rotaryPinA;
  uint8_t rotaryPinB;
  uint8_t rotaryIdle;
  uint8_t rotaryActive;
  uint8_t rotaryStateWas;

  public:
    Rotary( uint8_t pinA, uint8_t pinB );
    int8_t read();
};

#endif
