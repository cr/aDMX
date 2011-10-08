#ifndef __DMX_H_
#define __DMX_H_

#include <inttypes.h>

#define DMX_MSG_LEN 512

void dmxSetup( uint16_t channels, uint8_t txPin, uint8_t drvEnablePin, uint8_t rxPin, uint8_t rcvEnablePin );
void dmxModeDisable( void );
void dmxModeReceive( void );
void dmxModeSend( void );
void dmxClear( void );
void dmxSet( int channel, uint8_t value );
void dmxSend( void );

extern uint8_t dmxMsg[DMX_MSG_LEN];

class DmxBus {
  public:
    void begin( uint16_t channels, uint8_t txPin, uint8_t drvEnablePin, uint8_t rxPin, uint8_t rcvEnablePin );
    void clear( void );
    void set( uint16_t channel, uint8_t value );
    void send( void );
    void end();
};
extern DmxBus Dmx;

class DmxClient {
  uint16_t clientChannel;
  DmxBus * bus;
  public:
    DmxClient( DmxBus * dmx, uint16_t channel );
    void set();
    void send();
};

#endif //__DMX_H_
