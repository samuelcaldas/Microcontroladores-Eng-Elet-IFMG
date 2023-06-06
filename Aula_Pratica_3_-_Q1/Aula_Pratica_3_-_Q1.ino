/**
 * @file Aula Prática 3 - Programa 1.ino
 * @brief Aula Prática 3 - Programação e Montagem de Hardware na Plataforma Arduino
 * @version 1.0
 * @date 2023-06-04
 *
 * A. O roteiro envolve o desenvolvimento de uma lógica em que um LED varie seu brilho automaticamente com 51 niveis diferentes com intervalo de 100ms.
 * B. Quando chegar ao máximo, deve ir para o mínimo em processo inverso.
 * C. Adicione mais 3 LEDs, um para indicar o brilho mínimo (<10%), outro para indicar o médio (45 a 55%), e o último para indicar o máximo (>90%).
 */

// Definir os pinos dos LEDs
#define LED_PIN 9
#define LED_MIN 10
#define LED_MED 11
#define LED_MAX 12

void setup()
{
  // Configurar os pinos dos LEDs como saída
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_MIN, OUTPUT);
  pinMode(LED_MED, OUTPUT);
  pinMode(LED_MAX, OUTPUT);
}

void loop()
{
  // Aumentar o brilho do LED de 0 a 255 em incrementos de 5
  for (int i = 0; i <= 255; i += 5)
  {
    analogWrite(LED_PIN, i);
    delay(100);

    // Acender o LED correspondente ao nível de brilho atual
    if (i < 25)
    {
      digitalWrite(LED_MIN, HIGH);
      digitalWrite(LED_MED, LOW);
      digitalWrite(LED_MAX, LOW);
    }
    else if (i >= 114 && i <= 140)
    {
      digitalWrite(LED_MIN, LOW);
      digitalWrite(LED_MED, HIGH);
      digitalWrite(LED_MAX, LOW);
    }
    else if (i > 229)
    {
      digitalWrite(LED_MIN, LOW);
      digitalWrite(LED_MED, LOW);
      digitalWrite(LED_MAX, HIGH);
    }
    else
    {
      digitalWrite(LED_MIN, LOW);
      digitalWrite(LED_MED, LOW);
      digitalWrite(LED_MAX, LOW);
    }
  }

  // Diminuir o brilho do LED de 255 a 0 em incrementos de -5
  for (int i = 255; i >= 0; i -= 5)
  {
    analogWrite(LED_PIN, i);
    delay(100);

    // Acender o LED correspondente ao nível de brilho atual
    if (i < 25)
    {
      digitalWrite(LED_MIN, HIGH);
      digitalWrite(LED_MED, LOW);
      digitalWrite(LED_MAX, LOW);
    }
    else if (i >= 114 && i <= 140)
    {
      digitalWrite(LED_MIN, LOW);
      digitalWrite(LED_MED, HIGH);
      digitalWrite(LED_MAX, LOW);
    }
    else if (i > 229)
    {
      digitalWrite(LED_MIN, LOW);
      digitalWrite(LED_MED, LOW);
      digitalWrite(LED_MAX, HIGH);
    }
    else
    {
      digitalWrite(LED_MIN, LOW);
      digitalWrite(LED_MED, LOW);
      digitalWrite(LED_MAX, LOW);
    }
  }
}
