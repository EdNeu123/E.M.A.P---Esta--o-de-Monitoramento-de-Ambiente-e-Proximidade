/*
 * Projeto: Estação de Monitoramento (Pinos customizados)
 * Autor: [Seu Nome/Nome da Equipe]
 *
 * Componentes:
 * - Sensores: DHT11 (Temp/Umidade), HC-SR04 (Distância)
 * - Atuadores: LED, Buzzer
 *
 * Mapeamento de Pinos:
 * - DHT11 (Data): D10
 * - HC-SR04 (Trig): D12
 * - HC-SR04 (Echo): D11
 * - LED: D7
 * - Buzzer: D13
 */

// Bibliotecas
#include <DHT.h> // Biblioteca para o sensor DHT

// --- Mapeamento de Pinos (Constantes) ---
#define DHTPIN 10       // Pino digital do DHT11 (Temp/Umidade)
#define HCTRIGPIN 12    // Pino Trigger do HC-SR04
#define HCECHOPIN 11    // Pino Echo do HC-SR04
#define LEDPIN 7        // Pino digital para o LED (Alerta de Temp)
#define BUZZERPIN 13    // Pino digital para o Buzzer (Alerta de Distância)

// --- Constantes de Lógica ---
#define TEMPERATURA_ALERTA 24 // Temp. em °C para ligar o LED
#define DISTANCIA_ALERTA 50     // Distância em cm para tocar o buzzer

// --- Configuração do Sensor DHT ---
#define DHTTYPE DHT11  // Define o tipo do sensor como DHT11
DHT dht(DHTPIN, DHTTYPE); // Inicializa o objeto DHT

// --- Variáveis Globais ---
float umidade;
float temperatura;
long duracao;
int distanciaCm;

// =================================================================
// FUNÇÃO SETUP: Executada uma vez
// =================================================================
void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando Estação de Monitoramento...");

  dht.begin(); // Inicializa o sensor DHT

  // Configura pinos dos sensores
  pinMode(HCTRIGPIN, OUTPUT);
  pinMode(HCECHOPIN, INPUT);

  // Configura pinos dos atuadores
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUZZERPIN, OUTPUT);

  // Garante que atuadores comecem desligados
  digitalWrite(LEDPIN, LOW);
  digitalWrite(BUZZERPIN, LOW);
}

// =================================================================
// FUNÇÃO LOOP: Executada continuamente
// =================================================================
void loop() {
  // --- 1. Leitura dos Sensores ---

  // Leitura 1 e 2: Umidade e Temperatura (DHT11)
  umidade = dht.readHumidity();
  temperatura = dht.readTemperature(); // Lê em Celsius

  // Verifica se a leitura do DHT falhou
  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Falha ao ler do sensor DHT!");
  } else {
    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.print("%\t");
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" *C");
  }

  // Leitura 3: Distância (HC-SR04)
  distanciaCm = lerDistancia();
  Serial.print("Distância: ");
  Serial.print(distanciaCm);
  Serial.println(" cm");


  // --- 2. Lógica dos Atuadores ---

  // Lógica 1: Alerta de Temperatura (LED)
  if (!isnan(temperatura) && temperatura > TEMPERATURA_ALERTA) {
    digitalWrite(LEDPIN, HIGH); // Liga o LED
    Serial.println("ALERTA: Temperatura alta!");
  } else {
    digitalWrite(LEDPIN, LOW); // Desliga o LED
  }

  // Lógica 2: Alerta de Proximidade (Buzzer)
  if (distanciaCm < DISTANCIA_ALERTA && distanciaCm > 0) {
    Serial.println("ALERTA: Objeto próximo!");
    // Toca um som curto (frequência, duração)
    tone(BUZZERPIN, 2000, 200); // 2000Hz por 200ms
  }
  // Não precisa de 'else' para desligar o tone com duração

  // --- 3. Delay ---
  Serial.println("-----------------");
  // Espera 2 segundos (o DHT11 não deve ser lido mais rápido que isso)
  delay(2000);
}

// =sem-fim
// FUNÇÃO AUXILIAR: Ler Distância do HC-SR04
// Retorna a distância em centímetros (cm)
// =================================================================
int lerDistancia() {
  // Limpa o HCTRIGPIN
  digitalWrite(HCTRIGPIN, LOW);
  delayMicroseconds(2);

  // Envia um pulso de 10 microssegundos
  digitalWrite(HCTRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(HCTRIGPIN, LOW);

  // Lê o tempo que o pino HCECHOPIN ficou em HIGH
  duracao = pulseIn(HCECHOPIN, HIGH);

  // Calcula a distância: (Tempo * 0.0343) / 2
  int distancia = duracao * 0.0343 / 2;
  return distancia;
}