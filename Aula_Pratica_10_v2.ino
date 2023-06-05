// Aula Prática 10 - Teclado Matricial

#include <Adafruit_LiquidCrystal.h>
#include <Keypad.h>

// Inicializa o display com o endereço I2C do LCD
Adafruit_LiquidCrystal lcd(0);

const byte numLinhas = 4;  // número de linhas do teclado
const byte numColunas = 4; // número de colunas do teclado

// MapaTeclas define a tecla pressionada de acordo com a linha e a coluna conforme pressionado no teclado
char MapaTeclas[numLinhas][numColunas] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Código que mostra as conexões do teclado ao arduino
byte PinosLinhas[numLinhas] = {7, 6, 5, 4};   // Linhas 0 a 3
byte PinosColunas[numColunas] = {3, 2, 1, 0}; // Colunas 0 a 3

// Inicializa a programação do teclado
Keypad MeuTeclado = Keypad(makeKeymap(MapaTeclas), PinosLinhas, PinosColunas, numLinhas, numColunas);

void setup()
{
    lcd.begin(16, 2); // Inicializa o display com 16 colunas e 2 linhas
}

void loop()
{
    char TeclaPressionada = MeuTeclado.getKey();
    if (TeclaPressionada != NO_KEY)
    {
        lcd.print(TeclaPressionada); // Mostra a tecla pressionada no display
    }
}