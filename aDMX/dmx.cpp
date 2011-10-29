#include "lowlevel.h"
#include "debug.h"
#include "dmx.h"
#include <WProgram.h>
#include <pins_arduino.h>

#define DMX_BIT_HOLD 4
#define DMX_RESET_HOLD (88+30)
#define DMX_RESET_WAIT 8
#define DMX_FRAME_DELAY 8

uint8_t dmxMsg[DMX_MSG_LEN];

static uint16_t dmxChannels;
static uint8_t dmxRcvPin;
static uint8_t dmxDrvPin;
static uint8_t dmxRxPin;
static uint8_t dmxTxPin;
volatile uint8_t * dmxTxPort;
static uint8_t dmxTxBitMask;

void dmxSetup( uint16_t channels, uint8_t txPin, uint8_t drvEnablePin, uint8_t rxPin, uint8_t rcvEnablePin ) {

  dmxChannels = channels;
  dmxRcvPin = rcvEnablePin;
  dmxDrvPin = drvEnablePin;
  dmxRxPin = rxPin;
  dmxTxPin = txPin;
  dmxTxPort = portOutputRegister( digitalPinToPort( txPin ) );
  dmxTxBitMask = digitalPinToBitMask( txPin );

  if( dmxDrvPin > 0 )
    pinMode( dmxDrvPin, OUTPUT );
  if( dmxRcvPin > 0 )
    pinMode( dmxRcvPin, OUTPUT );
  if( dmxRxPin > 0 ) 
    pinMode( dmxRxPin, INPUT );
  if( dmxTxPin > 0 ) 
    pinMode( dmxTxPin, OUTPUT );

  //dmxModeDisable();
  //dmxModeSend();

  dmxClear();
}

void dmxModeDisable() {
  if( dmxDrvPin > 0 ) 
    digitalWrite( dmxDrvPin, LOW ); // DE is inactive LOW
  if( dmxRcvPin > 0 ) 
    digitalWrite( dmxRcvPin, HIGH ); // RE/ is inactive HIGH
  if( dmxTxPin > 0 ) 
    digitalWrite( dmxTxPin, LOW );
}

void dmxModeReceive() {
  if( dmxDrvPin > 0 ) 
    digitalWrite( dmxDrvPin, LOW ); // DE is inactive LOW
  if( dmxRcvPin > 0 ) 
    digitalWrite( dmxRcvPin, LOW ); // RE/ is active LOW
  if( dmxTxPin > 0 ) 
    digitalWrite( dmxTxPin, LOW );
}

void dmxModeSend() {
  if( dmxRcvPin > 0 ) 
    digitalWrite( dmxRcvPin, HIGH ); // RE/ is inactive HIGH
  if( dmxDrvPin > 0 ) 
    digitalWrite( dmxDrvPin, HIGH ); // DE is active HIGH
  if( dmxTxPin > 0 ) 
    digitalWrite( dmxTxPin, HIGH );  // TX idle HIGH
}

void dmxSendReset() {
  digitalWrite( dmxTxPin, LOW );
  delayMicroseconds( DMX_RESET_HOLD );
  digitalWrite( dmxTxPin, HIGH );
  delayMicroseconds( DMX_RESET_WAIT );
}

void dmxBitout( volatile uint8_t * p, uint8_t m, uint8_t v ) {
  int dly = 1;
  uint8_t notm = ~m;
  if( v==0 ) {
    *p &= notm;
    //nop32; nop16; nop8; nop4;
    //nop32; nop16; nop4; nop2;
    //nop32; nop8; nop4; nop2; nop;
    // almost: nop32; nop16;
    nop32; 
    nop8; 
    nop4;
  } 
  else {
    *p |= m;
    //nop32; nop16; nop2;
    //nop32; nop16;
    // almost: nop32; nop8; nop2;
    nop32; 
    nop8; 
    nop4;
  }
}


volatile void dmxSendByte( uint8_t c ) {  
  uint16_t v = c;

  v <<= 1; // Start bit 0
  v |= 0x600; // Two stop bits 1

  cli();
  while( v != 0 ) {
    dmxBitout( dmxTxPort, dmxTxBitMask, v&1 ); 
    v >>= 1;
  }

  sei();
  delayMicroseconds( DMX_FRAME_DELAY );
}

void dmxClear() {
  int channel;
  for( channel=0 ; channel<dmxChannels ; channel++ )
    dmxMsg[channel] = 0; 
}

void dmxSet( int channel, uint8_t value ) {
  dmxMsg[channel-1] = value;
}

void dmxSend() {
  uint8_t * msg = dmxMsg;
  uint16_t len = dmxChannels;

  dmxModeSend();

  dmxSendReset();
  dmxSendByte( 0 );
  debug();
  while( len-- > 0 ) dmxSendByte( *(msg++) );
  delayMicroseconds( 1000 );

  // dmxModeDisable();
}

void DmxBus::begin( uint16_t channels, uint8_t txPin, uint8_t drvEnablePin, uint8_t rxPin, uint8_t rcvEnablePin ) {
  dmxSetup( channels, txPin, drvEnablePin, rxPin, rcvEnablePin );
}

void DmxBus::clear() {
  dmxClear();
}

void DmxBus::set( uint16_t channel, uint8_t value ) {
  dmxSet( channel, value );
}

void DmxBus::send() {
  dmxSend();
}

void DmxBus::end() {
  dmxModeDisable();
}

DmxBus Dmx;

/*
DmxClient::DmxClient( DmxBus * bus, uint16_t channel ) {
 dmxClientBus = bus;
 dmxClientChannel = channel;
 }
 
 DmxClient::set() {
 
 }
 
 */

