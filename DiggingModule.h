#define DIG_MOTOR 1
#define ACTUATOR_SPEED 252
#define ACTL_UPSENSE 49
#define ACTL_DOWNSENSE 46
#define ACTR_UPSENSE 45
#define ACTR_DOWNSENSE 42
#define DIG_TIME 100000




volatile int INTERRUPT_DIG_A = digitalPinToInterrupt(20);
volatile int INTERRUPT_DIG_B = digitalPinToInterrupt(19);

volatile int prevTime = millis(), prevPos = 0;
volatile boolean motorRunning = false;

volatile int DIG_A_SIG = 0, DIG_B_SIG = 1;
volatile int digPos = 0;
volatile double actVel = 0;

volatile int speedSetting = 0;
volatile int digSpeed = 0;
volatile float K_p_A = 5;

void DIG_A_RISE();
void DIG_A_FALL();
void DIG_B_RISE();
void DIG_B_FALL();


void DIG_A_RISE(){
  detachInterrupt(INTERRUPT_DIG_A);
  DIG_A_SIG = 1;
  if (DIG_B_SIG == 0)
    digPos++; // moving forward
  if (DIG_B_SIG == 1)
    digPos--; // moving reverse
  attachInterrupt(INTERRUPT_DIG_A, DIG_A_FALL, FALLING);
}
void DIG_A_FALL(){
  detachInterrupt(INTERRUPT_DIG_A);
  DIG_A_SIG = 0;
  if (DIG_B_SIG == 1)
    digPos++; // moving forward
  if (DIG_B_SIG == 0)
    digPos--; // moving reverse
  attachInterrupt(INTERRUPT_DIG_A, DIG_A_RISE, RISING);
}
void DIG_B_RISE(){
  detachInterrupt(INTERRUPT_DIG_B);
  DIG_B_SIG = 1;
  if (DIG_A_SIG == 1)
    digPos++; // moving forward
  if (DIG_A_SIG == 0)
    digPos--; // moving reverse
  attachInterrupt(INTERRUPT_DIG_B, DIG_B_FALL, FALLING);
}
void DIG_B_FALL(){
  detachInterrupt(INTERRUPT_DIG_B);
  DIG_B_SIG = 0;
  if (DIG_A_SIG == 0)
    digPos++; // moving forward
  if (DIG_A_SIG == 1)
    digPos--; // moving reverse
  attachInterrupt(INTERRUPT_DIG_B, DIG_B_RISE, RISING);
}


ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  if (motorRunning){
    int curTime = millis();
    int curPos = digPos;
    actVel = (curPos-prevPos) / (curTime-prevTime)*1000/3600;
    int error = digSpeed - actVel;                            //calcuate error term
    speedSetting = int(float(speedSetting) + K_p_A*error); //calcutate new output from controller
    speedSetting = constrain(speedSetting, -30, 30);
    ST.motor(DIG_MOTOR, speedSetting);
    prevPos = curPos;
    prevTime = curTime;
  }
}


class DiggingModule
{
private:
  LinearActuator *actLeft, *actRight;
  int a1, b1, p1;

public:
  DiggingModule()
  {
    // MotorController(p1, a1, b1) => p1 is PWN (0->255); a1 is forward (HIGH/LOW); b1 is backward (HIGH/LOW);
    actRight = new LinearActuator(5,29,27, round(ACTUATOR_SPEED * .85));
    actLeft = new LinearActuator(4,28,26, ACTUATOR_SPEED);
    // digDrive = new MotorController(9,33,34);
    int time = millis();
  }

  void extend()
  {

    actRight->extend();
    actLeft->extend();
    boolean lHalt = false, rHalt = false;
    while(lHalt == false or rHalt == false){
      if (digitalRead(ACTR_DOWNSENSE) == LOW){
        actRight->halt();
        rHalt = true;
      }
      if (digitalRead(ACTL_DOWNSENSE) == LOW){
        actLeft->halt();
        lHalt = true;
      }
    }
    actRight->halt();
    actLeft->halt();
  }

  void retract()
  {
    actRight->retract();
    actLeft->retract();
    boolean lHalt = false, rHalt = false;
    while(lHalt == false or rHalt == false){
      if (digitalRead(ACTR_UPSENSE) == LOW){
        actRight->halt();
        rHalt = true;
      }
      if (digitalRead(ACTL_UPSENSE) == LOW){
        actLeft->halt();
        lHalt = true;
      }
    }
    actRight->halt();
    actLeft->halt();
  }
  void startMotor(int speed){
    digSpeed = speed;
    prevTime = millis();
    prevPos = digPos;
    motorRunning = true;
  }
  void stopMotor(){
    motorRunning = false;
    ST.motor(DIG_MOTOR, 0);
  }
  void digCycle()
  {
    int startTime = millis();
    this->startMotor(50);
    this->extend();

    int timeDiff = millis()-startTime;
    while (timeDiff < DIG_TIME){
      delay(100);
      timeDiff = millis()-startTime;
    }
    this->startMotor(10);
    this->retract();
    this->stopMotor();
  }
};
