// Define constants for pin numbers
const int BOMBA_1_PIN = 2;
const int BOMBA_2_PIN = 3;
const int LED_VERDE_PIN = 4;
const int LED_AMARELO_PIN = 5;
const int CHAVE_SELETORA_PIN = 6;
const int BOTAO_1_PIN = 7;
const int BOTAO_2_PIN = 8;
const int SENSOR_ANALOGICO_PIN = A0;
const int SENSOR_DIGITAL_1_PIN = 9;
const int SENSOR_DIGITAL_2_PIN = 10;

// Define constants for water level thresholds
const int NIVEL_BAIXO = 25;
const int NIVEL_MEDIO = 50;
const int NIVEL_ALTO = 75;

// Define constants for debounce time
const unsigned long DEBOUNCE_DELAY = 50;

// Define a class to represent the reservoir
class Reservatorio
{
public:
  // Constructor
  Reservatorio(int nivel)
  {
    this->nivel = nivel;
  }

  // Method to set the water level
  void setNivel(int nivel)
  {
    this->nivel = nivel;
  }

  // Method to get the water level
  int getNivel()
  {
    return this->nivel;
  }

private:
  // Water level
  int nivel;
};

// Define a class to represent the pump
class Bomba
{
public:
  // Constructor
  Bomba(int pin)
  {
    this->pin = pin;
    pinMode(pin, OUTPUT);
    desligar();
  }

  // Method to turn on the pump
  void ligar()
  {
    digitalWrite(pin, HIGH);
  }

  // Method to turn off the pump
  void desligar()
  {
    digitalWrite(pin, LOW);
  }

private:
  // Pin number
  int pin;
};

// Define a class to represent the LED
class LED
{
public:
  // Constructor
  LED(int pin)
  {
    this->pin = pin;
    pinMode(pin, OUTPUT);
    desligar();
  }

  // Method to turn on the LED
  void ligar()
  {
    digitalWrite(pin, HIGH);
  }

  // Method to turn off the LED
  void desligar()
  {
    digitalWrite(pin, LOW);
  }

private:
  // Pin number
  int pin;
};

// Define a class to represent a button with debounce functionality
class Botao
{
public:
  // Constructor
  Botao(int pin)
  {
    this->pin = pin;
    pinMode(pin, INPUT_PULLUP);

    estadoBotao = digitalRead(pin);
    ultimoEstadoBotao = estadoBotao;
    ultimoTempoMudancaEstado = 0;
  }

  // Method to update the button state with debounce
  void atualizar()
  {
    int leitura = digitalRead(pin);

    if (leitura != ultimoEstadoBotao)
    {
      ultimoTempoMudancaEstado = millis();
    }

    if ((millis() - ultimoTempoMudancaEstado) > DEBOUNCE_DELAY)
    {
      if (leitura != estadoBotao)
      {
        estadoBotao = leitura;

        if (estadoBotao == LOW)
        {
          pressionado();
        }
      }
    }

    ultimoEstadoBotao = leitura;
  }

  // Method to check if the button is pressed
  bool estaPressionado()
  {
    return (estadoBotao == LOW);
  }

protected:
  virtual void pressionado() {}

private:
  // Pin number
  int pin;

  // Button state variables for debounce functionality
  int estadoBotao;
  int ultimoEstadoBotao;
  unsigned long ultimoTempoMudancaEstado;
};

// Define a class to represent the mode selector switch with debounce functionality
class ChaveSeletora : public Botao
{
public:
  ChaveSeletora(int pin) : Botao(pin) {}

  bool estaEmModoAutomatico() { return modoAutomatico; }

protected:
  virtual void pressionado() { modoAutomatico = !modoAutomatico; }

private:
  bool modoAutomatico = true;
};

// Define a class to represent a digital water level sensor
class SensorNivelDigital
{
public:
  SensorNivelDigital(int pin1, int pin2) : pin1(pin1), pin2(pin2)
  {
    pinMode(pin1, INPUT_PULLUP);
    pinMode(pin2, INPUT_PULLUP);
  }

  int lerNivel()
  {
    bool nivelBaixoAcionado = digitalRead(pin1) == LOW;
    bool nivelAltoAcionado = digitalRead(pin2) == LOW;

    if (nivelBaixoAcionado && nivelAltoAcionado)
      return NIVEL_ALTO;
    else if (nivelBaixoAcionado)
      return NIVEL_MEDIO;
    else
      return NIVEL_BAIXO;
  }

private:
  const int pin1;
  const int pin2;
};

// Define a class to represent an analog water level sensor
class SensorNivelAnalogico
{
public:
  SensorNivelAnalogico(int pin) : pin(pin) {}

  int lerNivel()
  {
    return map(analogRead(pin), 0, 1023, NIVEL_BAIXO, NIVEL_ALTO);
  }

private:
  const int pin;
};

// Define a class to represent the control system using digital water level sensors
class SistemaDeControleDigital
{
public:
  SistemaDeControleDigital(Reservatorio *reservatorio,
                           Bomba *bomba1,
                           Bomba *bomba2,
                           LED *ledVerde,
                           LED *ledAmarelo,
                           ChaveSeletora *chaveSeletora,
                           Botao *botao1,
                           Botao *botao2,
                           SensorNivelDigital *sensorNivel)
      : reservatorio(reservatorio),
        bomba1(bomba1),
        bomba2(bomba2),
        ledVerde(ledVerde),
        ledAmarelo(ledAmarelo),
        chaveSeletora(chaveSeletora),
        botao1(botao1),
        botao2(botao2),
        sensorNivel(sensorNivel) {}

