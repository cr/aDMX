#include "led.h"
#include <WProgram.h>


Led::Led( uint8_t pin ) {
  ledPin = pin;
  pinMode( pin, OUTPUT );
  ledChangeTime = millis();
  digitalWrite( pin, LOW );
  ledFlashLength = 100;
  ledState = OFF;
}

void Led::flash( void ) {
  ledChangeTime = millis();
  ledState = FLASHED;
  update();
}

void Led::flash( uint16_t ms ) {
  ledChangeTime = millis();
  ledState = FLASHED;
  ledFlashLength = ms;
  update();
}

void Led::on( void ) {
  ledChangeTime = millis();
  ledState = ON;
  update();
}

void Led::off( void ) {
  ledChangeTime = millis();
  ledState = OFF;
  update();
}

void Led::update( void ) {
  switch( ledState ) {

  case OFF:
    digitalWrite( ledPin, LOW );
    break;

  case ON:
    digitalWrite( ledPin, HIGH );
    break;

  case FLASHED:
    if( millis() - ledChangeTime < ledFlashLength ) {
      digitalWrite( ledPin, HIGH );
    } 
    else {
      digitalWrite( ledPin, LOW );
      ledState = OFF;
    }
    break;

  default:
    digitalWrite( ledPin, LOW );
    ledState = OFF;

  }
}



