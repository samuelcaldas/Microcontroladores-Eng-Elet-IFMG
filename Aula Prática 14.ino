// Define constants for pin numbers
const int BOMBA_1_PIN = 2;
const int BOMBA_2_PIN = 3;
const int LED_VERDE_PIN = 4;
const int LED_AMARELO_PIN = 5;
const int CHAVE_SELETORA_PIN = 6;
const int BOTAO_1_PIN = 7;
const int BOTAO_2_PIN = 8;

// Define constants for water level thresholds
const int NIVEL_BAIXO = 25;
const int NIVEL_MEDIO = 50;
const int NIVEL_ALTO = 75;

// Define a class to represent the reservoir
class Reservatorio {
  public:
    // Constructor
    Reservatorio(int nivel) {
      this->nivel = nivel;
    }

    // Method to set the water level
    void setNivel(int nivel) {
      this->nivel = nivel;
    }

    // Method to get the water level
    int getNivel() {
      return this->nivel;
    }

  private:
    // Water level
    int nivel;
};

// Define a class to represent the pump
class Bomba {
  public:
    // Constructor
    Bomba(int pin) {
      this->pin = pin;
      pinMode(pin, OUTPUT);
      desligar();
    }

    // Method to turn on the pump
    void ligar() {
      digitalWrite(pin, HIGH);
    }

    // Method to turn off the pump
    void desligar() {
      digitalWrite(pin, LOW);
    }

  private:
    // Pin number
    int pin;
};

// Define a class to represent the LED
class LED {
  public:
    // Constructor
    LED(int pin) {
      this->pin = pin;
      pinMode(pin, OUTPUT);
      desligar();
    }

    // Method to turn on the LED
    void ligar() {
      digitalWrite(pin, HIGH);
    }

    // Method to turn off the LED
    void desligar() {
      digitalWrite(pin, LOW);
    }

  private:
    // Pin number
    int pin;
};

// Define a class to represent the control system
class SistemaDeControle {
  public:
    // Constructor
    SistemaDeControle(Reservatorio *reservatorio, Bomba *bomba1, Bomba *bomba2, LED *ledVerde, LED *ledAmarelo) {
      this->reservatorio = reservatorio;
      this->bomba1 = bomba1;
      this->bomba2 = bomba2;
      this->ledVerde = ledVerde;
      this->ledAmarelo = ledAmarelo;

      pinMode(CHAVE_SELETORA_PIN, INPUT_PULLUP);
      pinMode(BOTAO_1_PIN, INPUT_PULLUP);
      pinMode(BOTAO_2_PIN, INPUT_PULLUP);

      modoAutomatico = true;
      ledVerde->ligar();
      ledAmarelo->desligar();
    }

    // Method to update the control system
    void atualizar() {
      // Check if the mode selector switch has been toggled
      if (digitalRead(CHAVE_SELETORA_PIN) == LOW) {
        modoAutomatico = !modoAutomatico;

        if (modoAutomatico) {
          ledVerde->ligar();
        } else {
          ledVerde->desligar();
        }
      }

      if (modoAutomatico) {
        // Automatic mode

        // Get the current water level
        int nivel = reservatorio->getNivel();

        if (nivel < NIVEL_BAIXO) {
          // Water level is very low - turn on both pumps
          bomba1->ligar();
          bomba2->ligar();
        } else if (nivel < NIVEL_MEDIO) {
          // Water level is below 50% - turn on pump 1 and turn off pump 2
          bomba1->ligar();
          bomba2->desligar();
        } else if (nivel < NIVEL_ALTO) {
          // Water level is between 50% and 75% - turn off both pumps
          bomba1->desligar();
          bomba2->desligar();
        } else {
          // Water level is above 75% - turn off both pumps
          bomba1->desligar();
          bomba2->desligar();
        }
      } else {
        // Manual mode

        // Check if button 1 is pressed
        if (digitalRead(BOTAO_1_PIN) == LOW) {
          bomba1->ligar();
        } else {
          bomba1->desligar();
        }

        // Check if button 2 is pressed
        if (digitalRead(BOTAO_2_PIN) == LOW) {
          bomba2->ligar();
        } else {
          bomba2->desligar();
        }
      }
    }

  private:
    Reservatorio *reservatorio;
    Bomba *bomba1;
    Bomba *bomba2;
    LED *ledVerde;
    LED *ledAmarelo;

    bool modoAutomatico;
};

// Create objects for reservoir, pumps and LEDs
Reservatorio reservatorio(0);
Bomba bomba1(BOMBA_1_PIN);
Bomba bomba2(BOMBA_2_PIN);
LED ledVerde(LED_VERDE_PIN);
LED ledAmarelo(LED_AMARELO_PIN);

// Create object for control system
SistemaDeControle sistemaDeControle(&reservatorio, &bomba1, &bomba2, &ledVerde, &ledAmarelo);

void setup() {}

void loop() {
  sistemaDeControle.atualizar();

  delay(100);
}
