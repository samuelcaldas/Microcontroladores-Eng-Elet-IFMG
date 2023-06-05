/**
 * @file Aula Prática 3 - Programa 2.ino
 * @brief Aula Prática 3 - Programação e Montagem de Hardware na Plataforma Arduino
 * @version 1.0
 * @date 2023-06-04
 *
 * A. O roteiro envolve o desenvolvimento de uma lógica em que dois botões controlem o brilho de um LED, um aumente e o outro diminua o brilho.
 * B. Deve conter 25/26 níveis com intervalo de 200ms.
 * C. Quando chegar ao máximo ou mínimo, o botão correspondente não deverá mais atuar no brilho.
 * D. Se chegou no máximo/mínimo, devera ficar assim ate que o botão "oposto" seja pressionado.
 */

// Definir os pinos dos botões e do LED
#define LED_PIN 9
#define BUTTON_UP 2
#define BUTTON_DOWN 3

// Variável para armazenar o brilho atual do LED
int brightness = 0;

// Variável para armazenar o intervalo de tempo entre cada atualização do brilho
int intervalo = 200;

void setup()
{
  // Configurar o pino do LED como saída
  pinMode(LED_PIN, OUTPUT);

  // Configurar os pinos dos botões como entrada
  pinMode(BUTTON_UP, INPUT);
  pinMode(BUTTON_DOWN, INPUT);
}

void loop()
{

  // Verificar se o botão para aumentar o brilho foi pressionado
  if (digitalRead(BUTTON_UP) == HIGH && brightness < 255)
  {
    // Aumentar o brilho do LED de 0 a 255 em incrementos de 10 e com intervalo de 200ms
    for (brightness = 0; brightness <= 255; brightness += 10)
    {
      // Atualizar o brilho do LED
      analogWrite(LED_PIN, brightness);
      delay(intervalo);
    }
  }

  // Verificar se o botão para diminuir o brilho foi pressionado
  if (digitalRead(BUTTON_DOWN) == HIGH && brightness > 0)
  {
    // Diminuir o brilho do LED de 255 a 0 em incrementos de -10 e intervalo de 200ms
    for (brightness = 255; brightness >= 0; brightness -= 10)
    {
      // Atualizar o brilho do LED
      analogWrite(LED_PIN, brightness);
      delay(intervalo);
    }
  }
}
