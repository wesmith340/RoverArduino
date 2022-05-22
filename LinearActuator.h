class LinearActuator
{
private:
  int p1, a1, b1, vel;

public:
  LinearActuator(int p1, int a1, int b1, int vel)
  {
    this->p1 = p1;
    this->a1 = a1;
    this->b1 = b1;
    this->vel = vel;
    pinMode(p1, OUTPUT);
    pinMode(a1, OUTPUT);
    pinMode(b1, OUTPUT);
  }
  void extend()
  {
    digitalWrite(this->a1, HIGH);
    digitalWrite(this->b1, LOW);
    analogWrite(this->p1, this->vel);
  }
  void retract()
  {
    digitalWrite(this->a1, LOW);
    digitalWrite(this->b1, HIGH);
    analogWrite(this->p1, this->vel);
  }
  void halt()
  {
    digitalWrite(this->a1, LOW);
    digitalWrite(this->b1, LOW);
  }
};