#define DEP_BOTTOM 0
#define TOP_OFFSET 38000 // Ticks from bottom to top

int INTERRUPT_DEP_A = digitalPinToInterrupt(18);
int INTERRUPT_DEP_B = digitalPinToInterrupt(21);

#define DEP_MOTOR 2

volatile double depPos = 0;
volatile int DEP_A_SIG = 0, DEP_B_SIG = 1;

void DEP_A_RISE();
void DEP_A_FALL();
void DEP_B_RISE();
void DEP_B_FALL();

void DEP_A_RISE(){
  detachInterrupt(INTERRUPT_DEP_A);
  DEP_A_SIG = 1;
  if (DEP_B_SIG == 0)
    depPos++; // moving forward
  if (DEP_B_SIG == 1)
    depPos--; // moving reverse
  attachInterrupt(INTERRUPT_DEP_A, DEP_A_FALL, FALLING);
}
void DEP_A_FALL(){
  detachInterrupt(INTERRUPT_DEP_A);
  DEP_A_SIG = 0;
  if (DEP_B_SIG == 1)
    depPos++; // moving forward
  if (DEP_B_SIG == 0)
    depPos--; // moving reverse
  attachInterrupt(INTERRUPT_DEP_A, DEP_A_RISE, RISING);
}
void DEP_B_RISE(){
  detachInterrupt(INTERRUPT_DEP_B);
  DEP_B_SIG = 1;
  if (DEP_A_SIG == 1)
    depPos++; // moving forward
  if (DEP_A_SIG == 0)
    depPos--; // moving reverse
  attachInterrupt(INTERRUPT_DEP_B, DEP_B_FALL, FALLING);
}
void DEP_B_FALL(){
  detachInterrupt(INTERRUPT_DEP_B);
  DEP_B_SIG = 0;
  if (DEP_A_SIG == 0)
    depPos++; // moving forward
  if (DEP_A_SIG == 1)
    depPos--; // moving reverse
  attachInterrupt(INTERRUPT_DEP_B, DEP_B_RISE, RISING);
}

class DepositModule
{
private:
  int error, lasterror;
  int dE, iE;
  int kp = 1, ki = 0, kd = 0;

public:

  void top() // moves bucket to top
  {
    while (TOP_OFFSET - depPos > 0)
    {
      ST.motor(DEP_MOTOR, -40);
    }
    ST.motor(DEP_MOTOR, 0);
  }
  void middle() // Align bucket with bottom then move bucket to halfway
  {
    this->bottom();
    while (TOP_OFFSET/2 - depPos > 0)
    {
      ST.motor(DEP_MOTOR, -40);
    }
    ST.motor(DEP_MOTOR, 0);
  }
  void bottom() // moves bucket to bottom
  {
    while (depPos > 0)
    {
      ST.motor(DEP_MOTOR, 40);
    }
    ST.motor(DEP_MOTOR, 0);
  }
};