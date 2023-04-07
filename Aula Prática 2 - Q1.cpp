// Aula Prática 2 - Programa 1
// - Pressionando b0 liga o LED L1.
// - Liberando o acionamento de b0 desliga o LED L1.

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
