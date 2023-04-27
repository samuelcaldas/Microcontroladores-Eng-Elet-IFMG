// Aula Prática 2 - Programa 2
// - Inicialmente o LED L1 estará aceso e o LED L2 estará apagado.
// - Ao pressionar b0 apagará o LED L1 e acenderá o LED L2, liberando acionamento
//   do botão b0, o circuito voltará as condições iniciais

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
