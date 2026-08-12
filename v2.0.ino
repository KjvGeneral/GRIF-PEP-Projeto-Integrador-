// Sistema totalmente integrado - Arduino Mega 2560 - Ponte H monster +  TCS3200
// Autor: Christian Antonio D. R. M.
// Instituto Federal de São Paulo - Campus Presidente Epitácio
// GRIF - PEP -- Grupo de Robotica do Instituto Federal - Campus Presidente Epitácio

/*
=============================================================
                   DEF. PINOS e VARIAVEIS
=============================================================
*/

// ---- CONFIGURAÇÃO DA SHIELD PONTE H MONSTER ----

//      Canal A
const int A_ho  = 4;  // -> Sentido Horário
      int A_ah  = 9;  // -> Sentido Ant-horário
      int A_pwm = 6;  // -> Velocidade - pwm do motor
      int A_En  = A1; // -> Pino de enable do canal A

//      Canal B
const int B_ho  = 7;  // -> Sentido Horário
      int B_ah  = 8;  // -> Sentido Ant-horário
      int B_pwm = 5;  // -> Velocidade - pwm do motor
      int B_En  = A0; // -> Pino de enable do canal B


// ---- CONFIGURAÇÃO DOS 2 - SENSORES TCS3200 ----

//  -    Sensor lado Direito -> D = Direita    -

const int D_S0  = 13;   // Seleção de escala de frequência - bit 0.
const int D_S1  = 14;   // Seleção de escala de frequência - bit 1.
const int D_S2  = 15;   // Seleção de filtro de cor.
const int D_S3  = 16;   // Seleção de filtro de cor. 
const int D_Out = 17;   // Sainda de pulsos (proporcional a frequência detectada).

//        Variaveis de calibração

unsigned long DR_branco, DG_branco, DB_branco;
unsigned long DR_preto,  DG_preto,  DB_preto; 
unsigned long DR_verde,  DG_verde,  DB_verde;
//unsigned long DR_verm,   DG_verm,   DB_verm;


//  -    Sensor lado Esquerdo -> E = Esquerda    -

const int E_S0  = 18;   // Seleção de escala de frequencia - bit 0.
const int E_S1  = 19;   // Seleção de escala de frequencia - bit 1.
const int E_S2  = 20;   // Seleção do filtro de cor.
const int E_S3  = 21;   // Seleção de filtro de cor.
const int E_Out = 22;   // Saida de pulsos (proporcional a frequênia detectada).

//       Variaveis de Calibração

unsigned long ER_branco, EG_branco, EB_branco;
unsigned long ER_preto,  EG_preto,  EB_preto;
unsigned long ER_verde,  EG_verde,  EB_verde;
//unsigned long ER_verm,   EG_verm,   EB_verm;

//       Variaveis de leitura atual

unsigned long DR_atual, DG_atual, DB_atual; // leitura atual do lado direito
unsigned long ER_atual, EG_atual, EB_atual; // leitura atual do lado esquerdo

//       Vetor para confurar pinos
//       --------->     (      Pinos Shield     ) (           Pinos do TCS3200           )
const int ConfigPins[18] = {7, 8, 5, A0, 4, 9, 6, A1, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22};

void setup() {
  Serial.begin(9600);

  //     Configuração dos pinos
  for(int i = 0; i < 18; i++){

    digitalWrite(ConfigPins[i], OUTPUT);
    Serial.print(ConfigPins[i]);
    Serial.println(" - Foi configurado como OUTPUT com sucesso!!");

    if(ConfigPins[i] == 17 || ConfigPins[i] == 22){

      digitalWrite(ConfigPins[i], INPUT);
      Serial.print(ConfigPins[i]);
      Serial.println(" - Foi configurado como INPUT com sucesso!!");
    }
  }

  //     Calibração dos Sensore de cor - TCS3200
  //     = Sensor Esquerdo =

  Serial.println("\n1-) Aponte para o BRANCO com o sensor ESQUERDO!...\n");
  esperarBotao();
  medirCores(E_S2, E_S3, E_Out, ER_branco, EB_branco, EG_branco);
  mostrarCalibracao("Branco Esquerdo", ER_branco, EB_branco, EG_branco);
  Serial.println("Branco Calibrado");
  delay(100);

  Serial.println("\n2-) Aponte para o PRETO com o sensor ESQUERDO!...\n");
  esperarBotao();
  medirCores(E_S2, E_S3, E_Out, ER_preto, EG_preto, EB_preto);
  mostrarCalibracao("Preto Esquerdo", ER_preto, EG_preto, EB_preto);
  Serial.println("Preto Calibrado");
  delay(100);

  Serial.println("\n3-) Aponte para o VERDE com o sensor ESQUERDo!...\n");
  esperarBotao();
  medirCores(E_S2, E_S3, E_Out, ER_verde, EG_verde, EB_verde);
  mostrarCalibracao("Verde Esquerdo", ER_verde, EG_verde, EB_verde);
  Serial.println("Verde calibrado");

  //    = Sensor Direito =
  Serial.println("\n4-) Aponte para o BRANCO com o sensor DIREITO!...\n");
  esperarBotao();
  medirCores(D_S2, D_S3, D_Out, DR_branco, DG_branco, DB_branco);
  mostrarCalibracao("Branco Direito", DR_branco, DG_branco, DB_branco);
  Serial.println("Branco Calibrado");

  Serial.println("\n5-) Aponte para o PRETO com o sensor DIREITO!...\n");
  esperarBotao();
  medirCores(D_S2, D_S3, D_Out, DR_preto, DG_preto, DB_preto);
  mostrarCalibracao("Preto Calibrado", DR_preto, DG_preto, DB_preto);

  Serial.println("Aponte para o VERDE com o sensor DIREITO!...\n");
  esperarBotao();
  medirCores(D_S2, D_S3, D_Out, DR_verde, DG_verde, DB_verde);
  mostrarCalibracao("Verde Calibrado", DR_verde, DG_verde, DB_verde);

  delay(50);
  Serial.println("\n Os sensores foram calibrados corretamente! \n\n");

}

