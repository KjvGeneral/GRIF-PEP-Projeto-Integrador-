// ---------------------- SENSOR ULTRASSOM -----------------------------

#define TRIG 34
#define ECHO 33

unsigned long distacia;


// ---------------------- PINOS DA PONTE H -----------------------------
//      Canal A (motor ESQUERDO)
#define A_ho 4   // -> Sentido Horário
#define A_ah 9   // -> Sentido Anti-horário
#define A_pwm 6  // -> Velocidade - PWM do motor
#define A_En A1  // -> Enable do canal A

//      Canal B (motor DIREITO)
#define B_ho 7   // -> Sentido Horário
#define B_ah 8   // -> Sentido Anti-horário
#define B_pwm 5  // -> Velocidade - PWM do motor
#define B_En A0  // -> Enable do canal B

const int monster[8] = { A_ho, A_ah, A_pwm, A_En, B_ho, B_ah, B_pwm, B_En };

// ---------------------- SENSORES ------------------------------------
// pin[0] = sensor mais à ESQUERDA ... pin[5] = sensor mais à DIREITA
const int pin[6] = { A8, A9, A10, A11, A12, A13 };

// "Peso" de cada sensor = distância dele até o centro do robô.
// Negativo = esquerda, positivo = direita. O centro fica entre S2 e S3.
const int PESO[6] = { -5, -3, -1, 1, 3, 5 };

int BRA_BASE[6];          // Valor calibrado no branco
int PRE_BASE[6];          // Valor calibrado no preto
int CORTE[6];             // Média entre branco e preto (limite de decisão)
int LEITURA_ATUAL[6];     // Leitura analógica atual
int LEITURA_BIN[6];       // 1 = vendo linha, 0 = fora da linha
bool PRETO_MAIOR = true;  // true se o preto lê valor MAIOR que o branco

// ---------------------- BOTÃO E LED ---------------------------------
// Botão ligado entre o pino 24 e o GND (usa o pull-up interno):
//   solto = HIGH, apertado = LOW.
const int bot = 28;

// LED simples no pino 48 (com resistor de ~220 a 330 ohms em série).
const int ledL = 32; // AZUL -- Responsavel pela linha ao vivo
const int ledC = 30; // ROXO -- Responsavel pelo sensor de cor

// Velocidade do pisca (em ms) para cada etapa de espera do botão
const int PISCA_BRANCO = 500;   // lento        -> coloque no BRANCO
const int PISCA_PRETO = 150;    // rápido       -> coloque no PRETO
const int PISCA_PRONTO = 1000;  // bem lento    -> aperte para LARGAR
const int PISCA_PAUSA = 80;     // muito rápido -> robô pausado


float Kp = 50.0;
float Ki = 0.0;
float Kd = 0.5;

int VEL_BASE = 90;
int VEL_MAX = 255;
int VEL_MIN = -80;

// Limite da soma do erro (evita a integral crescer sem fim = "windup")
const float LIMITE_INTEGRAL = 100.0;

// ---------------------------------------------------------------------
// PERIODO_US -> de quanto em quanto tempo o PID é calculado (microssegundos)
// ---------------------------------------------------------------------
//   O PID roda SEMPRE no mesmo intervalo, não importa quanto o resto do
//   código demore. Assim a derivada e a integral têm um "dt" fixo e os
//   valores de Kp/Ki/Kd não mudam de efeito se você adicionar Serial etc.
//   3000 us = 3 ms = ~333 cálculos por segundo.
//   DIMINUA (ex.: 2000) para reagir mais rápido em alta velocidade.
//   AUMENTE (ex.: 5000) se o Kd estiver muito "nervoso".
//   ATENÇÃO: se mudar este valor, reajuste o Kd (e o Ki), pois o efeito
//   deles depende do intervalo. Ele deve ser MAIOR que o tempo do loop
//   (com Serial.print ligado o loop pode passar de 3 ms!).
const unsigned long PERIODO_US = 3000;
unsigned long ultimoPID = 0;

// Variáveis internas do PID
float erro = 0;
float erroAnterior = 0;
float integral = 0;


