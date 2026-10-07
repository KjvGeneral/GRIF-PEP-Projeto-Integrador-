const int pin[6] = {A1, A2, A3, A4, A5, A6};
int leitura[6];
//const int MAX_VALUE = 1023;
//const int MIN_VALUE = 0;
int BRA_LEITURA[6];
int PRE_LEITURA[6];
int ATUAL_LEITURA[6];

void setup() {
  Serial.begin(9600);

  for(int i = 0; i < 6; i++)
  {
    pinMode(pin[i], INPUT);
    Serial.print("Pino ");
    Serial.print(pin[i]);
    Serial.println("foi configurado");
  }
  delay(500);

  // Calibração do branco 
  Serial.println("Calibracao do sensor branco");
  BOT();

  for(int i = 0; i < 6; i++){
    BRA_LEITURA[i] = analogRead(pin[i]);
    Serial.print("Leitura de BRANCO E: ");
    Serial.print(BRA_LEITURA[i]);
    Serial.print(" | ");
  }
  Serial.println("");

  delay(500);
  
  // Calibração do preto
  Serial.println("Calibracao sensor do preto");
  BOT();

  for(int i = 0; i < 6; i++){
    PRE_LEITURA[i] = analogRead(pin[i]);
    Serial.print("Leitura de PRETO E: ");
    Serial.print(PRE_LEITURA[i]);
    Serial.print(" | ");
  }
  Serial.println("");

  delay(500);
}


void loop() 
{
  /*Serial.print("Leitura: ");
  for(int i = 0; i < 6; i++){
    leitura[i] = analogRead(pin[i]);
    Serial.print("");
    Serial.print(leitura[i]);
    Serial.print(" | ");
  }
  Serial.println("");
  */

  // LENDO LINHA = 1
  for(int i = 0; i < 6; i++){
    ATUAL_LEITURA[i] = analogRead(pin[i]);

    if(BRA_LEITURA[i] <= ATUAL_LEITURA[i]){
      ATUAL_LEITURA[i] = 0;
    }
    else if(PRE_LEITURA[i] >= ATUAL_LEITURA[i]){
      ATUAL_LEITURA[i] = 1;
    }
    
    Serial.print("L: ");
    Serial.print(ATUAL_LEITURA[i]);
    Serial.print(" | ");
  }
  Serial.println();


}
void BOT(){
  
  while (Serial.available() == 0) {
    Serial.read();
  }
  delay(200);
  while (Serial.available() >= 1) {
    Serial.read();
  }
  delay(200);
}