  void atualizar()
  {
    chaveSeletora->atualizar();
    botao1->atualizar();
    botao2->atualizar();

    if (chaveSeletora->estaEmModoAutomatico())
    {
      reservatorio->setNivel(sensorNivel->lerNivel());

      if (reservatorio->getNivel() < NIVEL_BAIXO)
      {
        bomba1->ligar();
        bomba2->ligar();
      }
      else if (reservatorio->getNivel() < NIVEL_MEDIO)
      {
        bomba1->ligar();
        bomba2->desligar();
      }
      else
      {
        bomba1->desligar();
        bomba2->desligar();
      }

      ledVerde->ligar();
    }
    else
    {

      if (botao1->estaPressionado())
      {
        bomba1->ligar();
      }
      else
      {
        bomba1->desligar();
      }

      if (botao2->estaPressionado())
      {
        bomba2->ligar();
      }
      else
      {
        bomba2->desligar();
      }

      ledVerde->desligar();
    }
  }

private:
  Reservatorio *reservatorio;

  Bomba *bomba1;

  Bomba *bomba2;

  LED *ledVerde;

  LED *ledAmarelo;

  ChaveSeletora *chaveSeletora;

  Botao *botao1;

  Botao *botao2;

  SensorNivelDigital *sensorNivel;
};

// Define a class to represent the control system using an analog water level sensor

class SistemaDeControleAnalogico
{

public:
  SistemaDeControleAnalogico(Reservatorio *reservatorio,

                             Bomba *bomba1,

                             Bomba *bomba2,

                             LED *ledVerde,

                             LED *ledAmarelo,

                             ChaveSeletora *chaveSeletora,

                             Botao *botao1,

                             Botao *botao2,

                             SensorNivelAnalogico *sensorNivel)

      : reservatorio(reservatorio),

        bomba1(bomba1),

        bomba2(bomba2),

        ledVerde(ledVerde),

        ledAmarelo(ledAmarelo),

        chaveSeletora(chaveSeletora),

        botao1(botao2),

        botao2(botao2),

        sensorNivel(sensorNivel)
  {
  }

  void atualizar()
  {

    chaveSeletora->atualizar();

    botao1->atualizar();

    botao2->atualizar();

    if (chaveSeletora->estaEmModoAutomatico())
    {

      reservatorio->setNivel(sensorNivel->lerNivel());

      if (reservatorio->getNivel() < NIVEL_BAIXO)
      {

        bomba1->ligar();

        bomba2->ligar();
      }
      else if (reservatorio->getNivel() < NIVEL_MEDIO)
      {

        bomba1->ligar();

        bomba2->desligar();
      }
      else
      {

        bomba1->desligar();

        bomba2->desligar();
      }

      ledVerde->ligar();
    }
    else
    {

      if (botao1->estaPressionado())
      {

        bomba1->ligar();
      }
      else
      {

        bomba1->desligar();
      }

      if (botao2->estaPressionado())
      {

        bomba2->ligar();
      }
      else
      {

        bomba2->desligar();
      }

      ledVerde->desligar();
    }
  }

private:
  Reservatorio *reservatorio;

  Bomba *bomba1;

  Bomba *bomba2;

  LED *ledVerde;

  LED *ledAmarelo;

  ChaveSeletora *chaveSeletora;

  Botao *botao1;

  Botao *botao2;

  SensorNivelAnalogico *sensorNivel;
};

// Create objects for reservoir, pumps and LEDs

Reservatorio reservatorio(0);

Bomba bomba1(BOMBA_1_PIN);

Bomba bomba2(BOMBA_2_PIN);

LED ledVerde(LED_VERDE_PIN);

LED ledAmarelo(LED_AMARELO_PIN);

// Create objects for buttons and mode selector switch

ChaveSeletora chaveSeletora(CHAVE_SELETORA_PIN);

Botao botao1(BOTAO_1_PIN);

Botao botao2(BOTAO_2_PIN);

// Create objects for water level sensors

SensorNivelDigital sensorNivelDigital(SENSOR_DIGITAL_1_PIN, SENSOR_DIGITAL_2_PIN);

SensorNivelAnalogico sensorNivelAnalogico(SENSOR_ANALOGICO_PIN);

// Create object for control system using digital water level sensors

SistemaDeControleDigital sistemaDeControleDigital(&reservatorio,

                                                  &bomba1,

                                                  &bomba2,

                                                  &ledVerde,

                                                  &ledAmarelo,

                                                  &chaveSeletora,

                                                  &botao1,

                                                  &botao2,

                                                  &sensorNivelDigital);

// Create object for control system using an analog water level sensor

SistemaDeControleAnalogico sistemaDeControleAnalogico(&reservatorio,

                                                      &bomba1,

                                                      &bomba2,

                                                      &ledVerde,

                                                      &ledAmarelo,

                                                      &chaveSeletora,

                                                      &botao1,

                                                      &botao2,

                                                      &sensorNivelAnalogico);

void setup() {}

void loop()
{

  // Uncomment one of the following lines to use either digital or analog water level sensors

  // sistemaDeControleDigital.atualizar();

  // sistemaDeControleAnalogico.atualizar();

  // Use millis() instead of delay()

  unsigned long currentMillis = millis();

  while (millis() - currentMillis < 100)
  {
  }
}