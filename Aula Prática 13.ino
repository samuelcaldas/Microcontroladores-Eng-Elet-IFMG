// Aula Prática 12 - CONTROLE DE MISTURA LEITE/ESSÊNCIA
// A. Ao pulsar o botão LIGA, os volumes dos tanques de Leite e de Essência são memorizados e a Válvula de Leite deverá ser aberta por 5 segundos;
// B. Em seguida, a Válvula de Essência deverá ser aberta até a mistura atingir a proporção de 1 parte de essência para 10 partes de leite.
//    Quando o tanque de Essência esvaziar até atingir a proporção adequada, a Válvula de Essência deverá ser fechada.
// C. O Motor deverá ser ligado por 10 segundos, em seguida desligado e a Válvula de Saída aberta.
// D. Após abrir a Válvula de Saída e o tanque esvaziar, ela deverá ser novamente fechada.
// E. O ciclo é reiniciado ao pulsar novamente o botão LIGA. Em qualquer momento que o botão DESLIGA for pulsado,
//    o Motor deverá ser desligado e as válvulas fechadas;
// F. Deverá existir uma sinalização exclusiva para controle ligado.
// Obs.: Considere que os tanques de Leite e Essência tenham capacidade de 10.000 litros e o de Mistura tenha capacidade de 20.000 litros.
//       A simulação dos níveis nos tanques de Leite e Essência serão simulados através de potenciômetros.
//       A simulação das saídas deverá ser feita através de LEDs.

/**
 * @brief Class representing a tank.
 */
class Tanque
{
public:
    /**
     * @brief Constructor for the Tanque class.
     * @param capacidadeMaxima The maximum capacity of the tank.
     */
    Tanque(int capacidadeMaxima)
    {
        this->capacidadeMaxima = capacidadeMaxima;
        this->volumeAtual = 0;
    }

    /**
     * @brief Sets the current volume of the tank.
     * @param volume The current volume of the tank.
     */
    void setVolumeAtual(int volume)
    {
        this->volumeAtual = volume;
    }

    /**
     * @brief Gets the current volume of the tank.
     * @return The current volume of the tank.
     */
    int getVolumeAtual()
    {
        return this->volumeAtual;
    }

    /**
     * @brief Gets the maximum capacity of the tank.
     * @return The maximum capacity of the tank.
     */
    int getCapacidadeMaxima()
    {
        return this->capacidadeMaxima;
    }

private:
    int capacidadeMaxima;
    int volumeAtual;
};

/**
 * @brief Class representing a valve.
 */
class Valvula
{
public:
    /**
     * @brief Constructor for the Valvula class.
     * @param pino The pin number of the valve.
     */
    Valvula(int pino)
    {
        this->pino = pino;
        pinMode(pino, OUTPUT);
    }

    /**
     * @brief Opens the valve.
     */
    void abrir()
    {
        digitalWrite(pino, HIGH);
    }

    /**
     * @brief Closes the valve.
     */
    void fechar()
    {
        digitalWrite(pino, LOW);
    }

private:
    int pino;
};

/**
 * @brief Class representing a motor.
 */
class Motor
{
public:
    /**
     * @brief Constructor for the Motor class.
     * @param pino The pin number of the motor.
     */
    Motor(int pino)
    {
        this->pino = pino;
        pinMode(pino, OUTPUT);
    }

    /**
     * @brief Turns on the motor.
     */
    void ligar()
    {
        digitalWrite(pino, HIGH);
    }

    /**
     * @brief Turns off the motor.
     */
    void desligar()
    {
        digitalWrite(pino, LOW);
    }

private:
    int pino;
};

class Nivel
{
public:
    Nivel(int pino, Tanque *tanque)
    {
        this->pino = pino;
        this->tanque = tanque;
    }

    /**
     * @brief Reads the value of the potentiometer and updates the tank volume
     */
    void atualizar()
    {
        int volume = map(analogRead(pino), 0, 1023, 0, tanque->getCapacidadeMaxima());
        tanque->setVolumeAtual(volume);
    }

private:
    int pino;
    Tanque *tanque;
};

class Sistema
{
public:
    Sistema(int pinoBotaoLiga, int pinoBotaoDesliga, int pinoLed)
    {
        this->pinoBotaoLiga = pinoBotaoLiga;
        this->pinoBotaoDesliga = pinoBotaoDesliga;
        this->pinoLed = pinoLed;

        pinMode(pinoBotaoLiga, INPUT_PULLUP);
        pinMode(pinoBotaoDesliga, INPUT_PULLUP);
        pinMode(pinoLed, OUTPUT);

        this->estado = false; // Initial state: off
        this->ultimoTempoBotaoLiga = 0;
        this->ultimoTempoBotaoDesliga = 0;

        atualizarLed();
    }

