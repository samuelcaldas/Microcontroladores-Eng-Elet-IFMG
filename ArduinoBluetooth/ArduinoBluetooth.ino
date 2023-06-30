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
 * - 'W': ajusta o brilho do LED PWM (deve ser seguido pelo valor do brilho)
 * - 'P': lê o valor do potenciômetro
 * - 'B': lê o estado do botão
 *
 * O código utiliza os seguintes conceitos e técnicas:
 * - Orientação a objetos: as classes LED, LED_PWM, Potentiometer e Button representam os componentes eletrônicos
 * - Polimorfismo: a classe abstrata Controller define uma interface comum para as classes controladoras
 * - Princípio Aberto-Fechado: as classes controladoras implementam suas próprias versões do método control sem modificar o código existente
 * - Comunicação serial: a classe SoftwareSerial é utilizada para ler dados enviados pelo aplicativo Bluetooth
 */

// Incluindo a biblioteca SoftwareSerial
#include "SoftwareSerial.h"

// Definindo constantes para os pinos
const unsigned int BLUETOOTH_RX = 2;
const unsigned int BLUETOOTH_TX = 3;
const unsigned int BUTTON_PIN = 4;
const unsigned int LED_PWM_PIN = 5;
const unsigned int LED_PIN = 13;
const unsigned int POTENTIOMETER_PIN = A0;

// Define constants for debounce time
const unsigned int DEBOUNCE_DELAY = 10;

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
    if (bluetooth.available() > 0)
    {
      if (bluetooth.peek() != -1)
      {
        // Lendo o primeiro caractere da serial do bluetooth para obter o comando
        char command = (char)bluetooth.read();

        // Verificando se o comando é uma letra maiúscula
        if (command < 'A' || command > 'Z')
          return BluetoothData('\0', 0);

        // Lendo os próximos caracteres da serial do bluetooth para obter o valor inteiro de 0 a 255
        int value = 0;
        if (bluetooth.available() > 0)
        {
          value = bluetooth.read();
        }

        // Verificando se o valor está entre 0 e 255
        if (value < 0 || value > 255)
        {
          return BluetoothData('\0', 0);
        }

        // Imprimindo o comando e o valor lidos no monitor serial
        Serial.print("Command: ");
        Serial.print(command);
        Serial.print(", Value: ");
        Serial.println(value);

        return BluetoothData(command, value);
      }
    }
    return BluetoothData('\0', 0);
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
  LED_PWM(int pin) : LED(pin)
  {
    desligar();
  }
  void ligar(int brightness) { analogWrite(pin, brightness); }
  void desligar() { analogWrite(pin, 0); }
};

class Potentiometer
{
public:
  Potentiometer(int pin, int numReadings) : pin(pin), numReadings(numReadings)
  {
    readings = new int[numReadings];
    for (int i = 0; i < numReadings; i++)
    {
      readings[i] = 0;
    }
  }

  int read()
  {
    total = total - readings[index];
    readings[index] = analogRead(pin);
    total = total + readings[index];
    index = (index + 1) % numReadings;
    int average = total / numReadings;
    return map(average, 0, 1023, 0, 255);
  }

private:
  int *readings;
  int total = 0;
  int index = 0;
  int pin;
  int numReadings;
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
  virtual void control(const BluetoothData &data) = 0;
  virtual BluetoothData control()
  {
    return BluetoothData('\0', 0);
  };
};

class ReceiverController : public Controller
{
public:
  BluetoothData control() override { return BluetoothData('\0', 0); }
};

class SenderController : public Controller
{
public:
  void control(const BluetoothData &data) override { return; }
};

class LED_Controller : public ReceiverController
{
public:
  LED_Controller(LED &led) : led(led) {}

  void control(const BluetoothData &data) override
  {
    if (data.getCommand() == LED_ON)
    {
      led.ligar();
      Serial.println("LED ligado");
    }
    else if (data.getCommand() == LED_OFF)
    {
      led.desligar();
      Serial.println("LED desligado");
    }
    else
    {
      return;
    }
  }

private:
  static const char LED_ON = 'H';
  static const char LED_OFF = 'L';

  LED &led;
};

class LED_PWM_Controller : public ReceiverController
{
public:
  LED_PWM_Controller(LED_PWM &led_pwm) : led_pwm(led_pwm) {}

