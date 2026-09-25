
//      Canal A
#define A_ho   4  // -> Sentido Horário
#define A_ah   9  // -> Sentido Ant-horário
#define A_pwm  6  // -> Velocidade - pwm do motor
#define A_En   A1 // -> Pino de enable do canal A

//      Canal B
#define B_ho   7  // -> Sentido Horário
#define B_ah   8  // -> Sentido Ant-horário
#define B_pwm  5  // -> Velocidade - pwm do motor
#define B_En   A0 // -> Pino de enable do canal B

int pin[8] = {A_ho, A_ah, A_pwm, A_En, B_ho, B_ah, B_pwm, B_En};


void setup() {
  Serial.begin(9600);
  for(int i = 0; i < 7; i++){
    pinMode(pin[i], OUTPUT);
  }  
  digitalWrite(A_En, HIGH);
  digitalWrite(B_En, HIGH);
}

void loop() {

    Motor(HIGH, LOW, 255, HIGH, LOW, 255);
    delay(5000);
    
    Motor(LOW, HIGH, 155, HIGH, LOW, 0);
    delay(1000);

    Motor(HIGH, LOW, 255, HIGH, LOW, 255);
    delay(5000);


}

//    Lado Esquerdo    velocidade | Lado Direito       Velocidade
void Motor(bool in1, bool in2, int pwm_A, bool in3, bool in4, int pwm_B){

  //           Canal A
  digitalWrite(A_ho, in1);
  digitalWrite(A_ah, in2);
  digitalWrite(A_pwm, pwm_A);

  //           Canal B
  digitalWrite(B_ho, in3);
  digitalWrite(B_ah, in4);
  digitalWrite(B_pwm, pwm_B);
}