//----------------------------------TCS32000-----------------------------

//  -    Sensor lado Direito -> D = Direita    -

#define D_S0 51   // Seleção de escala de frequência - bit 0.
#define D_S1 53   // Seleção de escala de frequência - bit 1.
#define D_S2 47   // Seleção de filtro de cor.
#define D_S3 45   // Seleção de filtro de cor.
#define D_Out 49  // Sainda de pulsos (proporcional a frequência detectada).

//        Variaveis de calibração

unsigned long DR_branco, DG_branco, DB_branco;
unsigned long DR_preto,  DG_preto,  DB_preto;
unsigned long DR_verde,  DG_verde,  DB_verde;
unsigned long DR_verm,   DG_verm,   DB_verm;


//  -    Sensor lado Esquerdo -> E = Esquerda    -

#define E_S0 50   // Seleção de escala de frequencia - bit 0.
#define E_S1 52   // Seleção de escala de frequencia - bit 1.
#define E_S2 46   // Seleção do filtro de cor.
#define E_S3 44   // Seleção de filtro de cor.
#define E_Out 48  // Saida de pulsos (proporcional a frequênia detectada).

//       Variaveis de Calibração

unsigned long ER_branco, EG_branco, EB_branco;
unsigned long ER_preto,  EG_preto,  EB_preto;
unsigned long ER_verde,  EG_verde,  EB_verde;
unsigned long ER_verm,   EG_verm,   EB_verm;

// Variaveis atuais 

unsigned long DR_atual, DG_atual, DB_atual; // leitura atual do lado direito
unsigned long ER_atual, EG_atual, EB_atual; // leitura atual do lado esquerd

const int Tcs3200[10] = {D_S0, D_S1, D_S2, D_S3, D_Out, E_S0, E_S1, E_S2, E_S3, E_Out}; // Pinos dos sensores TCS3200

/* ===================================================================== */

