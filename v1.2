/*
  Calibração de BRANCO, PRETO e VERDE - Sensor TCS3200
  -----------------------------------------------------
  Versão para Arduino Mega

  Ligações:
  S0 -> 35
  S1 -> 33
  S2 -> 37
  S3 -> 39
  OUT -> 41
  VCC -> 5V
  GND -> GND

  Fluxo:
  1. Solicita apontar para o branco
  2. Solicita apontar para o preto
  3. Faz leitura contínua normalizada (0–255)
*/
// --------------------- Bibliotecas -----------------------

#include <AFMotor.h>  //biblioteca da Sheild

// ------------------ Definição dos pinos ------------------

// Botão
const int botao = 22;

// LED RGB 1 - Direita
const int ledD_R = 30;
const int ledD_G = 32;
const int ledD_B = 34;

// LED RGB 2 - Esquerda
const int ledE_R = 40;
const int ledE_G = 42;
const int ledE_B = 44;


// SENSOR 1 - Direita
const int S0_D = 47;   // Seleção de escala de frequência - bit 0
const int S1_D = 49;   // Seleção de escala de frequência - bit 1
const int S2_D = 51;   // Seleção do filtro de cor - bit 0
const int S3_D = 53;   // Seleção do filtro de cor - bit 1
const int OUT_D = 45;  // Saída de pulsos do sensor (frequência proporcional à cor detectada)

//  SENSOR 2 - Esquerda
const int S0_E = 35;
const int S1_E = 33;
const int S2_E = 37;
const int S3_E = 39;
const int OUT_E = 41;

// Motores
// Borns da Shield MX
AF_DCMotor motorfd(1);  // Frente Direita
AF_DCMotor motorfe(2);  // Frente Esquerda
AF_DCMotor motorte(4);  // Trás Esquerda
AF_DCMotor motortd(3);  // Trás Direita


// ------------------ Variáveis de calibração ------------------
// Frequências medidas durante calibração das superfícies de referência
// Sensor do loado Direita
unsigned long R_brancoD, G_brancoD, B_brancoD;  // valores sobre branco
unsigned long R_pretoD, G_pretoD, B_pretoD;     // valores sobre preto
unsigned long R_verdeD, G_verdeD, B_verdeD;     // valores sobre verde

// Leituras atuais do sensor (em Hz aproximado)
unsigned long R_atualD, G_atualD, B_atualD;


// Frequências medidas durante calibração das superfícies de referência
// Sensor do lado Equerda
unsigned long R_brancoE, G_brancoE, B_brancoE;  // valores sobre branco
unsigned long R_pretoE, G_pretoE, B_pretoE;     // valores sobre preto
unsigned long R_verdeE, G_verdeE, B_verdeE;     // valores sobre verde

// Leituras atuais do sensor (em Hz aproximado)
unsigned long R_atualE, G_atualE, B_atualE;

// ------------ Definição das variaveis de posição ------------
int SensorEsquerda;  // Sensor Esquerda --> estado da cor, 0 = BRANCO, 1 = PRETO, 2 = VERDE
int SensorDireita;   // Sensor Direita  --> estado da cor, 0 = BRANCO, 1 = PRETO, 2 = VERDE


