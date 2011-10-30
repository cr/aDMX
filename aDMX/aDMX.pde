#include "dmx.h"
#include "debug.h"
#include "rotary.h"
#include "faders.h"
#include "button.h"

//#define TX_OUT  40
//#define DRV_ENA 46
//#define RX_IN   0
//#define RCV_ENA 0
//#define DEBUG_OUT 52

// Pins for v1
//#define RX_IN 7
//#define TX_OUT 8
//#define DRV_ENA 9
//#define RCV_ENA 0
//#define ROTARY_A 5
//#define ROTARY_B 6
//#define BUTTON_A 4
//#define BUTTON_B 3
//#define BUTTON_R 2
//#define LED_A 10
//#define LED_B 11
//#define DEBUG_OUT 12

// Pins for v2
#define RX_IN 7
#define TX_OUT 8
#define DRV_ENA 9
#define RCV_ENA 0
#define ROTARY_A 3
#define ROTARY_B 4
#define BUTTON_A A2
#define BUTTON_B A3
#define BUTTON_R A4
#define LED_A 5
#define LED_B 6
#define DEBUG_OUT 12


uint8_t R, G, B;
uint16_t H, S, V;

void hsv_to_rgb( float Hin, float Sin, float Vin ) {
  // Converts HSV color space into RGB color space.
  // See http://en.wikipedia.org/wiki/HSL_and_HSV

  float V = Vin*255.0;
  int H = (int) Hin % 360;
  int hi = H/60;
  float f = H/60.0-hi;

  int p = V*(1.0-Sin);
  int q = V*(1.0-Sin*f);
  int t = V*(1.0-Sin*(1.0-f));

  switch( hi ) {  
  case 0:
  case 6:
    R = V; 
    G = t; 
    B = p;
    break;
  case 1:
    R = q; 
    G = V; 
    B = p;
    break;
  case 2:
    R = p; 
    G = V; 
    B = t;
    break;
  case 3:
    R = p; 
    G = q; 
    B = V;
    break;
  case 4:
    R = t; 
    G = p; 
    B = V;
    break;
  case 5:
    R = V; 
    G = p; 
    B = q;
    break;
  }
}


void clearLamps() {
  Dmx.clear();
}

void setLamp( int offset, uint8_t ch1, uint8_t ch2, uint8_t ch3, uint8_t ch4, uint8_t ch5 ) {
  Dmx.set( offset+0, ch1 );
  Dmx.set( offset+1, ch2 );
  Dmx.set( offset+2, ch3 );
  Dmx.set( offset+3, ch4 );
  Dmx.set( offset+4, ch5 );
}

void sendLamps() {
  Dmx.send();
}


void setup() {
  Serial.begin( 115200 );

  pinMode( LED_A, OUTPUT );
  digitalWrite( LED_A, HIGH );
  pinMode( LED_B, OUTPUT );
  digitalWrite( LED_B, HIGH );

  debugSetup( DEBUG_OUT );
  Dmx.begin( 128, TX_OUT, DRV_ENA, RX_IN, RCV_ENA );

  clearLamps();
  sendLamps();
  delay( 2000 );

  digitalWrite( LED_A, LOW );
  digitalWrite( LED_B, LOW );

}


void loop() {

  uint8_t pR=0, pG=0, pB=0, nR, nG, nB;

  float bpm = 120.0;
  float ratio = 3.0;
  AutoFader fader ( bpm, ratio );
  fader.setColorCount( 6 );
  fader.setColor( 0, 255, 0, 0 );
  fader.setColor( 1, 127, 127, 0 );
  fader.setColor( 2, 0, 255, 0 );
  fader.setColor( 3, 0, 127, 127 );
  fader.setColor( 4, 0, 0, 255 );
  fader.setColor( 5, 127, 0, 127 );

  Rotary rotary ( ROTARY_A, ROTARY_B );
  Button button ( BUTTON_R );
  button.pullup();
  Button buttonA ( BUTTON_A );
  buttonA.pullup();
  Button buttonB ( BUTTON_B );
  buttonB.pullup();

  float angle = 0.0;
  float sat = 1.0;
  float value = 1.0;
  int8_t d;
  uint8_t state = 0;
  uint32_t last = millis();
  uint8_t blast = LOW;
  uint8_t now;

  while( 1 ) {

    if( button.falling() ) {
      //state = ((state-2)+1) % 3 + 2;
      state = (state+1) % 2;
      Serial.println( state, DEC );
    }

    d = rotary.read();
    if( d != 0 ) {

      switch( state ) {

      case 0:
        if( d > 0 )
          bpm *= 1.05;
        else
          bpm /= 1.05;
        fader.setTiming( bpm, ratio );
        Serial.print( bpm );
        Serial.write( ',' );
        Serial.println( ratio );
        break;

      case 1:
        if( d > 0 )
          ratio *= 1.05;
        else
          ratio /= 1.05;
        fader.setTiming( bpm, ratio );
        Serial.print( bpm );
        Serial.write( ',' );
        Serial.println( ratio );
        break;

      case 2:
        angle = (angle + d);
        if( angle < 0.0 ) angle += 360.0;
        if( angle > 360.0 ) angle -= 360.0;
        hsv_to_rgb( angle, sat, value );
        setLamp( 1, 0, R, G, B, 0 );
        setLamp( 65, 0, R, G, B, 0 );
        sendLamps();
        Serial.print( angle, DEC );
        Serial.write( ' ' );
        Serial.print( sat, DEC );
        Serial.write( ' ' );
        Serial.println( value, DEC );
        break;

      case 3:
        sat = sat + 0.01*d;
        if( sat < 0.0 ) sat = 0.0;
        if( sat > 1.0 ) sat = 1.0;
        hsv_to_rgb( angle, sat, value );
        setLamp( 1, 0, R, G, B, 0 );
        setLamp( 65, 0, R, G, B, 0 );
        sendLamps();
        Serial.print( angle, DEC );
        Serial.write( ' ' );
        Serial.print( sat, DEC );
        Serial.write( ' ' );
        Serial.println( value, DEC );
        break;

      case 4:
        value = value + 0.01*d;
        if( value < 0.0 ) value = 0.0;
        if( value > 1.0 ) value = 1.0;
        hsv_to_rgb( angle, sat, value );
        setLamp( 1, 0, R, G, B, 0 );
        setLamp( 65, 0, R, G, B, 0 );
        sendLamps();
        Serial.print( angle, DEC );
        Serial.write( ' ' );
        Serial.print( sat, DEC );
        Serial.write( ' ' );
        Serial.println( value, DEC );
        break;

      }

    }

    if( state < 2 && millis() - last > 5 ) {
      last = millis();
      fader.update();

      nR = fader.R;
      nG = fader.G;
      nB = fader.B;
      if( nR != pR || nG != pG || nB != pB ) {
        setLamp( 1, 0, nR, nG, nB, 0 );
        setLamp( 65, 0, nR, nG, nB, 0 );
        sendLamps();
        pR = nR;
        pG = nG;
        pB = nB;
      }
    }

  }

}