void setup() {
  Serial.begin(9600);

  //      MONSTER
  for (int i = 0; i < 8; i++) {  // CORRIGIDO: eram só 7 pinos
    pinMode(monster[i], OUTPUT);
  }
  digitalWrite(A_En, HIGH);
  digitalWrite(B_En, HIGH);
  //Motor(0,0);

  //      SENSOR DE COR TCS3200
  for(int i = 0; i < 10; i++){

    pinMode(Tcs3200[i], OUTPUT);
    Serial.print(Tcs3200[i]);
    Serial.println(" - Foi configurado como OUTPUT com sucesso!!");

    if(Tcs3200[i] == D_Out || Tcs3200[i] == E_Out){

      pinMode(Tcs3200[i], INPUT);
      Serial.print(Tcs3200[i]);
      Serial.println(" - Foi configurado como INPUT com sucesso!!");
    }
  }

  digitalWrite(E_S0, HIGH); digitalWrite(E_S1, LOW); // Define com escala de 20%
  digitalWrite(D_S0, HIGH); digitalWrite(D_S1, LOW); // Define com escala de 20% 

  pinMode(bot, INPUT_PULLUP);
  pinMode(ledL, OUTPUT);
  pinMode(ledC, OUTPUT);
  digitalWrite(ledL, LOW);

  delay(500);
  

  //     Calibração dos Sensore de cor - TCS3200
  //     = Sensor Esquerdo =

  Serial.println("\n1-) Aponte para o BRANCO com o sensor ESQUERDO!...\n");
  esperarBotao(ledC, PISCA_BRANCO);
  medirCores(E_S2, E_S3, E_Out, ER_branco, EG_branco, EB_branco);
  mostrarCalibracao("Branco Esquerdo", ER_branco, EG_branco, EB_branco);
  Serial.println("Branco Calibrado");
  delay(100);

  Serial.println("\n2-) Aponte para o PRETO com o sensor ESQUERDO!...\n");
  esperarBotao(ledC, PISCA_PRETO);
  medirCores(E_S2, E_S3, E_Out, ER_preto, EG_preto, EB_preto);
  mostrarCalibracao("Preto Esquerdo", ER_preto, EG_preto, EB_preto);
  Serial.println("Preto Calibrado");
  delay(100);

  Serial.println("\n3-) Aponte para o VERDE com o sensor ESQUERDo!...\n");
  esperarBotao(ledC, PISCA_PAUSA);
  medirCores(E_S2, E_S3, E_Out, ER_verde, EG_verde, EB_verde);
  mostrarCalibracao("Verde Esquerdo", ER_verde, EG_verde, EB_verde);
  Serial.println("Verde calibrado");
  delay(100);

  Serial.println("\n4-) Aponte para o VERMELHO com o sensor ESQUERDo!...\n");
  esperarBotao(ledC, PISCA_PRETO);
  medirCores(E_S2, E_S3, E_Out, ER_verm, EG_verm, EB_verm);
  mostrarCalibracao("Verde Esquerdo", ER_verm, EG_verm, EB_verm);
  Serial.println("Vermelho calibrado");
  delay(300);


  //    = Sensor Direito =
  Serial.println("\n4-) Aponte para o BRANCO com o sensor DIREITO!...\n");
  esperarBotao(ledC, PISCA_BRANCO);
  medirCores(D_S2, D_S3, D_Out, DR_branco, DG_branco, DB_branco);
  mostrarCalibracao("Branco Direito", DR_branco, DG_branco, DB_branco);
  Serial.println("Branco Calibrado");

  Serial.println("\n5-) Aponte para o PRETO com o sensor DIREITO!...\n");
  esperarBotao(ledC, PISCA_PRETO);
  medirCores(D_S2, D_S3, D_Out, DR_preto, DG_preto, DB_preto);
  mostrarCalibracao("Preto Calibrado", DR_preto, DG_preto, DB_preto);

  Serial.println("\n6Aponte para o VERDE com o sensor DIREITO!...\n");
  esperarBotao(ledC, PISCA_PAUSA);
  medirCores(D_S2, D_S3, D_Out, DR_verde, DG_verde, DB_verde);
  mostrarCalibracao("Verde Calibrado", DR_verde, DG_verde, DB_verde);

  Serial.println("\n7-) Aponte para o VERMELHO com o sensor DIREITO!...\n");
  esperarBotao(ledC, PISCA_PRETO);
  medirCores(D_S2, D_S3, D_Out, DR_verm, DG_verm, DB_verm);
  mostrarCalibracao("Vermelho Direito", DR_verm, DG_verm, DB_verm);
  Serial.println("Vermelho calibrado");
  int DR;
  int ES;

  digitalWrite(ledL, HIGH);
  esperarBotao(ledC, PISCA_PAUSA);

  digitalWrite(ledC, LOW); digitalWrite(ledL, LOW);
  


  // =========================== TCRT5000 ==============================
  // ---------- 1) Calibração do BRANCO (parado) ----------
  Serial.println("1) Coloque TODOS os sensores no BRANCO e aperte o botao (pisca lento)");
  esperarBotao(ledL, PISCA_BRANCO);
  digitalWrite(ledL, HIGH);  // aceso fixo = lendo...
  Serial.print("BRANCO: ");
  for (int i = 0; i < 6; i++) {
    BRA_BASE[i] = calibrarIR(pin[i]);
    PRE_BASE[i] = BRA_BASE[i];  // ponto de partida para buscar o pico do preto
    Serial.print(BRA_BASE[i]);
    Serial.print(" | ");
  }
  Serial.println();
  digitalWrite(ledL, LOW);

  // ---------- 2) Calibração do PRETO (varredura de 4 segundos) ----------
  // Como a linha é mais estreita que a barra de 6 sensores, não dá para
  // parar com todos em cima do preto ao mesmo tempo. Então você aperta o
  // botão e MOVE o robô de um lado para o outro sobre a linha por 4 s
  // enquanto o LED estiver ACESO FIXO, garantindo que TODOS os 6 sensores
  // cruzem a fita preta pelo menos uma vez.
  Serial.println("2) Aperte o botao e, com o LED ACESO, passe TODOS os sensores sobre a linha PRETA por 4s (pisca rapido)");
  esperarBotao(ledL, PISCA_PRETO);
  digitalWrite(ledL, HIGH);  // aceso fixo durante os 4 s de varredura

  int minLido[6], maxLido[6];
  for (int i = 0; i < 6; i++) {
    minLido[i] = BRA_BASE[i];
    maxLido[i] = BRA_BASE[i];
  }

  unsigned long inicioCal = millis();
  while (millis() - inicioCal < 4000) {
    for (int i = 0; i < 6; i++) {
      int v = analogRead(pin[i]);
      if (v < minLido[i]) minLido[i] = v;
      if (v > maxLido[i]) maxLido[i] = v;
    }
    delay(2);
  }
  digitalWrite(ledL, LOW);

  // Descobre para qual lado (acima ou abaixo do branco) os sensores mudam
  // mais quando passam na linha preta, e salva o pico de cada sensor.
  long somaSubiu = 0, somaDesceu = 0;
  for (int i = 0; i < 6; i++) {
    somaSubiu += (maxLido[i] - BRA_BASE[i]);
    somaDesceu += (BRA_BASE[i] - minLido[i]);
  }
  PRETO_MAIOR = (somaSubiu >= somaDesceu);

  Serial.print("PRETO:  ");
  for (int i = 0; i < 6; i++) {
    PRE_BASE[i] = PRETO_MAIOR ? maxLido[i] : minLido[i];
    Serial.print(PRE_BASE[i]);
    Serial.print(" | ");
  }
  Serial.println();

  // ---------- Linha de corte ----------
  Serial.print("CORTE:  ");
  for (int i = 0; i < 6; i++) {
    CORTE[i] = (BRA_BASE[i] + PRE_BASE[i]) / 2;
    Serial.print(CORTE[i]);
    Serial.print(" | ");
  }
  Serial.println();
  Serial.println(PRETO_MAIOR ? "Preto le MAIOR que branco" : "Preto le MENOR que branco");

  // ---------- 3) Teste ao vivo antes de largar ----------
  // Enquanto espera o botão de largada, o LED mostra AO VIVO se algum sensor
  // está vendo a linha (ACESO = vendo linha, APAGADO = todo no branco).
  // Passe o robô na mão sobre a linha e fora dela para confirmar ANTES de soltar!
  Serial.println("3) Calibrado! Teste na mao: LED acende na linha e apaga no branco. Aperte o botao para LARGAR.");
  esperarLargadaComTeste();
  digitalWrite(ledL, HIGH);
  ultimoPID = micros();  // começa a contar o período a partir daqui
}

