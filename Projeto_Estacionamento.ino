// =====================
// CONFIGURAÇÃO
// =====================
const int NUM_VAGAS = 20;
const int DISTANCIA_OCUPADA = 6;

// Pinos TRIG
int trigPins[NUM_VAGAS] = {
  2,4,6,8,10,12,14,16,18,20,
  22,24,26,28,30,32,34,36,38,40
};

// Pinos ECHO
int echoPins[NUM_VAGAS] = {
  3,5,7,9,11,13,15,17,19,21,
  23,25,27,29,31,33,35,37,39,41
};

// Estado das vagas
bool vaga[NUM_VAGAS];
bool estadoAnt[NUM_VAGAS];

// Tempo de entrada
unsigned long entrada[NUM_VAGAS];


// =====================
// SETUP
// =====================
void setup() {

  Serial.begin(9600);

  for(int i = 0; i < NUM_VAGAS; i++){
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }

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

  Serial.println("-----------------------");

  for(int i = 0; i < NUM_VAGAS; i++){

    float distancia = medirDistancia(trigPins[i], echoPins[i]);

    vaga[i] = (distancia < DISTANCIA_OCUPADA);

    Serial.print("VAGA ");
    Serial.print(i+1);
    Serial.print(": ");

    if(vaga[i]){
      Serial.println("OCUPADA");
    }else{
      Serial.println("LIVRE");
    }

    Serial.print("distancia: ");
    Serial.println(distancia);


    // REGISTRO DE EVENTOS
    if(vaga[i] != estadoAnt[i]){

      if(vaga[i]){

        entrada[i] = millis();

        Serial.print(">>> VAGA ");
        Serial.print(i+1);
        Serial.println(" - ENTRADA REGISTRADA");

      }else{

        unsigned long tempo = millis() - entrada[i];

        Serial.print(">>> VAGA ");
        Serial.print(i+1);
        Serial.print(" - SAIDA | Tempo: ");
        Serial.print(tempo / 1000);
        Serial.println(" s");

      }

      estadoAnt[i] = vaga[i];
    }

    delay(500);
  }

  delay(1500);
}