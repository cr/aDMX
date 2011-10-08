#ifndef __BUTTON_H_
#define __BUTTON_H_

#include <inttypes.h>

class Button {
    uint8_t buttonPin;
    uint8_t bounceStateWas;
    uint8_t stableState;
    uint8_t stableStateWas;
    uint8_t stableWas;
    uint16_t holdTime;
    uint32_t bounceTime;
    
  public:
    Button( uint8_t pin, uint16_t hold );
    Button( uint8_t pin );
    void pullup();
    void pulldown();
    uint8_t read();
    uint8_t previous();
    uint8_t rising();
    uint8_t falling();
};

#endif // __BUTTON_H_
