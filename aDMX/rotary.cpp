
#include <WProgram.h>
#include "rotary.h"
#include "button.h"

Rotary::Rotary( uint8_t pinA, uint8_t pinB ) {
  rotaryPinA = pinA;
  rotaryPinB = pinB;

  pinMode( rotaryPinA, INPUT );
  pinMode( rotaryPinB, INPUT );

  digitalWrite( rotaryPinA, HIGH ); // Enable pull-up
  digitalWrite( rotaryPinB, HIGH ); // Enable pull-up

  rotaryIdle = HIGH;
  rotaryActive = LOW;
  rotaryStateWas = rotaryIdle;
}

int8_t Rotary::read() {
  uint8_t stateIs = digitalRead( rotaryPinA );
  if( ( rotaryStateWas == rotaryIdle ) && ( stateIs == rotaryActive ) ) {
    if( digitalRead( rotaryPinB ) == LOW ) {
      rotaryStateWas = stateIs;
      return -1;
    } else {
      rotaryStateWas = stateIs;
      return 1;
    }
  }
  rotaryStateWas = stateIs;
  return 0;
}