// =============================================================
//                      CONFIGURAÇÃO INICIAL
// =============================================================
void setup() {
  Serial.begin(9600);
  Serial.println("=== Calibracao do Sensor TCS3200 ===");
  Serial.println("Pinos configurados para Arduino Mega");
  Serial.println("------------------------------------");

  // Configura LEDs
  // LED - Direita
  pinMode(ledD_R, OUTPUT);
  pinMode(ledD_G, OUTPUT);
  pinMode(ledD_B, OUTPUT);

  // LED - Esquerda
  pinMode(ledE_R, OUTPUT);
  pinMode(ledE_G, OUTPUT);
  pinMode(ledE_B, OUTPUT);

  // Botão com pull-up interno
  pinMode(botao, INPUT_PULLUP);


  // =====================================
  // ==== CONFIGURAÇÃO SENSOR DIREITA ====
  // =====================================

  // Configuração dos pinos do sensor - Direita
  pinMode(S0_D, OUTPUT);
  pinMode(S1_D, OUTPUT);
  pinMode(S2_D, OUTPUT);
  pinMode(S3_D, OUTPUT);
  pinMode(OUT_D, INPUT);

  // Define escala de frequência de saída do TCS3200 (S0=HIGH, S1=LOW → 20%)
  // Isso reduz a frequência de saída, tornando a leitura mais estável.
  // --- Lado Direito ---
  digitalWrite(S0_D, HIGH);
  digitalWrite(S1_D, LOW);
  delay(500);

  // Começa com os LEDs apagados
  setLED(ledD_R, ledD_G, ledD_B, LOW, LOW, LOW);
  setLED(ledE_R, ledE_G, ledE_B, LOW, LOW, LOW);

  // ---- Calibração BRANCO Direita ----
  Serial.println("===== SENSOR DIREITA =====");
  Serial.println("1) Aponte o sensor para o BRANCO e pressione o botao...");
  setLED(ledD_R, ledD_G, ledD_B, HIGH, HIGH, HIGH);
  esperarBotao();                                // espera o usuário confirmar
  medirCoresD(R_brancoD, G_brancoD, B_brancoD);  // mede as 3 cores
  mostrarCalibracao("Branco", R_brancoD, G_brancoD, B_brancoD);

  // ---- Calibração PRETO Direita ----
  Serial.println("\n2) Aponte o sensor para o PRETO e pressione o botao...");
  setLED(ledD_R, ledD_G, ledD_B, LOW, LOW, LOW);
  esperarBotao();
  medirCoresD(R_pretoD, G_pretoD, B_pretoD);
  mostrarCalibracao("Preto", R_pretoD, G_pretoD, B_pretoD);

  // ---- Calibração VERDE Direita ----
  Serial.println("\n3) Aponte o sensor para o VERDE e pressione o botao...");
  setLED(ledD_R, ledD_G, ledD_B, LOW, HIGH, LOW);
  esperarBotao();
  medirCoresD(R_verdeD, G_verdeD, B_verdeD);
  mostrarCalibracao("Verde", R_verdeD, G_verdeD, B_verdeD);

  // Apaga LED - Direita
  setLED(ledD_R, ledD_G, ledD_B, LOW, LOW, LOW);


  // ======================================
  // ==== CONFIGURAÇÃO SENSOR ESQUERDA ====
  // ======================================

  // Configuração dos pinos do sensor - Esquerda
  pinMode(S0_E, OUTPUT);
  pinMode(S1_E, OUTPUT);
  pinMode(S2_E, OUTPUT);
  pinMode(S3_E, OUTPUT);
  pinMode(OUT_E, INPUT);

  // Define escala de frequência de saída do TCS3200 (S0=HIGH, S1=LOW → 20%)
  // --- Lado Equerda ---
  digitalWrite(S0_E, HIGH);
  digitalWrite(S1_E, LOW);
  delay(500);

  // Começa com os LEDs apagados
  setLED(ledD_R, ledD_G, ledD_B, LOW, LOW, LOW);
  setLED(ledE_R, ledE_G, ledE_B, LOW, LOW, LOW);

  // ---- Calibração BRANCO Direita ----
  Serial.println("===== SENSOR DIREITA =====");
  Serial.println("1) Aponte o sensor para o BRANCO e pressione o botao...");
  setLED(ledE_R, ledE_G, ledE_B, HIGH, HIGH, HIGH);
  esperarBotao();                                // espera o usuário confirmar
  medirCoresE(R_brancoE, G_brancoE, B_brancoE);  // mede as 3 cores
  mostrarCalibracao("Branco", R_brancoE, G_brancoE, B_brancoE);

  // ---- Calibração PRETO Direita ----
  Serial.println("\n2) Aponte o sensor para o PRETO e pressione o botao...");
  setLED(ledE_R, ledE_G, ledE_B, LOW, LOW, LOW);
  esperarBotao();
  medirCoresE(R_pretoE, G_pretoE, B_pretoE);
  mostrarCalibracao("Preto", R_pretoE, G_pretoE, B_pretoE);

  // ---- Calibração VERDE Direita ----
  Serial.println("\n3) Aponte o sensor para o VERDE e pressione o botao...");
  setLED(ledE_R, ledE_G, ledE_B, LOW, HIGH, LOW);
  esperarBotao();
  medirCoresE(R_verdeE, G_verdeE, B_verdeE);
  mostrarCalibracao("Verde", R_verdeE, G_verdeE, B_verdeE);

  // Apaga LED - Esquerda
  setLED(ledE_R, ledE_G, ledE_B, LOW, LOW, LOW);

  /*// ---- Calibração VERMELHO ----
  Serial.println("\n4) Aponte o sensor para o VERMELHO e pressione qualquer tecla...");
  aguardarEntrada();
  medirCores(R_vermelho, G_vermelho, B_vermelho);
  mostrarCalibracao("Vermelho", R_vermelho, G_vermelho, B_vermelho);

  Serial.println("\nCalibracao concluida. Iniciando leitura continua...\n");*/
  delay(500);

  // Constante de velocidade dos motores
  motorfd.setSpeed(150);
  motorfe.setSpeed(150);
  motortd.setSpeed(150);
  motorte.setSpeed(150);
  moverMotores(RELEASE, RELEASE, RELEASE, RELEASE); // Start sem mover os motores
}


