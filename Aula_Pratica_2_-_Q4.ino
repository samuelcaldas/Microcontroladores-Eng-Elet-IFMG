/**
 * @file Aula Prática 2 - Programa 4.ino
 * @brief Aula Prática 2 - Programação e Montagem de Hardware na Plataforma Arduino
 * @version 1.0
 * @date 2023-06-04
 *
 * A. O roteiro envolve o desenvolvimento de uma lógica aplicada em um circuito com dois botões e dois LEDs.
 * B. Pressionando os botões b0 E b1 simultaneamente, acendem os LEDs L1 e L2.
 * C. Liberando o acionamento dos botões, os leds continuam acesos.
 * D. Pressionando novamente os botões b0 E b1 simultaneamente, apagam os LEDs L1 e L2.
 * E. Liberando o acionamento dos botões, os leds continuam apagados.
 * F. OBS: Utilizar a função AND (&) da linguagem C.
 */

// Pinos do Arduino
int b0 = 2;  // Pino do botão b0
int b1 = 3;  // Pino do botão b1
int L1 = 12; // Pino do LED L1
int L2 = 13; // Pino do LED L2

// Variável para armazenar o estado dos LEDs (acesos ou apagados)
bool ledsAcesos = false;

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

    // Se os botões b0 E b1 estiverem pressionados simultaneamente
    if (estadoBotao0 == HIGH && estadoBotao1 == HIGH)
    {
        // Aguarda o tempo de debounce
        delay(debounce);

        // Alterna o estado dos LEDs (se estiverem acesos, apaga; se estiverem apagados, acende)
        ledsAcesos = !ledsAcesos;
        digitalWrite(L1, ledsAcesos);
        digitalWrite(L2, ledsAcesos);
    }
}
