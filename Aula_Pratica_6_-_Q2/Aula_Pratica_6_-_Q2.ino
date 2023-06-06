/**
 * @file Aula Prática 6 - Programa 2.ino
 * @brief Aula Prática 6 - Servo motor controlado por potenciômetro
 * @version 1.0
 * @date 2023-06-04
 *
 * A. O roteiro envolve o desenvolvimento de uma lógica em que um potenciômetro controla o giro de um servo motor.
 */

#include <Servo.h> // Inclui a biblioteca do Servo

Servo meuservo;     // Cria um objeto Servo para controlar o servo motor
int servoPin = 9;   // Define o pino do servo motor
int potpin = A0;    // Define o pino do potenciômetro
int val = 0;        // Variável para armazenar o valor lido do potenciômetro
int intervalo = 15; // Define o intervalo de tempo entre cada grau do servo motor

void setup()
{
  meuservo.attach(servoPin); // Anexa o servo motor ao pino 9
  meuservo.write(val);       // Define a posição do servo motor como 0 graus
}

void loop()
{
  val = analogRead(potpin);        // Lê o valor do potenciômetro
  val = map(val, 0, 1023, 0, 180); // Mapeia o valor para o intervalo de 0 a 180 graus
  meuservo.write(val);             // Define a posição do servo motor com base no valor mapeado
  delay(intervalo);                // Espera por 15 ms
}
