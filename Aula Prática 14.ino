/**
 * @file Aula Prática 14.ino
 * @brief Aula Prática - CONTROLE DIGITAL E ANALÓGICO DE NÍVEL DE RESERVATÓRIO
 * @version 1.0
 * @date 2023-06-03
 *
 * A. O controle de nível do reservatório deve ser realizado da seguinte forma:
 *    Quando o nível estiver em 50% a Bomba 1 deverá ligar até que o reservatório encha completamente.
 *    Se a Bomba 1 não conseguir manter o nível no reservatório, existe uma bomba de segurança (Bomba 2),
 *    que deve ser ligada apenas quando o nível estiver muito baixo (abaixo de 25%) e ela deve ser desligada
 *    somente quando o nível de água atingir 75%.
 * B. Deverá haver uma chave seletora, com retenção, para definir o modo Automático/Manual.
 *    Em Automático funcionará conforme descrito acima e, em modo Manual,
 *    dois botões pulsantes farão o controle de ligarem as bombas, ignorando os sensores de nível,
 *    ou seja, enquanto o botão 1 estiver pressionado a Bomba 1 deverá ficar ligada e enquanto o Botão 2 estiver pressionado a Bomba 2 deverá ficar ligada.
 * C. Um LED verde deverá ser usado para sinalização do modo Automático (aceso) / Manual (apagado).
 * D. Utilize botões/chaves com retenção para simular o acionamento das chaves boia e LEDs vermelhos para as bombas.
 *    Lembrando que quando o nível estiver em 100% todos as chaves deverão estar acionadas.
 * E. Se a chave do nível superior for acionada sem que a chave do nível inferior esteja acionada um LED amarelo deverá acender indicando o estado de erro.
 * F. Após concluída a primeira etapa, faça uma modificação no controle de nível substituindo os sensores digitais por um sensor analógico.
 *    Utilize um potenciômetro para simular o funcionamento do sensor analógico de nível.
 */

// Define constants for pin numbers
const int BOMBA_1_PIN = 2;
const int LED_BOMBA_1_PIN = 9;
const int BOTAO_BOMBA_1_PIN = 7;
const int BOMBA_2_PIN = 3;
const int LED_BOMBA_2_PIN = 10;
const int BOTAO_BOMBA_2_PIN = 8;
const int LED_MODO_AUTOMATICO_PIN = 4;
const int CHAVE_SELETORA_PIN = 6;
const int SENSOR_ANALOGICO_PIN = A0;
const int LED_ERRO_SENSOR_PIN = 11;

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

// Define a class to represent the pump
class Bomba
{
public:
  // Constructor
  Bomba(int pin, LED *led) : pin(pin), led(led)
  {
    pinMode(pin, OUTPUT);
    desligar();
  }

  // Method to turn on the pump
  void ligar()
  {
    digitalWrite(pin, HIGH);
    led->ligar();
  }

  // Method to turn off the pump
  void desligar()
  {
    digitalWrite(pin, LOW);
    led->desligar();
  }

private:
  // Pin number
  int pin;
  LED *led;
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

  // Method to check if the button is pressed
  bool estaPressionado()
  {
    atualizar();
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
};

// Define a class to represent the mode selector switch with debounce functionality
class ChaveSeletora : public Botao
{
public:
  ChaveSeletora(int pin) : Botao(pin) {}

  bool estaLigado()
  {
    atualizar();
    return estado;
  }

protected:
  virtual void pressionado() { estado = !estado; }

private:
  bool estado = true;
};

// Define an interface for water level sensors
class SensorNivel
{
public:
  virtual int lerNivel() = 0;
};

// Define a class to represent a digital water level sensor
class SensorNivelDigital : public SensorNivel
{
public:
  SensorNivelDigital(const int *pPinos, int numPinos, LED *ledErroSensor)
      : pPinos(pPinos), numPinos(numPinos), ledErroSensor(ledErroSensor)
  {
    for (int i = 0; i < numPinos; i++)
    {
      pinMode(pPinos[i], INPUT_PULLUP);
    }
  }

  virtual int lerNivel()
  {
    if (chavesInvalidas())
    {
      ledErroSensor->ligar();
      return 100;
    }
    else
    {
      ledErroSensor->desligar();
      return lerChaves();
    }
  }

private:
  const int *pPinos;
  const int numPinos;
  LED *ledErroSensor;

  bool chavesInvalidas()
  {
    for (int i = numPinos - 1; i >= 1; i--)
    {
      if (digitalRead(pPinos[i]) == LOW && digitalRead(pPinos[i - 1]) == HIGH)
      {
        return true;
      }
    }
    return false;
  }

