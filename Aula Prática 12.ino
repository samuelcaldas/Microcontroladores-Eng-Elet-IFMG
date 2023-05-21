// Aula Prática 12
// Um sensor (S1) mede o nível de líquido em um reservatório e retorna uma faixa de valores analógicos que deve ser dividida em 4 níveis.
// O algoritmo deve controlar a ativação de três bombas (B1, B2 e B3) de acordo com a seguinte tabela:
//  Nível > 90%: B1 Desligada, B2 Desligada, B3 Desligada;
//  60% < Nível <= 90%: B1 Ligada, B2 Desligada, B3 Desligada;
//  20% < Nível <= 60%: B1 Ligada, B2 Ligada, B3 Desligada;
//  Nível <= 20%: B1 Ligada, B2 Ligada, B3 Ligada.
// Um potenciômetro deve ser usado para simular o funcionamento do sensor de nível S1 e três LEDs para simular cada bomba respectivamente.

class WaterLevelSensor
{
private:
  int sensorPin;

public:
  WaterLevelSensor(int pin)
  {
    sensorPin = pin;
  }
  float getNormalizedValue()
  {
    int sensorValue = analogRead(sensorPin);
    return map(sensorValue, 0, 1023, 0, 100);
  }
};

class BombController
{
private:
  int bombPin;

public:
  BombController(int pin)
  {
    bombPin = pin;
    pinMode(bombPin, OUTPUT);
  }
  void turnOn()
  {
    digitalWrite(bombPin, HIGH);
  }
  void turnOff()
  {
    digitalWrite(bombPin, LOW);
  }
};

class PumpController
{
public:
  virtual void controlPumps(float waterLevel) = 0;
};

class PumpControllerLevel1 : public PumpController
{
private:
  BombController *b1;
  BombController *b2;
  BombController *b3;

public:
  PumpControllerLevel1(BombController *bomb1, BombController *bomb2, BombController *bomb3)
  {
    b1 = bomb1;
    b2 = bomb2;
    b3 = bomb3;
  }
  void controlPumps(float waterLevel)
  {
    if (waterLevel > 90)
    {
      b1->turnOff();
      b2->turnOff();
      b3->turnOff();
    }
  }
};

class PumpControllerLevel2 : public PumpController
{
private:
  BombController *b1;
  BombController *b2;
  BombController *b3;

public:
  PumpControllerLevel2(BombController *bomb1, BombController *bomb2, BombController *bomb3)
  {
    b1 = bomb1;
    b2 = bomb2;
    b3 = bomb3;
  }
  void controlPumps(float waterLevel)
  {
    if (waterLevel > 60 && waterLevel <= 90)
    {
      b1->turnOn();
      b2->turnOff();
      b3->turnOff();
    }
  }
};

class PumpControllerLevel3 : public PumpController
{
private:
  BombController *b1;
  BombController *b2;
  BombController *b3;

public:
  PumpControllerLevel3(BombController *bomb1, BombController *bomb2, BombController *bomb3)
  {
    b1 = bomb1;
    b2 = bomb2;
    b3 = bomb3;
  }
  void controlPumps(float waterLevel)
  {
    if (waterLevel > 20 && waterLevel <= 60)
    {
      b1->turnOn();
      b2->turnOn();
      b3->turnOff();
    }
  }
};

class PumpControllerLevel4 : public PumpController
{
private:
  BombController *b1;
  BombController *b2;
  BombController *b3;

public:
  PumpControllerLevel4(BombController *bomb1, BombController *bomb2, BombController *bomb3)
  {
    b1 = bomb1;
    b2 = bomb2;
    b3 = bomb3;
  }
  void controlPumps(float waterLevel)
  {
    if (waterLevel <= 20)
    {
      b1->turnOn();
      b2->turnOn();
      b3->turnOn();
    }
  }
};

class WaterLevelController
{
private:
  WaterLevelSensor *sensor;
  PumpController *pumpControllers[4];

public:
  WaterLevelController(WaterLevelSensor *waterLevelSensor, PumpController *pumpController1, PumpController *pumpController2, PumpController *pumpController3, PumpController *pumpController4)
  {
    sensor = waterLevelSensor;
    pumpControllers[0] = pumpController1;
    pumpControllers[1] = pumpController2;
    pumpControllers[2] = pumpController3;
    pumpControllers[3] = pumpController4;
  }
  void controlWaterLevel()
  {
    float waterLevel = sensor->getNormalizedValue();
    for (int i = 0; i < 4; i++)
    {
      pumpControllers[i]->controlPumps(waterLevel);
    }
  }
};

const int waterLevelSensorPin = A0;
const int bomb1Pin = 8;
const int bomb2Pin = 9;
const int bomb3Pin = 10;

WaterLevelSensor waterLevelSensor(waterLevelSensorPin);
BombController bomb1(bomb1Pin);
BombController bomb2(bomb2Pin);
BombController bomb3(bomb3Pin);

PumpControllerLevel1 pumpController1(&bomb1, &bomb2, &bomb3);
PumpControllerLevel2 pumpController2(&bomb1, &bomb2, &bomb3);
PumpControllerLevel3 pumpController3(&bomb1, &bomb2, &bomb3);
PumpControllerLevel4 pumpController4(&bomb1, &bomb2, &bomb3);

WaterLevelController waterLevelController(&waterLevelSensor, &pumpController1, &pumpController2, &pumpController3, &pumpController4);

void setup()
{
}

void loop()
{
  waterLevelController.controlWaterLevel();
}