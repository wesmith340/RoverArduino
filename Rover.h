/* 
    Rover Class
      - Main control for robot, highest abstraction
*/

#include "MotorController.h"
#include "LinearActuator.h"
#include "DepositModule.h"
#include "DiggingModule.h"

#define DIG_SPEED 1
#define DIG_TIME 10*1000 // After fully extended dig for X seconds
class Rover {

private:

  MotorController *frontLeft, *frontRight;
  MotorController *backLeft, *backRight;

  DiggingModule *digMod;
  DepositModule *depMod;

public:

  Rover() {
    frontRight = new MotorController(2,22,24); // (pwm, a, b)
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
  }

  // Full dig cycle, meant for high abstraction after all code is verified
  void digCycle() {
      depMod->bottom();
      digMod->startMotor(DIG_SPEED);
      digMod->extend();

      int startTime = millis();
      int curTime = millis();
      while (curTime - startTime < DIG_TIME){
          curTime = millis();
      }
      digMod->startMotor(1); // lose the dirt
      digMod->retract();
      digMod->stopMotor();
      depMod->middle();
  }

  void depositCycle() {
      digMod->startMotor(-1*DIG_SPEED); // Reverse does not work, will need to be replaced with hall sensor
      
      // Shake it out
      depMod->top();
      depMod->bottom();
      depMod->top();
      depMod->bottom();
  }

};