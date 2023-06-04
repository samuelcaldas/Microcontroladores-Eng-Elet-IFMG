/**
 * @file Aula Prática 1.ino
 * @brief Aula Prática 1 - Programação e Montagem de Hardware na Plataforma Arduino
 * @version 1.0
 * @date 2023-06-04
 *
 * A. O roteiro envolve o desenvolvimento de uma lógica aplicada em um estacionamento de carros.
 * B. Um sensor S1 deve detectar a entrada de carros e incrementar a contagem.
 * C. Um sensor S2 deve detectar a saída de carros e decrementar a contagem.
 * D. A lotação máxima é de 15 carros.
 * E. Enquanto houver vagas, um led verde deve permanecer aceso.
 * F. Quando a lotação atingir 80%, um led amarelo deve piscar com intervalos de ½ segundos.
 * G. Quando a lotação máxima for atingida, um led vermelho deve acender indicando que não há vagas.
 */

int contador = 0;   // Contador de carros no estacionamento
int maximo = 15;    // Lotação máxima do estacionamento
int debounce = 200; // Tempo de debounce em milissegundos

// Pinos do Arduino
int sensorEntrada = 5; // Pino do sensor de entrada
int sensorSaida = 6;   // Pino do sensor de saída
int ledVerde = 2;      // Pino do LED verde
int ledAmarelo = 3;    // Pino do LED amarelo
int ledVermelho = 4;   // Pino do LED vermelho

void setup()
{
  Serial.begin(9600);            // Inicializa a comunicação serial
  pinMode(sensorEntrada, INPUT); // Configura o pino do sensor de entrada como entrada
  pinMode(sensorSaida, INPUT);   // Configura o pino do sensor de saída como entrada
  pinMode(ledVerde, OUTPUT);     // Configura o pino do LED verde como saída
  pinMode(ledAmarelo, OUTPUT);   // Configura o pino do LED amarelo como saída
  pinMode(ledVermelho, OUTPUT);  // Configura o pino do LED vermelho como saída
}

void loop()
{
  if (digitalRead(sensorEntrada) == HIGH) // Se o sensor de entrada detectar um carro
  {
    contador++;               // Incrementa o contador de carros
    Serial.println(contador); // Imprime a quantidade de carros no estacionamento na saída serial
    delay(debounce);          // Aguarda alguns milissegundos para evitar contagem dupla
  }

  if (digitalRead(sensorSaida) == HIGH) // Se o sensor de saída detectar um carro
  {
    contador--;               // Decrementa o contador de carros
    Serial.println(contador); // Imprime a quantidade de carros no estacionamento na saída serial
    delay(debounce);          // Aguarda alguns milissegundos para evitar contagem dupla
  }

  if (contador < maximo * 0.8)
  {                                 // Se a lotação for menor que 80%
    digitalWrite(ledVerde, HIGH);   // Acende o LED verde
    digitalWrite(ledAmarelo, LOW);  // Apaga o LED amarelo
    digitalWrite(ledVermelho, LOW); // Apaga o LED vermelho
  }
  else if (contador < maximo) // Se a lotação for entre 80% e 100%
  {
    digitalWrite(ledVermelho, LOW); // Apaga o LED vermelho
    digitalWrite(ledAmarelo, HIGH); // Acende o LED amarelo
    delay(500);                     // Aguarda meio segundo
    digitalWrite(ledAmarelo, LOW);  // Apaga o LED amarelo
    delay(500);                     // Aguarda meio segundo
  }
  else // Se a lotação for igual ou maior que 100%
  {
    digitalWrite(ledVerde, LOW);     // Apaga o LED verde
    digitalWrite(ledAmarelo, LOW);   // Apaga o LED amarelo
    digitalWrite(ledVermelho, HIGH); // Acende o LED vermelho
  }
}