void loop() {
  
  // Leitura dos calores atuais de leitura

  medirCores(D_S2, D_S3, D_Out, DR_atual, DG_atual, DB_atual); // Sensor Direito
  medirCores(E_S2, E_S3, E_Out, ER_atual, EG_atual, EB_atual); // Sensor Esquerdo

  // Normalização dos valores atuais de leitura
  // - Sensor Direito -
  int DR_norm = constrain(map(DR_atual, DR_preto, DR_branco, 0, 1023), 0, 1023);
  int DG_norm = constrain(map(DG_atual, DG_preto, DG_branco, 0, 1023), 0, 1023);
  int DB_norm = constrain(map(DB_atual, DB_preto, DB_branco, 0, 1023), 0, 1023);

  // - Sensosr Esquerdo -
  int ER_norm = constrain(map(ER_atual, ER_preto, ER_branco, 0, 1023), 0, 1023);
  int EG_norm = constrain(map(EG_atual, EG_preto, EG_branco, 0, 1023), 0, 1023);
  int EB_norm = constrain(map(EB_atual, EB_preto, EB_branco, 0, 1023), 0, 1023);



  // - Sensor Direito -

  if((DR_atual == DR_branco || DG_atual == DG_branco || DB_atual == DB_branco) || (DR_norm > 700 && DG_norm > 700 && DB_norm > 700)){
    Serial.println("D__BRANCO");
  }
  else if((DR_atual < DR_verde || DG_atual >= DG_verde || DB_atual < DB_atual) || (DR_norm < DG_norm && DG_norm && DB_norm < DG_norm)){
    Serial.println("D__VERDE");
  }
  else if((DR_atual == DR_preto || DG_atual == DG_preto || DB_atual == DB_preto) || (DR_norm < 150 && DG_norm < 150 && DB_norm < 150)){
    Serial.println("D_PRETO");
  }

  // - Sensor Esquerdo -

  else if((ER_atual == ER_branco || EG_atual == EG_branco || EB_atual == EG_branco) || (ER_norm >= 700 && EG_norm >= 700 && EB_norm >= 700)){
    Serial.println("E__BRANCO");
  }
  else if((ER_atual < ER_verde || EG_atual == EG_verde || EB_atual < EB_verde) || (ER_norm < ER_verde && EG_norm == EG_verde && EB_norm < EB_verde)){
    Serial.println("E__VERDE");
  }
  else if((ER_atual == ER_preto || EG_atual == EG_preto || EB_atual == EB_preto) || (ER_preto < 150 && EG_preto < 150 && EB_preto < 150)){
    Serial.println("E__PRETO");
  }
  else{
    Serial.println("Nsei");
  }


}

/*
=============================================================
                      FUNÇÕES AUXILIARES
=============================================================
*/

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


// Aguarda a entrada de input do Serial da IDE
void esperarBotao(){
  while(Serial.available() == 0){
    Serial.read(); // Faz leitura do Serial
  }
  delay(200);
  while(Serial.available() >= 1){
    Serial.read(); // Faz leitura do Serial
  }
  delay(200);
}

// Comentario sobre a auteração da função

void medirCores(int pin_s2, int pin_s3, unsigned long pin_OUT, unsigned long &R, unsigned long &G, unsigned long &B){
  
  // Seleciona filtro Vermelho
  digitalWrite(pin_s2, LOW);
  digitalWrite(pin_s3, LOW);
  delay(50);                // So para dar uma folga
  R = medirFreq(pin_OUT);

  // Seleciona filtro Verde
  digitalWrite(pin_s2, HIGH);
  digitalWrite(pin_s3, HIGH);
  delay(50);                // So para dar uma folga
  G = medirFreq(pin_OUT);

  // Seleciona filtro Azul
  digitalWrite(pin_s2, HIGH);
  digitalWrite(pin_s3, HIGH);
  delay(50);                // So para dar uma folga dnv
  B = medirFreq(pin_OUT);
}

// Faz a leitura do periodo da onda e tranforma em frequência
unsigned long medirFreq(unsigned long pin_OUT){
  // pulseIn mede o periodo da onda quadrada em microssegundos apartir do pulso LOW
  // 100000 µs = 100 ms → tempo máximo de espera
  unsigned long duracao = pulseIn(pin_OUT, LOW, 100000);
  if(duracao == 0) return 0;  // caso de falha
  return 1000000UL / duracao; // converte o período em frequência (Hz)
}

void mostrarCalibracao(const char *nome, unsigned long R, unsigned long G, unsigned long B){
  //          Faz uma amostra de como os valores direto do sensor sendo lidos
  Serial.print(nome );
  Serial.print("-> R: ");
  Serial.print(R);
  Serial.print(" G: ");
  Serial.print(G);
  Serial.print(" B: ");
  Serial.println(B);
}
