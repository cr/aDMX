#ifndef __LED_H_
#define __LED_H_

#include <inttypes.h>

enum LedState { OFF, ON, FLASHED };

class Led {
  uint8_t ledPin;
  uint8_t ledState;
  uint16_t ledFlashLength;
  uint32_t ledChangeTime;

public:
  Led( uint8_t pin );
  void flash( void );
  void flash( uint16_t ms );
  void on( void );
  void off( void );
  void update( void );
};

#endif // __LED_H_
