#include "button.h"
#include <WProgram.h>

Button::Button( uint8_t pin, uint16_t hold ) {
  buttonPin = pin;
  holdTime = hold;
  stableState = LOW;
  stableStateWas = LOW;
  bounceStateWas = LOW;
  //bounceTime = micros(); 
  pinMode( buttonPin, INPUT );
}

Button::Button( uint8_t pin ) {
  buttonPin = pin;
  holdTime = 500;
  stableState = LOW;
  stableStateWas = LOW;
  bounceStateWas = LOW;
  //bounceTime = micros(); 
  pinMode( buttonPin, INPUT );
}

void Button::pullup() {
  digitalWrite( buttonPin, HIGH );
  stableState = HIGH;
  stableStateWas = HIGH;
}

void Button::pulldown() {
  digitalWrite( buttonPin, LOW );
  stableState = LOW;
  stableStateWas = LOW;
}

uint8_t Button::read() {
  stableStateWas = stableState;
  uint8_t bounceState = digitalRead( buttonPin );
  if( bounceStateWas != bounceState ) {
    bounceTime = micros();
    bounceStateWas = bounceState;
  } else if( bounceState != stableState ) {
    if( micros() - bounceTime > holdTime ) {
      stableState = bounceState;
    }
  }
  return stableState;
}

uint8_t Button::previous() {
  return stableStateWas;
}

uint8_t Button::rising() {
  if( read() == HIGH ) {
    if( stableStateWas == LOW )
      return true;
  }
  return false;
}

uint8_t Button::falling() {
  if( read() == LOW ) {
    if( stableStateWas == HIGH )
      return true;
  }
  return false;
}