// =============================================================
//                      LOOP PRINCIPAL
// =============================================================
void loop() {

  // Mede os valores atuais de R, G e B
  // --- Sensor Direita ---
  medirCoresD(R_atualD, G_atualD, B_atualD);
  medirCoresE(R_atualE, G_atualE, B_atualE);

  // Normaliza os valores com base nos limites preto e branco
  // A função map() converte o intervalo [preto, branco] para [0, 255]
  // constrain() garante que o resultado fique dentro dos limites válidos
  int R_norm_D = constrain(map(R_atualD, R_pretoD, R_brancoD, 0, 255), 0, 255);  // ==> Esquerda
  int G_norm_D = constrain(map(G_atualD, G_pretoD, G_brancoD, 0, 255), 0, 255);
  int B_norm_D = constrain(map(B_atualD, B_pretoD, B_brancoD, 0, 255), 0, 255);

  int R_norm_E = constrain(map(R_atualE, R_pretoE, R_brancoE, 0, 255), 0, 255);  // ==> Direita
  int G_norm_E = constrain(map(G_atualE, G_pretoE, G_brancoE, 0, 255), 0, 255);
  int B_norm_E = constrain(map(B_atualE, B_pretoE, B_brancoE, 0, 255), 0, 255);

  //=============================================================

  // Exibe as leituras normalizadas (0–255)
  // Direita
  Serial.print("R: ");
  Serial.print(R_norm_D);
  Serial.print("  G: ");
  Serial.print(G_norm_D);
  Serial.print("  B: ");
  Serial.print(B_norm_D);
  Serial.print("   -> ");
  // Esquerda
  Serial.print("R: ");
  Serial.print(R_norm_E);
  Serial.print("  G: ");
  Serial.print(G_norm_E);
  Serial.print("  B: ");
  Serial.print(B_norm_E);
  Serial.print("   -> ");

  // =====================================================
  // ================== Sensor Esquerdo ==================
  // =====================================================

  // Identificação simples de cor (classificação básica)
  if (R_norm_E > 150 && G_norm_E > 150 && B_norm_E > 150) {
    Serial.println("Provavelmente BRANCO -- Esquerda");
    setLED(ledE_R, ledE_G, ledE_B, HIGH, HIGH, HIGH);
    SensorEsquerda = 0;
  }
  /*else if (R_norm > 2*G_norm && R_norm > 2*B_norm)
    Serial.println("Provavelmente VERMELHO");*/
  else if (G_norm_E > R_norm_E && G_norm_E > B_norm_E) {
    Serial.println("Provavelmente VERDE -- Esquerda");
    setLED(ledE_R, ledE_G, ledE_B, LOW, HIGH, LOW);
    SensorEsquerda = 2;
  }
  /*else if (B_norm > R_norm && B_norm > G_norm)
    Serial.println("Prvavelmente AZUL");*/
  else if (R_norm_E < 30 && G_norm_E < 30 && B_norm_E < 30) {
    Serial.println("Provavelmente PRETO -- Esquerda");
    setLED(ledE_R, ledE_G, ledE_B, LOW, LOW, LOW);
    //led esq preto
    SensorEsquerda = 1;
  } else {
    Serial.println("Cor intermediaria ou mista -- Esquerda");
    setLED(ledE_R, ledE_G, ledE_B, LOW, LOW, HIGH);  // aciona azul
    SensorEsquerda = 0;
  }
  //delay(50);  // intervalo entre leituras

  // =====================================================
  // ================== Sensor Direito ===================
  // =====================================================

  // Identificação simples de cor (classificação básica)
  if (R_norm_D > 150 && G_norm_D > 150 && B_norm_D > 150) {
    Serial.println("Provavelmente BRANCO -- Direita");
    setLED(ledD_R, ledD_G, ledD_B, HIGH, HIGH, HIGH);
    SensorEsquerda = 0;
  }
  /*else if (R_norm > 2*G_norm && R_norm > 2*B_norm)
    Serial.println("Provavelmente VERMELHO");*/
  else if (G_norm_D > R_norm_D && G_norm_D > B_norm_D) {
    Serial.println("Provavelmente VERDE -- Direita");
    setLED(ledD_R, ledD_G, ledD_B, LOW, HIGH, LOW);
    SensorEsquerda = 2;
  }
  /*else if (B_norm > R_norm && B_norm > G_norm)
    Serial.println("Prvavelmente AZUL");*/
  else if (R_norm_D < 30 && G_norm_D < 30 && B_norm_D < 30) {
    Serial.println("Provavelmente PRETO -- Direita");
    setLED(ledD_R, ledD_G, ledD_B, LOW, LOW, LOW);
    //led esq preto
    SensorEsquerda = 1;
  } else {
    Serial.println("Cor intermediaria ou mista -- Direita");
    setLED(ledD_R, ledD_G, ledD_B, LOW, LOW, HIGH);  // aciona azul
    SensorEsquerda = 0;
  }


  // ---> logica de acionamento de motores <---
  if (SensorEsquerda == 2 && SensorDireita == 2) {
    // Retorno();
    // Serial.println("Sem Saida!!!");
  } else if (SensorEsquerda == 2 && SensorDireita == 0) {
    // detectou verde na direita, rotina a criar para verde DIR.
    // RotinaVerdeD();
    // Serial.println("Virando Para a Direita");
  } else if (SensorEsquerda == 0 && SensorDireita == 2) {
    // detectou verde na esquerda, rotina a criar para verde ESQ.
    // RotinaVerdeE();
    // Serial.println("Virando Para a Esquerda");
  } else if (SensorEsquerda == 1 && SensorDireita == 0) {
    // detectou preto na esquerda, ir para a direita
    moverMotores(FORWARD, RELEASE, FORWARD, RELEASE);

  } else if (SensorEsquerda == 0 && SensorDireita == 1) {
    // detectou preto na direita, ir para a esquerda
    moverMotores(RELEASE, FORWARD, RELEASE, FORWARD);
  } else {
    //(SE == 0 && SD == 0)
    // detectou branco nos dois, rotina de gap essa rotina não pode ser com delay
    moverMotores(FORWARD, FORWARD, FORWARD, FORWARD);
  }
}