  int lerChaves()
  {
    for (int i = 0; i < numPinos; i++)
    {
      if (digitalRead(pPinos[i]) == LOW)
      {
        return map(i + 1, 1, numPinos, 0, 100);
      }
    }

    return 0;
  }
};

// Define a class to represent an analog water level sensor
class SensorNivelAnalogico : public SensorNivel
{
public:
  SensorNivelAnalogico(int pin) : pin(pin) {}

  virtual int lerNivel()
  {
    return map(analogRead(pin), 0, 1023, 0, 100);
  }

private:
  const int pin;
};

// Define a class to represent the control system
class SistemaDeControle
{
public:
  SistemaDeControle(Reservatorio *reservatorio,
                    Bomba *bomba1,
                    Bomba *bomba2,
                    LED *ledModoAutomatico,
                    ChaveSeletora *chaveSeletora,
                    Botao *botaoBomba1,
                    Botao *botaoBomba2,
                    SensorNivel *sensorNivel)
      : reservatorio(reservatorio),
        bomba1(bomba1),
        bomba2(bomba2),
        ledModoAutomatico(ledModoAutomatico),
        chaveSeletora(chaveSeletora),
        botaoBomba1(botaoBomba1),
        botaoBomba2(botaoBomba2),
        sensorNivel(sensorNivel) {}

  void atualizar()
  {
    if (chaveSeletora->estaLigado())
    {
      modoAutomatico();
    }
    else
    {
      modoManual();
    }
  }

private:
  Reservatorio *reservatorio;
  Bomba *bomba1;
  Bomba *bomba2;
  LED *ledModoAutomatico;
  ChaveSeletora *chaveSeletora;
  Botao *botaoBomba1;
  Botao *botaoBomba2;
  SensorNivel *sensorNivel;

  void modoAutomatico()
  {
    reservatorio->setNivel(sensorNivel->lerNivel());

    if (reservatorio->getNivel() < NIVEL_BAIXO)
    {
      // Water level is very low - turn on both pumps
      bomba1->ligar();
      bomba2->ligar();
    }
    else if (reservatorio->getNivel() < NIVEL_MEDIO)
    {
      // Water level is below 50% - turn on pump 1 and turn off pump 2
      bomba1->ligar();
      bomba2->desligar();
    }
    else if (reservatorio->getNivel() < NIVEL_ALTO)
    {
      // Water level is between 50% and 75% - turn off both pumps
      bomba1->desligar();
      bomba2->desligar();
    }
    else
    {
      // Water level is above 75% - turn off both pumps
      bomba1->desligar();
      bomba2->desligar();
    }

    ledModoAutomatico->ligar();
  }

  void modoManual()
  {
    // Check if button 1 is pressed
    if (botaoBomba1->estaPressionado())
    {
      bomba1->ligar();
    }
    else
    {
      bomba1->desligar();
    }

    // Check if button 2 is pressed
    if (botaoBomba2->estaPressionado())
    {
      bomba2->ligar();
    }
    else
    {
      bomba2->desligar();
    }

    ledModoAutomatico->desligar();
  }
};

// Create objects for reservoir, pumps and LEDs
Reservatorio reservatorio(0);
LED ledBomba1(LED_BOMBA_1_PIN);
LED ledBomba2(LED_BOMBA_2_PIN);
Bomba bomba1(BOMBA_1_PIN, &ledBomba1);
Bomba bomba2(BOMBA_2_PIN, &ledBomba2);
LED ledModoAutomatico(LED_MODO_AUTOMATICO_PIN);
LED ledErroSensor(LED_ERRO_SENSOR_PIN);

// Create objects for buttons and mode selector switch
ChaveSeletora chaveSeletora(CHAVE_SELETORA_PIN);
Botao botaoBomba1(BOTAO_BOMBA_1_PIN);
Botao botaoBomba2(BOTAO_BOMBA_2_PIN);

// Create objects for water level sensors
const int pinoSensores[] = {12, 13, 14, 15};
const int numSensores = sizeof(pinoSensores) / sizeof(pinoSensores[0]);
SensorNivelDigital sensorNivel(pinoSensores, numSensores, &ledErroSensor);
// SensorNivelAnalogico sensorNivel(SENSOR_ANALOGICO_PIN);

// Create object for control system
SistemaDeControle sistemaDeControle(&reservatorio,
                                    &bomba1,
                                    &bomba2,
                                    &ledModoAutomatico,
                                    &chaveSeletora,
                                    &botaoBomba1,
                                    &botaoBomba2,
                                    &sensorNivel);

void setup() {}

void loop()
{
  sistemaDeControle.atualizar();

  // Use millis() instead of delay()
  unsigned long currentMillis = millis();
  while (millis() - currentMillis < 100)
  {
  }
}