//====================================================================
//                            LOOP
//====================================================================

void loop() {
  // 0) PERÍODO FIXO: só executa quando passar PERIODO_US desde o último PID
  unsigned long agora = micros();
  if (agora - ultimoPID < PERIODO_US) return;  // ainda não deu o tempo
  ultimoPID += PERIODO_US;
  // Se o loop atrasou muito (ex.: Serial lento), ressincroniza
  // para não executar várias vezes seguidas tentando "recuperar".
  if (agora - ultimoPID > PERIODO_US) ultimoPID = agora;

  // BOTÃO DURANTE A CORRIDA: aperta = pausa, aperta de novo = volta
  if (digitalRead(bot) == LOW) {
    Motor(0, 0);
    while (digitalRead(bot) == LOW) {}  // espera soltar
    delay(50);                          // debounce
    esperarBotao(ledL, PISCA_PAUSA);          // pausado até apertar de novo
    erroAnterior = 0;                   // zera a memória do PID
    integral = 0;
    digitalWrite(ledL, HIGH);
    ultimoPID = micros();
    return;
  }

  // 1) LER TODOS OS SENSORES (antes de decidir qualquer coisa)
  int ativos = 0;     // quantos sensores estão vendo a linha
  int somaPesos = 0;  // soma dos pesos dos sensores ativos

  for (int i = 0; i < 6; i++) {
    LEITURA_ATUAL[i] = analogRead(pin[i]);

    bool acimaDoCorte = (LEITURA_ATUAL[i] >= CORTE[i]);
    LEITURA_BIN[i] = (acimaDoCorte == PRETO_MAIOR) ? 1 : 0;

    if (LEITURA_BIN[i] == 1) {
      ativos++;
      somaPesos += PESO[i];
    }
  }

  // 2) CALCULAR O ERRO (posição da linha)
  if (ativos == 6) {
    // Todos no preto: cruzamento ou marca de chegada -> segue reto.
    // (Se quiser que pare, troque por: Motor(0, 0); return;)
    erro = 0;
    digitalWrite(ledL, HIGH);
  } else if (ativos == 0) {
    // Linha perdida: usa o lado em que ela foi vista por último
    // e força a curva máxima para esse lado.
    erro = (erroAnterior < 0) ? -6 : 6;
    digitalWrite(ledL, LOW);  // apaga quando perde a linha
  } else {
    digitalWrite(ledL, HIGH);  // aceso enquanto vê a linha
    // Média dos pesos dos sensores ativos.
    // Ex.: S2 e S3 ativos -> (-1 + 1) / 2 = 0  (centro)
    //      S0 e S1 ativos -> (-5 + -3) / 2 = -4 (linha bem à esquerda)
    erro = (float)somaPesos / ativos;
  }

  // 3) PID
  integral += erro;
  integral = constrain(integral, -LIMITE_INTEGRAL, LIMITE_INTEGRAL);
  if (erro == 0) integral = 0;  // zera a memória quando centralizado

  float derivada = erro - erroAnterior;
  float correcao = (Kp * erro) + (Ki * integral) + (Kd * derivada);
  erroAnterior = erro;

  // 4) APLICAR NOS MOTORES
  // Linha à direita (erro > 0) -> roda esquerda acelera, direita freia.
  int velEsq = VEL_BASE + correcao;
  int velDir = VEL_BASE - correcao;

  velEsq = constrain(velEsq, VEL_MIN, VEL_MAX);
  velDir = constrain(velDir, VEL_MIN, VEL_MAX);

  Motor(velEsq, velDir);

  // Depuração: descomente para ver no Serial Plotter.
  // (Deixe comentado na pista: o Serial deixa o loop mais lento.)
  // Serial.print(erro); Serial.print(" ");
  // Serial.print(velEsq); Serial.print(" ");
  // Serial.println(velDir);
  // (Se ligar o Serial, use Serial.begin(115200) no setup para não
  //  estourar o PERIODO_US.)   

  //--------------------------- COR ------------------------------
  /*

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
    //Serial.println("D__BRANCO");
  }
  else if((DR_atual < DR_verde || DG_atual >= DG_verde || DB_atual < DB_atual) || (DR_norm < DG_norm && DB_norm < DG_norm)){
    //Serial.println("D__VERDE");
    digitalWrite(led, LOW);
    delay(50);
    digitalWrite(led, HIGH);
  }
  else if((DR_atual == DR_verm || DG_atual <= DG_verm || DB_atual <= DB_verm) || (DR_norm == DR_norm && DG_norm < DR_norm && DB_norm < DR_norm)){
    //Serial.println("D_VERMEHO");
  }
  else if((DR_atual == DR_preto || DG_atual == DG_preto || DB_atual == DB_preto) || (DR_norm < 150 && DG_norm < 150 && DB_norm < 150)){
    //Serial.println("D__PRETO");
  }

  // - Sensor Esquerdo -

  else if((ER_atual == ER_branco || EG_atual == EG_branco || EB_atual == EG_branco) || (ER_norm >= 700 && EG_norm >= 700 && EB_norm >= 700)){
    //Serial.println("E__BRANCO");
  }
  else if((ER_atual < ER_verde || EG_atual == EG_verde || EB_atual < EB_verde) || (ER_norm < ER_verde && EB_norm < EB_verde)){
    //Serial.println("E__VERDE");
    digitalWrite(led, LOW);
    delay(50);
    digitalWrite(led, HIGH);
  }
  else if((ER_atual == ER_verm || EG_atual <= EG_verm || EB_atual <= EB_verm) || (ER_norm == ER_norm && EG_norm < ER_norm && EB_norm < ER_norm)){
    //Serial.println("E_VERMEHO");
  }
  else if((ER_atual == ER_preto || EG_atual == EG_preto || EB_atual == EB_preto) || (ER_preto < 150 && EG_preto < 150 && EB_preto < 150)){
    //Serial.println("E__PRETO");
  }
  else{
    //Serial.println("Nsei");
    /* O obejtivo futuro e tornar esse else como um retorno de "erro" que a pode ocorrer caso o sensor falhe, então salvar o valor lido
    anterior e aplicar nesse else para n ser uma ação perdida.*/


}


