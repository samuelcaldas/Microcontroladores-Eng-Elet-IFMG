/**
 * @file Aula Prática 11.ino
 * @brief Aula Prática 11 - Medição de distâncias através de ultrassom com o sensor HC-SR04
 * @version 1.0
 * @date 2023-06-03
 *
 * Este algoritmo mede a distância usando o sensor HC-SR04 e exibe o resultado no monitor serial.
 */

#include <Ultrasonic.h>

class Sensor
{
private:
    Ultrasonic ultrasonic;

public:
    Sensor(int trigPin, int echPin) : ultrasonic(trigPin, echPin) {}
    int measureDistance()
    {
        int distance = ultrasonic.read();
        return distance;
    }
};

class Manager
{
private:
    Sensor sensor;

public:
    Manager(Sensor s) : sensor(s) {}
    void update()
    {
        int distance = sensor.measureDistance();
        Serial.print("Distancia: ");
        Serial.print(distance);
        Serial.println(" cm");
    }
};

Manager manager(Sensor(6, 7));

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    manager.update();
    delay(500);
}

//  Em conclusão, esta aula prática nos permitiu aplicar o conhecimento teórico adquirido durante o curso de Microcontroladores em relação à programação e montagem de hardware na plataforma Arduino. Conseguimos medir com sucesso as distâncias usando um sensor ultrassônico HC-SR04. Embora tenhamos usado a biblioteca externa Ultrasonic.h para facilitar o uso do sensor, também seria possível modificar o código para usar o sensor sem a necessidade desta biblioteca. Além disso, aplicamos os princípios SOLID para criar um código legível e fácil de manter. Esta experiência prática aprofundou nosso entendimento sobre microcontroladores e suas aplicações.