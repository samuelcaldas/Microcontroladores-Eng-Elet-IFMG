/*
 * Este código implementa um sistema de controle de componentes eletrônicos
 * através de comandos enviados por um aplicativo Bluetooth.
 *
 * Os componentes controlados são:
 * - LED: pode ser ligado ou desligado
 * - LED PWM: pode ter seu brilho ajustado
 * - Potenciômetro: pode ter seu valor lido
 * - Botão: pode ter seu estado lido
 *
 * Os comandos enviados pelo aplicativo Bluetooth são:
 * - 'H': liga o LED
 * - 'L': desliga o LED e o LED PWM
 * - 'P': ajusta o brilho do LED PWM (deve ser seguido pelo valor do brilho)
 * - 'A': lê o valor do potenciômetro
 * - 'B': lê o estado do botão
 *
 * O código utiliza os seguintes conceitos e técnicas:
 * - Orientação a objetos: as classes LED, LED_PWM, Potentiometer e Button representam os componentes eletrônicos
 * - Polimorfismo: a classe abstrata Controller define uma interface comum para as classes controladoras
 * - Princípio Aberto-Fechado: as classes controladoras implementam suas próprias versões do método control sem modificar o código existente
 * - Comunicação serial: a classe SoftwareSerial é utilizada para ler dados enviados pelo aplicativo Bluetooth
 * - Conversão de tipos: a classe String é utilizada para converter uma string em um valor inteiro
 */

// Incluindo a biblioteca SoftwareSerial
#include "SoftwareSerial.h"

// Definindo constantes para os pinos
const unsigned int LED_PIN = 13;
const unsigned int LED_PWM_PIN = 5;
const unsigned int POTENTIOMETER_PIN = A0;
const unsigned int BUTTON_PIN = 4;
const unsigned int BLUETOOTH_RX = 2;
const unsigned int BLUETOOTH_TX = 3;

// Define constants for debounce time
const unsigned int DEBOUNCE_DELAY = 50;

class BluetoothData
{
public:
  BluetoothData(char command, int value) : command(command), value(value) {}

  char getCommand() const { return command; }
  int getValue() const { return value; }

private:
  char command;
  int value;
};

class BluetoothReader
{
public:
  BluetoothReader(SoftwareSerial &bluetooth) : bluetooth(bluetooth) {}

  BluetoothData readData()
  {
    // Lendo o primeiro caractere da serial do bluetooth para obter o comando
    char command = bluetooth.read();

    // Lendo os próximos caracteres da serial do bluetooth para obter o valor inteiro
    String valueStr = "";
    while (bluetooth.available())
    {
      char b = bluetooth.read();
      if (b == '\n' || b == '\r')
        break;
      valueStr += b;
    }

    // Convertendo a string lida para um valor inteiro
    int value = valueStr.toInt();

    return BluetoothData(command, value);
  }

private:
  SoftwareSerial &bluetooth;
};

class LED
{
public:
  LED(int pin) : pin(pin)
  {
    pinMode(pin, OUTPUT);
    desligar();
  }
  void ligar() { digitalWrite(pin, HIGH); }
  void desligar() { digitalWrite(pin, LOW); }

protected:
  int pin;
};

class LED_PWM : public LED
{
public:
  LED_PWM(int pin) : LED(pin) {}
  void ligar(int brightness) { analogWrite(pin, brightness); }
  void desligar() { analogWrite(pin, 0); }
};

class Potentiometer
{
public:
  Potentiometer(int pin) : pin(pin) {}
  int read() { return analogRead(pin); }

private:
  int pin;
};

class Button
{
public:
  Button(int pin, unsigned long debounceDelay) : pin(pin), debounceDelay(debounceDelay)
  {
    pinMode(pin, INPUT_PULLUP);

    estadoSwitch = digitalRead(pin);
    ultimoEstadoSwitch = estadoSwitch;
    ultimoTempoMudancaEstado = 0;
  }

  bool read()
  {
    atualizar();
    return (estadoSwitch == LOW);
  }

protected:
  void atualizar()
  {
    int leitura = digitalRead(pin);

    if (leitura != ultimoEstadoSwitch)
    {
      ultimoTempoMudancaEstado = millis();
    }

    if ((millis() - ultimoTempoMudancaEstado) > debounceDelay)
    {
      if (leitura != estadoSwitch)
      {
        estadoSwitch = leitura;
      }
    }

    ultimoEstadoSwitch = leitura;
  }

private:
  int pin;

  // Button state variables for debounce functionality
  unsigned long debounceDelay;
  int estadoSwitch;
  int ultimoEstadoSwitch;
  unsigned long ultimoTempoMudancaEstado;
};