// =============================================================
//                      FUNÇÕES AUXILIARES
// =============================================================

// Aciona LED RGB (nível ativo alto)
void setLED(int R, int G, int B, bool rVal, bool gVal, bool bVal) {
  digitalWrite(R, !rVal);
  digitalWrite(G, !gVal);
  digitalWrite(B, !bVal);
}


// --- Mede R, G e B individualmente usando os filtros do TCS3200 ---
// --- Lado Direito ---
void medirCoresD(unsigned long &R, unsigned long &G, unsigned long &B) {
  // Seleciona filtro vermelho (S2=LOW, S3=LOW)
  digitalWrite(S2_D, LOW);
  digitalWrite(S3_D, LOW);
  delay(50);
  R = medirFreq_D();  // mede frequência correspondente à cor vermelha

  // Seleciona filtro verde (S2=HIGH, S3=HIGH)
  digitalWrite(S2_D, HIGH);
  digitalWrite(S3_D, HIGH);
  delay(50);
  G = medirFreq_D();

  // Seleciona filtro azul (S2=LOW, S3=HIGH)
  digitalWrite(S2_D, LOW);
  digitalWrite(S3_D, HIGH);
  delay(50);
  B = medirFreq_D();
}
// --- Lado Esquerdo ---
void medirCoresE(unsigned long &R, unsigned long &G, unsigned long &B) {
  // Seleciona filtro vermelho (S2=LOW, S3=LOW)
  digitalWrite(S2_E, LOW);
  digitalWrite(S3_E, LOW);
  delay(50);
  R = medirFreq_E();  // mede frequência correspondente à cor vermelha

  // Seleciona filtro verde (S2=HIGH, S3=HIGH)
  digitalWrite(S2_E, HIGH);
  digitalWrite(S3_E, HIGH);
  delay(50);
  G = medirFreq_E();

  // Seleciona filtro azul (S2=LOW, S3=HIGH)
  digitalWrite(S2_E, LOW);
  digitalWrite(S3_E, HIGH);
  delay(50);
  B = medirFreq_E();
}


