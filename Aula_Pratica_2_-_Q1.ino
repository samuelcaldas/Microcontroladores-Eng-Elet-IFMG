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

void setup()
{
    pinMode(b0, INPUT);  // Define o pino do botão b0 como entrada
    pinMode(L1, OUTPUT); // Define o pino do LED L1 como saída
}

void loop()
{
    // Lê o estado do botão b0
    int estadoBotao = digitalRead(b0);

    if (estadoBotao == HIGH) // Se o botão b0 estiver pressionado
    {
        digitalWrite(L1, HIGH); // Liga o LED L1
    }
    else
    {
        digitalWrite(L1, LOW); // Desliga o LED L1
    }
}
