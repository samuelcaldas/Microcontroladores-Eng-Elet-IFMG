// Include the SoftwareSerial library
#include "SoftwareSerial.h"
// Create a new software serial
SoftwareSerial bluetooth(2, 3); // RX, TX
                                // define o nome do dispositivo bluetooth
                                //  e o número das portas Rx do arduino (conectado ao pino TX do módulo HC05)
                                //  e Tx do arduino (conectado ao pino RX do módulo HC05) e

// Define constants for pins and commands
const int LED_PIN = 13;       // número do pino onde o LED interno do Arduino está conectado
const int BUTTON_PIN = 4;     // número do pino onde o botão está conectado
const int PWM_PIN = 5;        // número do pino onde o LED PWM está conectado
const int POT_PIN = A0;       // número do pino onde o potenciômetro está conectado
const char LED_ON = 'H';      // comando para ligar o LED
const char LED_OFF = 'L';     // comando para desligar o LED
const char BUTTON_READ = 'B'; // comando para ler o estado do botão
const char PWM_WRITE = 'P';   // comando para ajustar o brilho do LED PWM
const char POT_READ = 'A';    // comando para ler o valor do potenciômetro

// Define variables for states and values
int incomingByte; // variável de armazenamento do dado recebido no serial bluetooth
int buttonState;  // variável de armazenamento do estado do botão
int pwmValue;     // variável de armazenamento do valor do PWM
int potValue;     // variável de armazenamento do valor do potenciômetro

void setup()
{
  bluetooth.begin(9600);      // definindo a velocidade de inicialização do bluetooth
  pinMode(LED_PIN, OUTPUT);   // definição do pino (13) do LED como saída
  pinMode(BUTTON_PIN, INPUT); // definição do pino (4) do botão como entrada
  pinMode(PWM_PIN, OUTPUT);   // definição do pino (5) do LED PWM como saída
  pinMode(POT_PIN, INPUT);    // definição do pino (A0) do potenciômetro como entrada
}

void loop()
{
  if (bluetooth.available() > 0) // verificar se há dados na entrada serial bluetooth
  {
    incomingByte = bluetooth.read(); // lê o último byte no armazenamento serial
    switch (incomingByte)
    {
    case LED_ON: // se é um caractere H (ASCII 72), liga o LED
      digitalWrite(LED_PIN, HIGH);
      bluetooth.println("LED: ON");
      break;
    case LED_OFF: // se é um caractere L (ASCII 76), desliga LED
      digitalWrite(LED_PIN, LOW);
      bluetooth.println("LED: OFF");
      break;
    case BUTTON_READ: // se é um caractere B (ASCII 66), lê o estado do botão
      buttonState = digitalRead(BUTTON_PIN);
      bluetooth.print("Button: ");
      bluetooth.println(buttonState);
      break;
    case PWM_WRITE:                    // se é um caractere P (ASCII 80), ajusta o brilho do LED PWM
      pwmValue = bluetooth.parseInt(); // lê um inteiro da entrada serial bluetooth
      analogWrite(PWM_PIN, pwmValue);  // escreve o valor no pino PWM
      bluetooth.print("PWM: ");
      bluetooth.println(pwmValue);
      break;
    case POT_READ:                    // se é um caractere A (ASCII 65), lê o valor do potenciômetro
      potValue = analogRead(POT_PIN); // lê o valor analógico no pino A0
      bluetooth.print("Potentiometer: ");
      bluetooth.println(potValue);
      break;
    default: // se é outro caractere, ignora e envia uma mensagem de erro
      bluetooth.println("Invalid command");
    }
  }
}
