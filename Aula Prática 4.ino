// Define os pinos dos LEDs
#define LED_VERMELHO 11
#define LED_VERDE    10
#define LED_AZUL     9

// Define os pinos dos potenciômetros
#define POT_VERMELHO A0
#define POT_VERDE    A1
#define POT_AZUL     A2

// Define o intervalo de tempo entre a troca de cores do LED RGB
int intevalo = 1000;

// Define o intervalo de tempo entre a leitura dos potenciômetros
int debounce = 100;

void setup()
{
    // Inicializa a comunicação serial
    Serial.begin(9600);

    // Inicializa os pinos do LED como saída
    pinMode(LED_VERMELHO, OUTPUT);
    pinMode(LED_VERDE, OUTPUT);
    pinMode(LED_AZUL, OUTPUT);

    // Testa o funcionameto de todas as cores do LED
    testa_led();
}

void loop()
{
    // Lê os valores dos potenciômetros (entre 0 e 1023)
    int valorVermelho = analogRead(POT_VERMELHO);
    int valorVerde    = analogRead(POT_VERDE);
    int valorAzul     = analogRead(POT_AZUL);

    // Imprime a porcentagem dos potenciometros na saída serial
    print_pot(valorVermelho, valorVerde, valorAzul);

    // Converte os valores para o intervalo de PWM (entre 0 e 255)
    valorVermelho = map(valorVermelho, 0, 1023, 0, 255);
    valorVerde    = map(valorVerde, 0, 1023, 0, 255);
    valorAzul     = map(valorAzul, 0, 1023, 0, 255);

    // Escreve os valores nos pinos dos LEDs usando PWM
    analogWrite(LED_VERMELHO, valorVermelho);
    analogWrite(LED_VERDE, valorVerde);
    analogWrite(LED_AZUL, valorAzul);

    // Aguarda um tempo para evitar que a leitura dos potenciômetros seja feita com muita frequência
    delay(debounce);
}

// Esta função testa o funcionamento de todas as cores do LED RGB
void testa_led()
{
    // Acende o LED vermelho na intensidade máxima
    analogWrite(LED_VERMELHO, 255);
    // Apaga os outros LEDs
    analogWrite(LED_VERDE, 0);
    analogWrite(LED_AZUL, 0);
    // Aguarda um segundo
    delay(intevalo);

    // Acende o LED verde na intensidade máxima
    analogWrite(LED_VERDE, 255);
    // Apaga os outros LEDs
    analogWrite(LED_VERMELHO, 0);
    analogWrite(LED_AZUL, 0);
    // Aguarda um segundo
    delay(intevalo);

    // Acende o LED azul na intensidade máxima
    analogWrite(LED_AZUL, 255);
    // Apaga os outros LEDs
    analogWrite(LED_VERMELHO, 0);
    analogWrite(LED_VERDE, 0);
    // Aguarda um segundo
    delay(intevalo);

    // Define todas as cores no valor mínimo (apaga o LED RGB)
    analogWrite(LED_VERMELHO, 0);
    analogWrite(LED_VERDE, 0);
    analogWrite(LED_AZUL, 0);
    // Aguarda um segundo
    delay(intevalo);
}

// Esta função recebe três valores inteiros entre 0 e 1023 e imprime na saída serial a porcentagem correspondente de cada potenciômetro
// A função só imprime se algum dos valores for diferente do anterior
void print_pot(int vermelho, int verde, int azul)
{
    // Variáveis estáticas para armazenar os valores anteriores
    static int vermelho_anterior = -1;
    static int verde_anterior    = -1;
    static int azul_anterior     = -1;

    // Verifica se algum dos valores mudou
    if (vermelho != vermelho_anterior || verde != verde_anterior || azul != azul_anterior)
    {
        // Calcula a porcentagem de cada valor
        float vermelho_pct = map(vermelho, 0, 1023, 0, 100);
        float verde_pct    = map(verde, 0, 1023, 0, 100);
        float azul_pct     = map(azul, 0, 1023, 0, 100);

        // Imprime na saída serial no formato desejado
        Serial.print("Potenciometro Vermelho: ");
        Serial.print(vermelho_pct);
        Serial.print("% | Potenciometro Verde: ");
        Serial.print(verde_pct);
        Serial.print("% | Potenciometro Azul: ");
        Serial.print(azul_pct);
        Serial.println("%");

        // Atualiza os valores anteriores
        vermelho_anterior = vermelho;
        verde_anterior    = verde;
        azul_anterior     = azul;
    }
}