// --- Mede a frequência do pino OUT (proporcional à intensidade da cor) ---
// --- Sensor Direita ---
unsigned long medirFreq_D() {
  // pulseIn mede o tempo em microssegundos de um pulso LOW
  // 100000 µs = 100 ms → tempo máximo de espera
  unsigned long duracao = pulseIn(OUT_D, LOW, 100000);
  if (duracao == 0) return 0;  // caso de falha de leitura
  return 1000000UL / duracao;  // converte o período em frequência (Hz)
}
//--- Sensor Esquerda ---
unsigned long medirFreq_E() {
  // pulseIn mede o tempo em microssegundos de um pulso LOW
  // 100000 µs = 100 ms → tempo máximo de espera
  unsigned long duracao = pulseIn(OUT_E, LOW, 100000);
  if (duracao == 0) return 0;  // caso de falha de leitura
  return 1000000UL / duracao;  // converte o período em frequência (Hz)
}


// --- Exibe os valores medidos no monitor serial ---
void mostrarCalibracao(const char *nome, unsigned long R, unsigned long G, unsigned long B) {
  Serial.print(nome);
  Serial.print(" -> R: ");
  Serial.print(R);
  Serial.print("  G: ");
  Serial.print(G);
  Serial.print("  B: ");
  Serial.println(B);
}


// Aguarda o botão ser pressionado e solto
void esperarBotao() {
  while (digitalRead(botao) == HIGH)
    ;  // esperando apertar (com INPUT_PULLUP)
  delay(200);
  while (digitalRead(botao) == LOW)
    ;  // esperando soltar
  delay(200);
}

// Função para mover os motores
//                FD       FE        TD       TE
void moverMotores(int fd, int fe, int td, int te) {
  motorfd.run(fd);  // motor frente-direita
  motorfe.run(fe);  // motor frente-esquerda
  motortd.run(td);  // motor trás-direita
  motorte.run(te);  // motor trás-esquerda
}

// Função de rotina verde
void RotinaVerdeD() {
  // Virar 90º para a Direita
}
void RotinaVerdeE() {
  // Virar 90º para a Esquerda
}
void Retorno() {
  // Virar 180º
}
