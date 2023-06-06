/**
 * @file Aula Prática 3.ino
 * @brief Aula Prática 3 - LCD 16x2
 * @version 1.0
 * @date 2023-06-04
 *
 * A. O roteiro envolve o desenvolvimento de uma lógica para controlar o brilho de um LED utilizando um display LCD 16x2.
 * B. O display deve mostrar informações sobre o estado atual do LED e os botões devem ser usados para aumentar ou diminuir o brilho.
 * C. O programa deve ter níveis de brilho pré-definidos e intervalos de tempo para mudança de brilho.
 * D. Quando o brilho atingir o máximo ou mínimo, o botão correspondente não deve mais alterar o brilho.
 */

#include <LiquidCrystal.h>

int segundos = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
  lcd.begin(16, 2); // Definir o número de colunas e linhas do LCD

  // Imprimir uma mensagem no LCD
  lcd.print("Olá Mundo!");
}

void loop()
{
  // Definir o cursor na coluna 0, linha 1
  // (nota: a linha 1 é a segunda linha, pois a contagem começa em 0):
  lcd.setCursor(0, 1);
  // imprimir o número de segundos desde o início do programa:
  lcd.print(segundos);
  delay(1000); // Esperar 1 segundo
  segundos += 1;
}