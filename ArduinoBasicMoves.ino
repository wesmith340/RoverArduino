#include <Sabertooth.h>
Sabertooth ST(128);

#include "Rover.h"

void setup() {
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 16000000 / 256 / 5;            // compare match register 16MHz/256/2Hz IMPoRTANT ONE
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt 
  interrupts();

  // Serial.begin(9600);
  SabertoothTXPinSerial.begin(9600); // 9600 is the default baud rate for Sabertooth packet serial.
  ST.autobaud(); // Send the autobaud command to the Sabertooth controller(s).
  
  Rover *rover = new Rover();

  rover->forward();
  delay(500);
  rover->backward();
  delay(500);
  rover->halt();
  delay(10000);

  rover->digCycle(); // ideally would work good
  delay(5000);
  rover->depositCycle();// need hall sensor


  // All else fails go down an abstraction level
  DepositModule *depMod = new DepositModule();
  DiggingModule *digMod = new DiggingModule();
  LinearActuator *actRight = new LinearActuator(5,29,27, round(ACTUATOR_SPEED * .85));
  LinearActuator *actLeft = new LinearActuator(4,28,26, ACTUATOR_SPEED);
  actRight->retract();
  actLeft->retract();
  delay(1000);
  actRight->halt();
  actLeft->halt();
}

void loop() {
}