    /**
     * @brief Checks if the ON button has been pressed and turns on the system
     */
    void verificarBotaoLiga()
    {
        if (digitalRead(pinoBotaoLiga) == LOW && millis() - ultimoTempoBotaoLiga >= 200) // Debounce
        {
            estado = true; // Turns on the system
            atualizarLed();
            ultimoTempoBotaoLiga = millis();
        }
    }

    /**
     * @brief Checks if the OFF button has been pressed and turns off the system
     */
    void verificarBotaoDesliga()
    {
        if (digitalRead(pinoBotaoDesliga) == LOW && millis() - ultimoTempoBotaoDesliga >= 200) // Debounce
        {
            estado = false; // Turns off the system
            atualizarLed();
            ultimoTempoBotaoDesliga = millis();
        }
    }

    /**
     * @brief Returns the current state of the system (on or off)
     */
    bool getEstado()
    {
        return estado;
    }

private:
    int pinoBotaoLiga;
    int pinoBotaoDesliga;
    int pinoLed;

    bool estado;                           // Current state of the system (on or off)
    unsigned long ultimoTempoBotaoLiga;    // Last time the ON button was pressed
    unsigned long ultimoTempoBotaoDesliga; // Last time the OFF button was pressed

    /**
     * @brief Updates the state of the LED according to the system state
     */
    void atualizarLed()
    {
        if (estado)
        {
            digitalWrite(pinoLed, HIGH);
        }
        else
        {
            digitalWrite(pinoLed, LOW);
        }
    }
};

/**
 * @brief Class representing the control of the mixing process.
 */
class Controle
{
public:
    /**
     * @brief Constructor for the Controle class.
     * @param tanqueLeite Pointer to the tank object representing the milk tank.
     * @param tanqueEssencia Pointer to the tank object representing the essence tank.
     * @param tanqueMistura Pointer to the tank object representing the mixture tank.
     * @param valvulaLeite Pointer to the valve object representing the milk valve.
     * @param valvulaEssencia Pointer to the valve object representing the essence valve.
     * @param valvulaSaida Pointer to the valve object representing the output valve.
     * @param motor Pointer to the motor object.
     */
    Controle(Tanque *tanqueLeite, Tanque *tanqueEssencia, Tanque *tanqueMistura,
             Valvula *valvulaLeite, Valvula *valvulaEssencia, Valvula *valvulaSaida,
             Motor *motor)
    {
        this->tanqueLeite = tanqueLeite;
        this->tanqueEssencia = tanqueEssencia;
        this->tanqueMistura = tanqueMistura;
        this->valvulaLeite = valvulaLeite;
        this->valvulaEssencia = valvulaEssencia;
        this->valvulaSaida = valvulaSaida;
        this->motor = motor;

        this->estado = 0; // Initial state
        this->tempoInicial = 0;
    }

    /**
     * @brief Starts the mixing process.
     */
    void iniciarProcesso()
    {
        if (estado == 0) // Initial state
        {
            // Stores the volumes of the milk and essence tanks
            volumeLeiteInicial = tanqueLeite->getVolumeAtual();
            volumeEssenciaInicial = tanqueEssencia->getVolumeAtual();

            // Opens the milk valve for 5 seconds
            valvulaLeite->abrir();
            tempoInicial = millis();
            estado = 1; // Next state
        }
    }

    /**
     * @brief Updates the control state according to time and tank volumes
     */
    void atualizar()
    {
        if (estado == 1) // Opening of the milk valve
        {
            if (millis() - tempoInicial >= 5000) // 5 seconds have passed
            {
                // Closes the milk valve
                valvulaLeite->fechar();

                // Opens the essence valve
                valvulaEssencia->abrir();
                estado = 2; // Next state
            }
        }
        else if (estado == 2) // Opening of the essence valve
        {
            // Calculates the current essence/milk ratio
            int volumeLeiteUsado = volumeLeiteInicial - tanqueLeite->getVolumeAtual();
            int volumeEssenciaUsado = volumeEssenciaInicial - tanqueEssencia->getVolumeAtual();
            float proporcaoAtual = (float)volumeEssenciaUsado / volumeLeiteUsado;

            if (proporcaoAtual >= 0.1) // Reached the appropriate ratio
            {
                // Closes the essence valve
                valvulaEssencia->fechar();

                // Turns on the motor for 10 seconds
                motor->ligar();
                tempoInicial = millis();
                estado = 3; // Next state
            }
        }
        else if (estado == 3) // Turning on the motor
        {
            if (millis() - tempoInicial >= 10000) // 10 seconds have passed
            {
                // Turns off the motor
                motor->desligar();

                // Opens the output valve
                valvulaSaida->abrir();
                estado = 4; // Next state
            }
        }
        else if (estado == 4) // Opening of the output valve
        {
            if (tanqueMistura->getVolumeAtual() == 0) // Tank has emptied
            {
                // Closes the output valve
                valvulaSaida->fechar();

                estado = 0; // Returns to the initial state
            }
        }
    }

