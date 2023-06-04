/**
 * @file Aula Prática 2 - Programa 1.ino
 * @brief Aula Prática 2 - Programação e Montagem de Hardware na Plataforma Arduino
 * @version 1.0
 * @date 2023-06-04
 *
 * A. O roteiro envolve o desenvolvimento de uma lógica aplicada em um circuito com um botão e um LED.
 * B. Pressionando b0 liga o LED L1.
 * C. Liberando o acionamento de b0 desliga o LED L1.
 */

// Pinos do Arduino
int b0 = 2;  // Pino do botão b0
int L1 = 12; // Pino do LED L1
int L2 = 13; // Pino do LED L2

void setup()
{
    // Define o pino do botão b0 como entrada
    pinMode(b0, INPUT);

    // Define os pinos dos LEDs L1 e L2 como saída
    pinMode(L1, OUTPUT);
    pinMode(L2, OUTPUT);

    // Inicialmente o LED L1 estará aceso e o LED L2 estará apagado
    digitalWrite(L1, HIGH);
    digitalWrite(L2, LOW);
}

void loop()
{
    int estadoBotao = digitalRead(b0); // Lê o estado do botão b0

    if (estadoBotao == HIGH) // Se o botão b0 estiver pressionado
    {
        // Apaga o LED L1 e acende o LED L2
        digitalWrite(L1, LOW);
        digitalWrite(L2, HIGH);
    }
    else
    {
        // Volta as condições iniciais (LED L1 aceso e LED L2 apagado)
        digitalWrite(L1, HIGH);
        digitalWrite(L2, LOW);
    }
}
