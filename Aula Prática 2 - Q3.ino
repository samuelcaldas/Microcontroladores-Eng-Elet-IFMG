/**
 * @file Aula Prática 2 - Programa 3.ino
 * @brief Aula Prática 2 - Programação e Montagem de Hardware na Plataforma Arduino
 * @version 1.0
 * @date 2023-06-04
 *
 * A. O roteiro envolve o desenvolvimento de uma lógica aplicada em um circuito com dois botões e dois LEDs.
 * B. Pressionando o botão b0 OU o botão b1 acendem os LEDs L1 e L2.
 * C. Liberando o acionamento dos botões, o circuito voltará as condições iniciais.
 * D. OBS: Utilizar a função OR ( | | ) da linguagem C.
 */

// Pinos do Arduino
int b0 = 2;  // Pino do botão b0
int b1 = 3;  // Pino do botão b1
int L1 = 12; // Pino do LED L1
int L2 = 13; // Pino do LED L2

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

    // Se o botão b0 OU o botão b1 estiverem pressionados
    if (estadoBotao0 == HIGH || estadoBotao1 == HIGH)
    {
        // Acende os LEDs L1 e L2
        digitalWrite(L1, HIGH);
        digitalWrite(L2, HIGH);
    }
    else
    {
        // Volta as condições iniciais (LEDs L1 e L2 apagados)
        digitalWrite(L1, LOW);
        digitalWrite(L2, LOW);
    }
}