    /**
     * @brief Turns off the system and closes all valves and turns off the motor
     */
    void desligar()
    {
        if (estado != 0) // If not in the initial state
        {
            // Turns off the motor and closes the valves
            motor->desligar();
            valvulaLeite->fechar();
            valvulaEssencia->fechar();
            valvulaSaida->fechar();

            estado = 0; // Returns to the initial state
        }
    }

private:
    Tanque *tanqueLeite;
    Tanque *tanqueEssencia;
    Tanque *tanqueMistura;
    Valvula *valvulaLeite;
    Valvula *valvulaEssencia;
    Valvula *valvulaSaida;
    Motor *motor;

    int estado;
    unsigned long tempoInicial;

    int volumeLeiteInicial;
    int volumeEssenciaInicial;
};

/**
 * @brief Pin numbers of the components.
 */
const int PINO_TANQUE_LEITE = A0;
const int PINO_TANQUE_ESSENCIA = A1;
const int PINO_VALVULA_LEITE = 2;
const int PINO_VALVULA_ESSENCIA = 3;
const int PINO_VALVULA_SAIDA = 4;
const int PINO_MOTOR = 5;
const int PINO_BOTAO_LIGA = 6;
const int PINO_BOTAO_DESLIGA = 7;
const int PINO_LED = 8;

// Capacities of the tanks (in liters)
const int CAPACIDADE_TANQUE_LEITE = 10000;
const int CAPACIDADE_TANQUE_ESSENCIA = 10000;
const int CAPACIDADE_TANQUE_MISTURA = 20000;

Tanque tanqueLeite(CAPACIDADE_TANQUE_LEITE);
Tanque tanqueEssencia(CAPACIDADE_TANQUE_ESSENCIA);
Tanque tanqueMistura(CAPACIDADE_TANQUE_MISTURA);

Valvula valvulaLeite(PINO_VALVULA_LEITE);
Valvula valvulaEssencia(PINO_VALVULA_ESSENCIA);
Valvula valvulaSaida(PINO_VALVULA_SAIDA);

Motor motor(PINO_MOTOR);

Controle controle(&tanqueLeite, &tanqueEssencia, &tanqueMistura,
                  &valvulaLeite, &valvulaEssencia, &valvulaSaida,
                  &motor);

Sistema sistema(PINO_BOTAO_LIGA, PINO_BOTAO_DESLIGA, PINO_LED);

Nivel nivelLeite(PINO_TANQUE_LEITE, &tanqueLeite);
Nivel nivelEssencia(PINO_TANQUE_ESSENCIA, &tanqueEssencia);

void setup()
{
}

void loop()
{
    // Updates the tank volumes
    nivelLeite.atualizar();
    nivelEssencia.atualizar();

    // Checks if the LIGA button has been pressed
    sistema.verificarBotaoLiga();

    // Checks if the DESLIGA button has been pressed
    sistema.verificarBotaoDesliga();

    // If the system is on, update the control
    if (sistema.getEstado())
    {
        controle.atualizar();
    }
}

// Conclusão
//  Nesta aula prática, aplicamos os conhecimentos teóricos adquiridos durante a disciplina de Microcontroladores para programar e montar um hardware na plataforma Arduino. O objetivo era controlar a mistura de leite e essência utilizando botões, resistores, LEDs e potenciômetros. Para isso, elaboramos um programa no Arduino seguindo as orientações do roteiro e utilizando os princípios SOLID para criar classes separadas para cada parte do processo. Além disso, utilizamos a função millis() em vez de delay() para controlar o tempo de abertura das válvulas e ligamento do motor. Com isso, conseguimos simular com sucesso o processo de mistura de leite/essência e atingir os objetivos propostos nesta aula prática.