/**
 * @file Aula Prática 8.ino
 * @brief Aula Prática 8 - LDR
 * @version 1.0
 * @date 2023-06-04
 *
 * A. O roteiro envolve o desenvolvimento de uma lógica em que um fotorresistor LDR é utilizado para medir a luminosidade do ambiente.
 * B. O código deve enviar através do monitor serial a proporção da resistência lida (0-100%) através do canal de entrada analógico.
 * C. O código deve acender um LED quando a luminosidade for abaixo de 50%.
 */

#define ledPin 13 // Define o pino do LED
#define ldrPin A0 // Define o pino do sensor LDR

void setup()
{
  // Inicializa o pino do LED como saída
  pinMode(ledPin, OUTPUT);

  // Inicializa a comunicação serial
  Serial.begin(9600);
}

void loop()
{
  // Lê o valor do sensor LDR
  int ldrValue = analogRead(ldrPin);

  // Converte o valor do sensor LDR para uma porcentagem de resistencia
  int resistencePercent = map(ldrValue, 0, 20, 0, 100);

  // Envia a porcentagem de luminosidade através do monitor serial
  Serial.print("Luminosidade: ");
  Serial.print(resistencePercent);
  Serial.println("%");

  // Acende o LED se a luminosidade for abaixo de 50%
  if (resistencePercent < 50)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }

  // Aguarda um pouco antes de ler novamente o sensor
  delay(1000);
}