//=========================================================================
//                            FUNÇÕES AUX
//=========================================================================

// Média de 20 leituras para reduzir ruído
int calibrarIR(int pino) {
  long soma = 0;
  for (int i = 0; i < 20; i++) {
    soma += analogRead(pino);
    delay(2);
  }
  return soma / 20;
}

// Trava o código até o botão ser apertado E solto.
// Enquanto espera, o LED pisca na velocidade indicada (intervaloMs).
// (Também aceita qualquer tecla pelo Serial, útil para testar na bancada.)
void esperarBotao(int led, int intervaloMs) {
  unsigned long ultimoPisca = millis();
  bool aceso = true;
  digitalWrite(led, HIGH);

  while (true) {
    // Pisca conforme o intervalo da etapa
    if (millis() - ultimoPisca >= (unsigned long)intervaloMs) {
      ultimoPisca = millis();
      aceso = !aceso;
      digitalWrite(led, aceso ? HIGH : LOW);
    }

    // Botão apertado? Confirma depois de 30 ms (debounce = ignora "tremida")
    if (digitalRead(bot) == LOW) {
      delay(30);
      if (digitalRead(bot) == LOW) {
        while (digitalRead(bot) == LOW) {}  // espera SOLTAR
        delay(30);
        break;
      }
    }

    // Alternativa pelo Serial
    if (Serial.available() > 0) {
      delay(50);
      while (Serial.available() > 0) Serial.read();
      break;
    }
  }

  digitalWrite(led, LOW);
  delay(300);  // tempo para tirar a mão antes de ler os sensores
}

