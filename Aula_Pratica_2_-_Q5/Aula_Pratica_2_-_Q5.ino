/**
 * @file Aula Prática 2 - Programa 5.ino
 * @brief Aula Prática 2 - Programação e Montagem de Hardware na Plataforma Arduino
 * @version 1.0
 * @date 2023-06-04
 *
 * A. O roteiro envolve o desenvolvimento de uma lógica aplicada em um circuito com dois botões e dois LEDs.
 * B. Pressionando 3 vezes o botão b0, acende os leds L1 e L2 se mantendo acesos.
 * C. Pressionando o botão b1, apaga todos os leds com o circuito voltando as condições iniciais.
 */

// Pinos do Arduino
int b0 = 2;  // Pino do botão b0
int b1 = 3;  // Pino do botão b1
int L1 = 12; // Pino do LED L1
int L2 = 13; // Pino do LED L2

// Variável para contar o número de vezes que o botão b0 foi pressionado
int contador = 0;

// Variável para armazenar o tempo de debounce em milissegundos
int debounce = 200;

void setup()
{
    // Define os pinos dos botões b0 e b1 como entrada
    pinMode(b0, INPUT);
    pinMode(b1, INPUT);

    // Define os pinos dos LEDs L1 e L2 como saída
    pinMode(L1, OUTPUT);
    pinMode(L2, OUTPUT);
}

void loop()
{
    // Lê o estado dos botões b0 e b1
    int estadoBotao0 = digitalRead(b0);
    int estadoBotao1 = digitalRead(b1);

    // Se o botão b0 estiver pressionado
    if (estadoBotao0 == HIGH)
    {
        // Incrementa o contador
        contador++;
        delay(debounce); // Aguarda um tempo para evitar contagem múltipla de um único clique
    }

    // Se o botão b0 foi pressionado 3 ou mais vezes
    if (contador >= 3)
    {
        // Acende os LEDs L1 e L2
        digitalWrite(L1, HIGH);
        digitalWrite(L2, HIGH);
    }

    // Se o botão b1 estiver pressionado
    if (estadoBotao1 == HIGH && contador >= 3)
    {
        // Apaga os LEDs L1 e L2 e reseta o contador
        digitalWrite(L1, LOW);
        digitalWrite(L2, LOW);
        contador = 0;
    }
}
