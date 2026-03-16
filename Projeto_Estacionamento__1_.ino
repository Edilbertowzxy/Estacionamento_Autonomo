// =====================
// DEFINIÇÃO DOS PINOS
// =====================
#define TRIG1 2
#define ECHO1 3

#define TRIG2 4
#define ECHO2 5

#define TRIG3 6
#define ECHO3 7

// Estado das vagas 
bool vaga1 = false;
bool vaga2 = false;
bool vaga3 = false;

bool estadoAnt1 = false;
bool estadoAnt2 = false;
bool estadoAnt3 = false;

// Tempo de entreda 
unsigned long entrada1 = 0;
unsigned long entrada2 = 0;
unsigned long entrada3 = 0;

// =====================
// SETUP
// =====================
void setup() {
  Serial.begin(9600);

  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);

  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  pinMode(TRIG3, OUTPUT);
  pinMode(ECHO3, INPUT);

  Serial.println("Sistema de Estacionamento Iniciado");
}

// =====================
// FUNÇÃO PARA MEDIR DISTÂNCIA
// =====================
float medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH);
  float distancia = duracao * 0.034 / 2;

  return distancia;
}

// =====================
// LOOP PRINCIPAL
// =====================
void loop() {
  float d1 = medirDistancia(TRIG1, ECHO1);
  float d2 = medirDistancia(TRIG2, ECHO2);
  float d3 = medirDistancia(TRIG3, ECHO3);

  Serial.println("-----------------------");

  // Atualiza estado atual
  vaga1 = (d1 < 6);
  vaga2 = (d2 < 6);
  vaga3 = (d3 < 6);


  if (vaga1) {
    Serial.println("VAGA 1: OCUPADA");
    Serial.print("distancia 1: ");
    Serial.println(d1);
  } else {
    Serial.println("VAGA 1: LIVRE");
    Serial.print("distancia 1: ");
    Serial.println(d1);
  }

  if (vaga2) {
    Serial.println("VAGA 2: OCUPADA");
    Serial.print("distancia 2: ");
    Serial.println(d2);
  } else {
    Serial.println("VAGA 2: LIVRE");
    Serial.print("distancia 2: ");
    Serial.println(d2);
  }

  if (vaga3) {
    Serial.println("VAGA 3: OCUPADA");
    Serial.print("distancia 3: ");
    Serial.println(d3);
  } else {
    Serial.println("VAGA 3: LIVRE");
    Serial.print("distancia 3: ");
    Serial.println(d3);
  }
    // REGISTRO DE EVENTOS
  if (vaga1 != estadoAnt1) {
    if (vaga1) {
      entrada1 = millis();
      Serial.println(">>> VAGA 1 - ENTRADA REGISTRADA");
    } else {
      unsigned long tempo = millis() - entrada1;
      Serial.print(">>> VAGA 1 - SAIDA | Tempo: ");
      Serial.print(tempo / 1000);
      Serial.println(" s");
    }
    estadoAnt1 = vaga1;
  }

  if (vaga2 != estadoAnt2) {
    if (vaga2) {
      entrada2 = millis();
      Serial.println(">>> VAGA 2 - ENTRADA REGISTRADA");
    } else {
      unsigned long tempo = millis() - entrada2;
      Serial.print(">>> VAGA 2 - SAIDA | Tempo: ");
      Serial.print(tempo / 1000);
      Serial.println(" s");
    }
    estadoAnt2 = vaga2;
  }

  if (vaga3 != estadoAnt3) {
    if (vaga3) {
      entrada3 = millis();
      Serial.println(">>> VAGA 3 - ENTRADA REGISTRADA");
    } else {
      unsigned long tempo = millis() - entrada3;
      Serial.print(">>> VAGA 3 - SAIDA | Tempo: ");
      Serial.print(tempo / 1000);
      Serial.println(" s");
    }
    estadoAnt3 = vaga3;
  }
  
  // 

  delay(1000); // a
}