// Antes de largar, mostra AO VIVO no LED se algum sensor está vendo a linha:
//   LED ACESO   = pelo menos 1 sensor vendo a linha preta
//   LED APAGADO = todos os 6 sensores no branco
// Assim você consegue testar com a mão antes dos motores ligarem!
void esperarLargadaComTeste() {
  while (true) {
    bool vendoLinha = false;
    for (int i = 0; i < 6; i++) {
      int v = analogRead(pin[i]);
      bool acima = (v >= CORTE[i]);
      if (acima == PRETO_MAIOR) {
        vendoLinha = true;
        break;
      }
    }
    digitalWrite(ledL, vendoLinha ? HIGH : LOW);

    if (digitalRead(bot) == LOW) {
      delay(30);
      if (digitalRead(bot) == LOW) {
        while (digitalRead(bot) == LOW) {}
        delay(300);  // tempo para tirar a mão antes do robô sair
        break;
      }
    }
    if (Serial.available() > 0) {
      delay(50);
      while (Serial.available() > 0) Serial.read();
      break;
    }
  }
}

// COR
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
  digitalWrite(pin_s2, LOW);
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


// Controla os dois motores com velocidade de -255 a 255.
// Positivo = frente, negativo = ré, 0 = parado.
// Se alguma roda girar ao contrário, troque HIGH/LOW dela aqui.
void Motor(int velEsq, int velDir) {
  // Canal A - esquerdo
  digitalWrite(A_ho, velEsq >= 0 ? HIGH : LOW);
  digitalWrite(A_ah, velEsq >= 0 ? LOW : HIGH);
  analogWrite(A_pwm, abs(velEsq));  // CORRIGIDO: era digitalWrite

  // Canal B - direito
  digitalWrite(B_ho, velDir >= 0 ? HIGH : LOW);
  digitalWrite(B_ah, velDir >= 0 ? LOW : HIGH);
  analogWrite(B_pwm, abs(velDir));
}