  void control(const BluetoothData &data) override
  {
    if (data.getCommand() == PWM_WRITE)
    {
      // Obtendo o valor do brilho enviado pelo app Bluetooth
      int brightness = data.getValue();

      // Limitando o valor do brilho entre 0 e 255
      brightness = max(0, min(255, brightness));

      // Ajustando o brilho do LED PWM
      led_pwm.ligar(brightness);

      Serial.println("LED PWM ligado com brilho " + String(brightness));
    }
    else if (data.getCommand() == LED_OFF)
    {
      led_pwm.desligar();
      Serial.println("LED PWM desligado");
    }
    else
    {
      return;
    }
  }

private:
  static const char PWM_WRITE = 'W';
  static const char LED_OFF = 'L';

  LED_PWM &led_pwm;
};

class Potentiometer_Controller : public SenderController
{
public:
  Potentiometer_Controller(Potentiometer &potentiometer) : potentiometer(potentiometer) {}

  BluetoothData control() override
  {
    int potValue = potentiometer.read();
    return BluetoothData('P', potValue);
  }

private:
  Potentiometer &potentiometer;
};

class Button_Controller : public SenderController
{
public:
  Button_Controller(Button &button) : button(button) {}

  BluetoothData control() override
  {
    bool state = button.read();
    return BluetoothData('B', (int)state);
  }

private:
  Button &button;
};

class Control
{
public:
  Control(Controller *controllers[], int numControllers, SoftwareSerial &bluetooth) : controllers(controllers), numControllers(numControllers), bluetooth(bluetooth) {}

  virtual void run() = 0;

protected:
  Controller **controllers;
  int numControllers;
  SoftwareSerial &bluetooth;
};

class ControlSender : public Control
{
public:
  ControlSender(Controller *controllers[], int numControllers, SoftwareSerial &bluetooth) : Control(controllers, numControllers, bluetooth) {}

  void run() override
  {
    for (int i = 0; i < numControllers; i++)
    {
      BluetoothData result = controllers[i]->control();
      bluetooth.write(result.getCommand()); // Envia o comando
      bluetooth.write(result.getValue());   // Envia o valor
    }
  }
};

class ControlReceiver : public Control
{
public:
  ControlReceiver(Controller *controllers[], int numControllers, SoftwareSerial &bluetooth) : Control(controllers, numControllers, bluetooth) {}

  void run() override
  {
    if (bluetooth.available() > 0)
    {
      BluetoothReader reader(bluetooth);
      BluetoothData data = reader.readData();
      for (int i = 0; i < numControllers; i++)
      {
        controllers[i]->control(data);
      }
    }
  }
};

// Criando uma nova conexão de serial com o bluetooth
SoftwareSerial bluetooth(BLUETOOTH_RX, BLUETOOTH_TX);

// Criando objetos das classes LED, LED PWM, Potenciômetro e Botão
LED led(LED_PIN);
LED_PWM led_pwm(LED_PWM_PIN);
Potentiometer potentiometer(POTENTIOMETER_PIN, 20);
Button button(BUTTON_PIN, DEBOUNCE_DELAY);

// Criando objetos das classes controladoras
LED_Controller led_controller(led);
LED_PWM_Controller led_pwm_controller(led_pwm);
Potentiometer_Controller potentiometer_controller(potentiometer);
Button_Controller button_controller(button);

// Criando um array de ponteiros para as classes controladoras
ReceiverController *receiverControllers[] = {&led_controller, &led_pwm_controller};
int numReceiverControllers = sizeof(receiverControllers) / sizeof(receiverControllers[0]);

// Criando um array de ponteiros para as classes controladoras
SenderController *senderControllers[] = {&potentiometer_controller, &button_controller};
int numSenderControllers = sizeof(senderControllers) / sizeof(senderControllers[0]);

// Criando um objeto da classe Control
ControlSender controlSender(senderControllers, numSenderControllers, bluetooth);
ControlReceiver controlReceiver(receiverControllers, numReceiverControllers, bluetooth);

void setup()
{
  // Iniciando a serial do bluetooth com a velocidade de 9600 bps
  bluetooth.begin(9600);

  // Iniciando a serial do monitor serial
  Serial.begin(9600);
}

void loop()
{
  controlSender.run();
  controlReceiver.run();
  delay(100);
}