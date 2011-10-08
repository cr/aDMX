#include "debug.h"
#include "lowlevel.h"
#include <WProgram.h>
#include <pins_arduino.h>

static uint8_t debugPin;
volatile uint8_t * debugPort;
static uint8_t debugBitMask;

void debugSetup( uint8_t pin ) {
  debugPin = pin;
  debugPort = portOutputRegister( digitalPinToPort( pin ) );
  debugBitMask = digitalPinToBitMask( pin );
  pinMode( debugPin, OUTPUT );
  digitalWrite( debugPin, LOW );
}

void debug() {
//  sbi( PORTG,5 );
//  nop32;
//  nop16;
//  nop8;
//  nop;
//  cbi( PORTG,5 );
  digitalWrite( debugPin, HIGH );
  delayMicroseconds(1);
  digitalWrite( debugPin, LOW );

//  *debugPort = *debugPort | debugBitMask;
//  nop32; nop16; nop2;
//  *debugPort = *debugPort & ~debugBitMask;
//  bitout( debugPort, debugBitMask, 1 );
//  bitout( debugPort, debugBitMask, 0 );
//  bitout( debugPort, debugBitMask, 1 );
//  bitout( debugPort, debugBitMask, 0 );
}

