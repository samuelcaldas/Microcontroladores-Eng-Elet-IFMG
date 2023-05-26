// Aula Prática 5 - LCD 16x2

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