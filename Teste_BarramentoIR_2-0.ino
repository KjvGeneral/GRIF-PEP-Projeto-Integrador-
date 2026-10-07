const int pin[6] = {A8, A9, A10, A11, A12, A13};

int LEITURA_BIN[6];

int BRA_BASE[6];  // Armazena o valor da linha branca calibrada
int PRE_BASE[6];  // Armazena o valor da linha preta calibrada
int LEITURA_ATUAL[6];// Armazena o valor atual do pino e depois usamos para binarizar
int CORTE[6];        // Valor medio para decidir a binarização

void setup() {
  Serial.begin(9600);

  for(int i = 0; i < 6; i++)
  {
    pinMode(pin[i], INPUT);
    Serial.print("Pino ");
    Serial.print(pin[i]);
    Serial.println("foi configurado ");
  }
  Serial.println("\n");
  delay(500);

  // Calibração do branco 
  Serial.println("Calibracao do sensor branco");
  esperarBOT();

  for(int i = 0; i < 6; i++){
    BRA_BASE[i] = calibrarIR(pin[i]); // aplica a media do sensor i para eliminar o ruido
    Serial.print("Leitura de BRANCO: ");
    Serial.print(BRA_BASE[i]);
    Serial.print(" | ");
  }
  Serial.println("");

  delay(500);
  
  // Calibração do preto
  Serial.println("Calibracao sensor do preto");
  esperarBOT();

  for(int i = 0; i < 6; i++){
    PRE_BASE[i] = calibrarIR(pin[i]);
    Serial.print("Leitura de PRETO: ");
    Serial.print(PRE_BASE[i]);
    Serial.print(" | ");
  }
  Serial.println("\n");

  delay(500);

  Serial.println("CALCULANDO LINHA DE CORTE \n");
  Serial.print("CORTE: ");
  for(int i = 0; i < 6; i++){
    CORTE[i] = (BRA_BASE[i] + PRE_BASE[i]) / 2; // Calcula a media de preto e branco
    Serial.print(CORTE[i]);
    Serial.print(" | ");
  }
  Serial.println("\n");

  delay(1000);
}


void loop() 
{
  
  // LENDO LINHA = 1

  for(int i = 0; i < 6; i++){
    LEITURA_ATUAL[i] = analogRead(pin[i]);

    if(LEITURA_ATUAL[i] < CORTE[i]){
      LEITURA_BIN[i] = 0;
    }
    else if(LEITURA_ATUAL[i] > CORTE[i]){
      LEITURA_BIN[i] = 1;
    }
    
    Serial.print("L: ");
    Serial.print(LEITURA_BIN[i]);
    Serial.print(" | ");
  }
  Serial.println();


}

// função deseguinada para fazer uma media da leitura para evitar ruidos.
int calibrarIR(int pino){
  long soma = 0; // Valor inicial da variavel respectivo a calibração

  // Vamos varrer 20 vezes
  for(int i = 0; i < 20; i++){
    soma += analogRead(pino);
    delay(2); // Tempo de espera pequena para não deixar entrar ruido extra 
  }

  return (soma / 20); /* pega o acumulado dos dados do sensor e divide pelo
  numero de vezes varrida, para obter a media do retorno do sensor i do laço
  for. */
  
}

// trava o codigo ate chegar um input
void esperarBOT(){
  
  while (Serial.available() == 0) {
    Serial.read();
  }
  delay(200);
  while (Serial.available() >= 1) {
    Serial.read();
  }
  delay(200);
}
