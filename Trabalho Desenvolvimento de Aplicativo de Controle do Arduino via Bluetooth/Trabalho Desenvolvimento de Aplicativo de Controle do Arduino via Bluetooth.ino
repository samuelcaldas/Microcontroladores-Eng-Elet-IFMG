// Include the SoftwareSerial library
#include "SoftwareSerial.h"
// Create a new software serial
SoftwareSerial bluetooth(2, 3); // RX, TX // define o nome do dispositivo bluetooth
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

class Led
{
public:
  Led(int pin)
  {
    _pin = pin;
    pinMode(_pin, OUTPUT);
  }
  virtual void on()
  {
    digitalWrite(_pin, HIGH);
  }
  virtual void off()
  {
    digitalWrite(_pin, LOW);
  }

private:
  int _pin;
};

class Pwm
{
public:
  Pwm(int pin)
  {
    _pin = pin;
    pinMode(_pin, OUTPUT);
  }
  void setLevel(int value)
  {
    analogWrite(_pin, value);
  }

private:
  int _pin;
};

class PwmLed : public Led, public Pwm
{
public:
  PwmLed(int pin) : Led(pin), Pwm(pin) {}
  void on(int value)
  {
    setLevel(value);
  }
};

void setup()
{
  bluetooth.begin(9600); // definindo a velocidade de inicialização do bluetooth
}

void loop()
{
  if (bluetooth.available() > 0) // verificar se há dados na entrada serial bluetooth
  {
    int incomingByte = bluetooth.read(); // lê o último byte no armazenamento serial
    switch (incomingByte)
    {
    case LED_ON: // se é um caractere H (ASCII 72), liga o LED
      Led led(LED_PIN);
      led.on();
      bluetooth.println("LED: ON");
      break;
    case LED_OFF: // se é um caractere L (ASCII 76), desliga LED
      Led led(LED_PIN);
      Led led(LED_PIN);
      led.off();
      bluetooth.println("LED: OFF");
      break;
    case BUTTON_READ: // se é um caractere B (ASCII 66), lê o estado do botão
      int buttonState = digitalRead(BUTTON_PIN);
      bluetooth.print("Button: ");
      bluetooth.println(buttonState);
      break;
    case PWM_WRITE:                        // se é um caractere P (ASCII 80), ajusta o brilho do LED PWM
      int pwmValue = bluetooth.parseInt(); // lê um inteiro da entrada serial bluetooth
      PwmLed pwmLed(PWM_PIN);
      pwmLed.on(pwmValue);
      bluetooth.print("PWM: ");
      bluetooth.println(pwmValue);
      break;
    case POT_READ:                        // se é um caractere A (ASCII 65), lê o valor do potenciômetro
      int potValue = analogRead(POT_PIN); // lê o valor analógico no pino A0
      bluetooth.print("Potentiometer: ");
      bluetooth.println(potValue);
      break;
    default: // se é outro caractere, ignora e envia uma mensagem de erro
      bluetooth.println("Invalid command");
    }
  }
}
