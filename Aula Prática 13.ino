class Tanque
{
public:
    Tanque(int capacidadeMaxima)
    {
        this->capacidadeMaxima = capacidadeMaxima;
        this->volumeAtual = 0;
    }

    void setVolumeAtual(int volume)
    {
        this->volumeAtual = volume;
    }

    int getVolumeAtual()
    {
        return this->volumeAtual;
    }

    int getCapacidadeMaxima()
    {
        return this->capacidadeMaxima;
    }

private:
    int capacidadeMaxima;
    int volumeAtual;
};

class Valvula
{
public:
    Valvula(int pino)
    {
        this->pino = pino;
        pinMode(pino, OUTPUT);
    }

    void abrir()
    {
        digitalWrite(pino, HIGH);
    }

    void fechar()
    {
        digitalWrite(pino, LOW);
    }

private:
    int pino;
};

class Motor
{
public:
    Motor(int pino)
    {
        this->pino = pino;
        pinMode(pino, OUTPUT);
    }

    void ligar()
    {
        digitalWrite(pino, HIGH);
    }

    void desligar()
    {
        digitalWrite(pino, LOW);
    }

private:
    int pino;
};

class Controle
{
public:
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

        this->estado = 0; // Estado inicial
        this->tempoInicial = 0;
    }

    void iniciarProcesso()
    {
        if (estado == 0) // Estado inicial
        {
            // Memoriza os volumes dos tanques de Leite e Essência
            volumeLeiteInicial = tanqueLeite->getVolumeAtual();
            volumeEssenciaInicial = tanqueEssencia->getVolumeAtual();

            // Abre a válvula de Leite por 5 segundos
            valvulaLeite->abrir();
            tempoInicial = millis();
            estado = 1; // Próximo estado
        }
    }

    void atualizar()
    {
        if (estado == 1) // Abertura da válvula de Leite
        {
            if (millis() - tempoInicial >= 5000) // Passaram-se 5 segundos
            {
                // Fecha a válvula de Leite
                valvulaLeite->fechar();

                // Abre a válvula de Essência
                valvulaEssencia->abrir();
                estado = 2; // Próximo estado
            }
        }
        else if (estado == 2) // Abertura da válvula de Essência
        {
            // Calcula a proporção atual de essência/leite
            int volumeLeiteUsado = volumeLeiteInicial - tanqueLeite->getVolumeAtual();
            int volumeEssenciaUsado = volumeEssenciaInicial - tanqueEssencia->getVolumeAtual();
            float proporcaoAtual = (float)volumeEssenciaUsado / volumeLeiteUsado;

            if (proporcaoAtual >= 0.1) // Atingiu a proporção adequada
            {
                // Fecha a válvula de Essência
                valvulaEssencia->fechar();

                // Liga o motor por 10 segundos
                motor->ligar();
                tempoInicial = millis();
                estado = 3; // Próximo estado
            }
        }
        else if (estado == 3) // Ligamento do motor
        {
            if (millis() - tempoInicial >= 10000) // Passaram-se 10 segundos
            {
                // Desliga o motor
                motor->desligar();

                // Abre a válvula de Saída
                valvulaSaida->abrir();
                estado = 4; // Próximo estado
            }
        }
        else if (estado == 4) // Abertura da válvula de Saída
        {
            if (tanqueMistura->getVolumeAtual() == 0) // Tanque esvaziou
            {
                // Fecha a válvula de Saída
                valvulaSaida->fechar();

                estado = 0; // Volta ao estado inicial
            }
        }
    }

    void desligar()
    {
        if (estado != 0) // Se não estiver no estado inicial
        {
            // Desliga o motor e fecha as válvulas
            motor->desligar();
            valvulaLeite->fechar();
            valvulaEssencia->fechar();
            valvulaSaida->fechar();

            estado = 0; // Volta ao estado inicial
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

// Pinos dos componentes
const int PINO_TANQUE_LEITE = A0;
const int PINO_TANQUE_ESSENCIA = A1;
const int PINO_VALVULA_LEITE = 2;
const int PINO_VALVULA_ESSENCIA = 3;
const int PINO_VALVULA_SAIDA = 4;
const int PINO_MOTOR = 5;
const int PINO_BOTAO_LIGA = 6;
const int PINO_BOTAO_DESLIGA = 7;

// Capacidades dos tanques (em litros)
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

void setup()
{
    // Configura os pinos dos botões como entrada
    pinMode(PINO_BOTAO_LIGA, INPUT_PULLUP);
    pinMode(PINO_BOTAO_DESLIGA, INPUT_PULLUP);
}

void loop()
{
    // Lê os valores dos potenciômetros e atualiza os volumes dos tanques
    int volumeLeite = map(analogRead(PINO_TANQUE_LEITE), 0, 1023, 0, CAPACIDADE_TANQUE_LEITE);
    int volumeEssencia = map(analogRead(PINO_TANQUE_ESSENCIA), 0, 1023, 0, CAPACIDADE_TANQUE_ESSENCIA);

    tanqueLeite.setVolumeAtual(volumeLeite);
    tanqueEssencia.setVolumeAtual(volumeEssencia);

    // Verifica se o botão LIGA foi pressionado
    if (digitalRead(PINO_BOTAO_LIGA) == LOW)
    {
        controle.iniciarProcesso();
    }

    // Verifica se o botão DESLIGA foi pressionado
    if (digitalRead(PINO_BOTAO_DESLIGA) == LOW)
    {
        controle.desligar();
    }

    // Atualiza o controle
    controle.atualizar();
}

// Conclusão
//  Nesta aula prática, aplicamos os conhecimentos teóricos adquiridos durante a disciplina de Microcontroladores para programar e montar um hardware na plataforma Arduino. O objetivo era controlar a mistura de leite e essência utilizando botões, resistores, LEDs e potenciômetros. Para isso, elaboramos um programa no Arduino seguindo as orientações do roteiro e utilizando os princípios SOLID para criar classes separadas para cada parte do processo. Além disso, utilizamos a função millis() em vez de delay() para controlar o tempo de abertura das válvulas e ligamento do motor. Com isso, conseguimos simular com sucesso o processo de mistura de leite/essência e atingir os objetivos propostos nesta aula prática.