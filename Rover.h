#include "MotorController.h"
#include "LinearActuator.h"
#include "DepositModule.h"
#include "DiggingModule.h"

#define DIG_SPEED 1
#define DIG_TIME 10*1000
class Rover {

private:

  MotorController *frontLeft, *frontRight;
  MotorController *backLeft, *backRight;

  DiggingModule *digMod;
  DepositModule *depMod;

  // boolean digging;

  //  

public:

  Rover() {

    // MotorController(p1, a1, b1) => p1 is PWN (0->255); a1 is forward (HIGH/LOW); b1 is backward (HIGH/LOW);

    frontRight = new MotorController(2,22,24);
    backRight = new MotorController(3,25,23);
    frontLeft = new MotorController(5,26,28);
    backLeft = new MotorController(5,27,29);
    
    digMod = new DiggingModule();
    depMod = new DepositModule();
  }
  void forward() {
    this->backLeft->forward();
    this->frontRight->forward();
    this->frontLeft->forward();
    this->backRight->forward();
  }
  void backward() {
    backLeft->backward();
    frontRight->backward();
    frontLeft->backward();
    backRight->backward();
  }
  void right() {
    backLeft->forward();
    frontRight->backward();
    frontLeft->forward();
    backRight->backward();
  }
  void left() {
    backLeft->backward();
    frontRight->forward();
    frontLeft->backward();
    backRight->forward();
  }
  void halt() {
    backLeft->halt();
    frontRight->halt();
    frontLeft->halt();
    backRight->halt();

    // digMod->halt();
    // depMod->halt();
  }

  void digCycle() {
      depMod->bottom();
      digMod->startMotor(DIG_SPEED);
      digMod->extend();

      int startTime = millis();
      int curTime = millis();
      while (curTime - startTime < DIG_TIME){
          curTime = millis();
      }
      digMod->startMotor(DIG_SPEED);
      digMod->retract();
      digMod->stopMotor();
      depMod->middle();
  }

  void depositCycle() {
      digMod->startMotor(-1*DIG_SPEED);
      depMod->top();
      depMod->bottom();
      depMod->top();
      depMod->bottom();
  }

};