class Controller
{
public:
  virtual String control(const BluetoothData &data) = 0;
};

class LED_Controller : public Controller
{
public:
  LED_Controller(LED &led) : led(led) {}

  String control(const BluetoothData &data) override
  {
    if (data.getCommand() == LED_ON)
    {
      led.ligar();
      return "LED ligado";
    }
    else if (data.getCommand() == LED_OFF)
    {
      led.desligar();
      return "LED desligado";
    }
    else
    {
      return "";
    }
  }

private:
  static const char LED_ON = 'H';
  static const char LED_OFF = 'L';

  LED &led;
};

class LED_PWM_Controller : public Controller
{
public:
  LED_PWM_Controller(LED_PWM &led_pwm) : led_pwm(led_pwm) {}

  String control(const BluetoothData &data) override
  {
    if (data.getCommand() == PWM_WRITE)
    {
      // Obtendo o valor do brilho enviado pelo app Bluetooth
      int brightness = data.getValue();

      // Limitando o valor do brilho entre 0 e 255
      brightness = max(0, min(255, brightness));

      // Ajustando o brilho do LED PWM
      led_pwm.ligar(brightness);

      return "LED PWM ligado com brilho " + String(brightness);
    }
    else if (data.getCommand() == LED_OFF)
    {
      led_pwm.desligar();
      return "LED PWM desligado";
    }
    else
    {
      return "";
    }
  }

private:
  static const char PWM_WRITE = 'P';
  static const char LED_OFF = 'L';

  LED_PWM &led_pwm;
};

class Potentiometer_Controller : public Controller
{
public:
  Potentiometer_Controller(Potentiometer &potentiometer) : potentiometer(potentiometer) {}

  String control(const BluetoothData &data) override
  {
    if (data.getCommand() == POT_READ)
      return "Valor do potenciômetro: " + String(potentiometer.read());
    else
      return "";
  }

private:
  static const char POT_READ = 'A';

  Potentiometer &potentiometer;
};

class Button_Controller : public Controller
{
public:
  Button_Controller(Button &button) : button(button) {}

  String control(const BluetoothData &data) override
  {
    if (data.getCommand() == BUTTON_READ)
      return button.read() ? "Botão pressionado" : "Botão não pressionado";
    else
      return "";
  }

private:
  static const char BUTTON_READ = 'B';

  Button &button;
};

class Control
{
public:
  Control(LED_Controller &led_controller, LED_PWM_Controller &led_pwm_controller, Potentiometer_Controller &potentiometer_controller, Button_Controller &button_controller, SoftwareSerial &bluetooth)
      : led_controller(led_controller), led_pwm_controller(led_pwm_controller), potentiometer_controller(potentiometer_controller), button_controller(button_controller), bluetooth(bluetooth)
  {
  }

  String run(const BluetoothData &data)
  {
    Controller *controllers[] = {&led_controller, &led_pwm_controller, &potentiometer_controller, &button_controller};
    int numControllers = sizeof(controllers) / sizeof(controllers[0]);
    for (int i = 0; i < numControllers; i++)
    {
      String result = controllers[i]->control(data);
      if (result != "")
      {
        bluetooth.println(result);
        break;
      }
    }
    return "";
  }

private:
  LED_Controller &led_controller;
  LED_PWM_Controller &led_pwm_controller;
  Potentiometer_Controller &potentiometer_controller;
  Button_Controller &button_controller;
  SoftwareSerial &bluetooth;
};

// Criando uma nova software de serial para o bluetooth
SoftwareSerial bluetooth(BLUETOOTH_RX, BLUETOOTH_TX);

// Criando objetos das classes LED, LED PWM, Potenciômetro e Botão
LED led(LED_PIN);
LED_PWM led_pwm(LED_PWM_PIN);
Potentiometer potentiometer(POTENTIOMETER_PIN);
Button button(BUTTON_PIN, DEBOUNCE_DELAY);

// Criando objetos das classes controladoras
LED_Controller led_controller(led);
LED_PWM_Controller led_pwm_controller(led_pwm);
Potentiometer_Controller potentiometer_controller(potentiometer);
Button_Controller button_controller(button);

// Criando um objeto da classe Control
Control control(led_controller, led_pwm_controller, potentiometer_controller, button_controller, bluetooth);

void setup()
{
  // Iniciando a serial do bluetooth com a velocidade de 9600 bps
  bluetooth.begin(9600);
}

void loop()
{
  if (bluetooth.available())
  {
    BluetoothReader reader(bluetooth);
    BluetoothData data = reader.readData();
    String result = control.run(data);
    if (result != "")
    {
      bluetooth.println(result);
    }
  }
}