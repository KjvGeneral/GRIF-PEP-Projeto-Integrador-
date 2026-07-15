/*
  Calibração de BRANCO, PRETO - Sensor TCS3200
  -----------------------------------------------------
  Versão para Arduino Mega 2650

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
  3. Faz leitura contínua normalizada (0–1023) 
*/

// ------------------ Definição dos pinos ------------------

// Botão
const int botao = 22;

// LED RGB 1 - Esquerda
const int led1_R = 30;
const int led1_G = 32;
const int led1_B = 34;

// LED RGB 2 - Direita
const int led2_R = 40;
const int led2_G = 42;
const int led2_B = 44;



const int S0 = 7;         // Seleção de escala de frequência - bit 0
const int S1 = 6;         // Seleção de escala de frequência - bit 1
const int S2 = 9;         // Seleção do filtro de cor - bit 0
const int S3 = 10;        // Seleção do filtro de cor - bit 1
const int sensorOut = 8;  // Saída de pulsos do sensor (frequência proporcional à cor detectada)


// ------------------ Variáveis de calibração ------------------
// Frequências medidas durante calibração das superfícies de referência
unsigned long R_branco, G_branco, B_branco;  // valores sobre branco
unsigned long R_preto, G_preto, B_preto;     // valores sobre preto
unsigned long R_verde, G_verde, B_verde;     // valores sobre verde


// Leituras atuais do sensor (em Hz aproximado)
unsigned long R_atual, G_atual, B_atual;


// =============================================================
//                      CONFIGURAÇÃO INICIAL
// =============================================================
void setup() {
  Serial.begin(9600);
  Serial.println("=== Calibracao do Sensor TCS3200 ===");
  Serial.println("Pinos configurados para Arduino Mega");
  Serial.println("------------------------------------");

  // Botão com pull-up interno
  pinMode(botao, INPUT_PULLUP);

  // Configuração dos pinos do sensor
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // Define escala de frequência de saída do TCS3200 (S0=HIGH, S1=LOW → 20%)
  // Isso reduz a frequência de saída, tornando a leitura mais estável.
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  delay(500);

  // por enquato no refino do sensor de cor não vou usar os leds.
  
  // ---- Calibração BRANCO ----
  Serial.println("1) Aponte o sensor para o BRANCO e pressione o botao...");
  esperarBotao();                            // espera o usuário confirmar
  medirCores(R_branco, G_branco, B_branco);  // mede as 3 cores
  mostrarCalibracao("Branco", R_branco, G_branco, B_branco);

  // ---- Calibração PRETO ----
  Serial.println("\n2) Aponte o sensor para o PRETO e pressione o botao...");
  esperarBotao();
  medirCores(R_preto, G_preto, B_preto);
  mostrarCalibracao("Preto", R_preto, G_preto, B_preto);

  /* 
  Não vou ustilizar mais a calibração do verde pois notei que ele no loop principal nunca era realmente 
  utilizado. Já tinha notado ano passado mas deixei para revisar esse ano no meu projeto integrador.
  O fluxo não muda nd, ele apenas le oque e maximo e minimo de claridade e o RGB funciona nesse maximo e
  minimo, para o sensor saber oque e preto e branco e n ficar ossilando doidamente,
  */

  Serial.println("\nCalibracao concluida. Iniciando leitura continua...\n");
  delay(500);
}


// =============================================================
//                      LOOP PRINCIPAL
// =============================================================
void loop() {
  // Mede os valores atuais de R, G e B
  medirCores(R_atual, G_atual, B_atual);

  // Normaliza os valores com base nos limites preto e branco
  // A função map() converte o intervalo [preto, branco] para [0, 1023]
  // constrain() garante que o resultado fique dentro dos limites válidos
  // apenas teste, vou usar de 10bits para ver se consigo uma margem maior para trabalhar
  int R_norm = constrain(map(R_atual, R_preto, R_branco, 0, 1023), 0, 1023);
  int G_norm = constrain(map(G_atual, G_preto, G_branco, 0, 1023), 0, 1023);
  int B_norm = constrain(map(B_atual, B_preto, B_branco, 0, 1023), 0, 1023);

  // Exibe as leituras normalizadas (0–255)
  Serial.print("R: ");
  Serial.print(R_norm);
  Serial.print("  G: ");
  Serial.print(G_norm);
  Serial.print("  B: ");
  Serial.print(B_norm);
  Serial.print("   -> ");

  // Identificação simples de cor (classificação básica)
  // Pr == provavelmente -> para n travar o arduino
  if (R_norm > 150 && G_norm > 150 && B_norm > 150)
    Serial.println("Pr BRANCO");
  else if (R_norm > 2*G_norm && R_norm > 2*B_norm)
    Serial.println("Pr VERMELHO");
  else if (G_norm > R_norm && G_norm > 2 * B_norm)
    Serial.println("Pr VERDE");
  else if (B_norm > R_norm && B_norm > G_norm)
    Serial.println("Pr AZUL");
  else if (R_norm < 30 && G_norm < 30 && B_norm < 30)
    Serial.println("Pr PRETO");



  /*
  else
    Serial.println("Cor intermediaria ou mista");
  */
  delay(50);  // intervalo entre leituras
}


// =============================================================
//                      FUNÇÕES AUXILIARES
// =============================================================     

// Aguarda o receber INPUT do Serial da IDE
void esperarBotao() {
  while (Serial.available() == 0){
    Serial.read(); // lê oque entra no Serial da IDE
    }
  delay(200);
  while (Serial.available() >= 1){
    Serial.read();
  }
  delay(200);
}

// --- Mede R, G e B individualmente usando os filtros do TCS3200 ---
void medirCores(unsigned long &R, unsigned long &G, unsigned long &B) {
  // Seleciona filtro vermelho (S2=LOW, S3=LOW)
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  delay(50);
  R = medirFreq();  // mede frequência correspondente à cor vermelha

  // Seleciona filtro verde (S2=HIGH, S3=HIGH)
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  delay(50);
  G = medirFreq();

  // Seleciona filtro azul (S2=LOW, S3=HIGH)
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  delay(50);
  B = medirFreq();
}


// --- Mede a frequência do pino OUT (proporcional à intensidade da cor) ---
unsigned long medirFreq() {
  // pulseIn mede o tempo em microssegundos de um pulso LOW
  // 100000 µs = 100 ms → tempo máximo de espera
  unsigned long duracao = pulseIn(sensorOut, LOW, 100000);
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
