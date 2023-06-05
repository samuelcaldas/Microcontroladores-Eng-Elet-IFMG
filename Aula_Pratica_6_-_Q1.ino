/**
 * @file Aula Prática 6 - Programa 1.ino
 * @brief Aula Prática 6 - Servo motor controlado por botão
 * @version 1.0
 * @date 2023-06-04
 *
 * A. O roteiro envolve o desenvolvimento de uma lógica em que dois botões controlem o giro de um servo motor, um aumente e o outro diminua o giro.
 * B. Deve conter 180 níveis com intervalo de 15ms.
 * C. Quando chegar ao máximo ou mínimo, o botão correspondente não deverá mais atuar no giro.
 * D. Se chegou no máximo/mínimo, devera ficar assim ate que o botão "oposto" seja pressionado.
 */

#include <Servo.h> // Inclui a biblioteca do Servo

Servo meuservo;     // Cria um objeto Servo para controlar o servo motor
int servoPin = 9;   // Define o pino do servo motor
int BT1 = 2;        // Define o pino do botão BT1
int BT2 = 3;        // Define o pino do botão BT2
int pos = 0;        // Variável para armazenar a posição do servo motor
int intervalo = 15; // Define o intervalo de tempo entre cada grau do servo motor

void setup()
{
  meuservo.attach(servoPin);  // Anexa o servo motor ao pino 9
  meuservo.write(pos);        // Define a posição do servo motor como 0 graus
  pinMode(BT1, INPUT_PULLUP); // Define o pino do botão BT1 como entrada com resistor de pull-up interno
  pinMode(BT2, INPUT_PULLUP); // Define o pino do botão BT2 como entrada com resistor de pull-up interno
}

void loop()
{
  if ((digitalRead(BT1) == LOW) && pos < 180) // Se o botão BT1 for pressionado e a posição do servo motor for menor que 180 graus
  {
    for (pos = 0; pos <= 180; pos++) // Gira o servo motor de 0 a 180 graus
    {
      meuservo.write(pos);
      delay(intervalo);
    }
  }

  if ((digitalRead(BT2) == LOW) && pos > 0) // Se o botão BT2 for pressionado e a posição do servo motor for maior que 0 graus
  {
    for (pos = 180; pos >= 0; pos--) // Gira o servo motor de 180 a 0 graus
    {
      meuservo.write(pos);
      delay(intervalo);
    }
  }
}
