#define SPEED 255

class MotorController
{
private:
  int p1, a1, b1;

public:
  MotorController(int p1, int a1, int b1)
  {
    this->p1 = p1;
    this->a1 = a1;
    this->b1 = b1;
    pinMode(p1, OUTPUT);
    pinMode(a1, OUTPUT);
    pinMode(b1, OUTPUT);
  }

  void forward()
  {
    digitalWrite(a1, HIGH);
    digitalWrite(b1, LOW);
    analogWrite(p1, SPEED);
  }

  void backward()
  {
    digitalWrite(a1, LOW);
    digitalWrite(b1, HIGH);
    analogWrite(p1, SPEED);
  }

  void halt()
  {
    digitalWrite(a1, LOW);
    digitalWrite(b1, LOW);
    analogWrite(p1, 0);
  }
};