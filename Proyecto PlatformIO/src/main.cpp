#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

// --- CONFIGURACIÓN PINES ---
// Pantalla ST7920
U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, /* cs=*/ 5, /* reset=*/ -1);

// Encoder
const int PIN_CLK = 32;
const int PIN_DT = 33;
const int PIN_SW = 22;

// Motores TB6612FNG
const int PWMA = 25; const int AIN1 = 26; const int AIN2 = 27;
const int PWMB = 14; const int BIN1 = 12; const int BIN2 = 13;

// Feedback
const int BUZZER = 4;
const int LED_R = 21; const int LED_G = 16; const int LED_B = 17;
const int LED_AZUL = 19;

void setup() {
  Serial.begin(115200);
  
  // Inicializar Pantalla
  u8g2.begin();
  u8g2.setContrast(160);

  // Inicializar Entradas (Pull-ups internas)
  pinMode(PIN_CLK, INPUT_PULLUP);
  pinMode(PIN_DT, INPUT_PULLUP);
  pinMode(PIN_SW, INPUT_PULLUP);

  // Inicializar Salidas
  pinMode(PWMA, OUTPUT); pinMode(AIN1, OUTPUT); pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT); pinMode(BIN1, OUTPUT); pinMode(BIN2, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_R, OUTPUT); pinMode(LED_G, OUTPUT); pinMode(LED_B, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  Serial.println("¡Mati, el sistema está vivo! 🤖🔥");
}

void testPantalla(String msg) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(10, 30, "TESTING...");
  u8g2.drawStr(10, 50, msg.c_str());
  u8g2.sendBuffer();
}

void loop() {
  // 1. TEST LEDS (Semáforo de amor para Coni)
  testPantalla("LEDs RGB y Azul");
  digitalWrite(LED_R, HIGH); delay(500); digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, HIGH); delay(500); digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, HIGH); delay(500); digitalWrite(LED_B, LOW);
  digitalWrite(LED_AZUL, HIGH); delay(500); digitalWrite(LED_AZUL, LOW);

  // 2. TEST BUZZER
  testPantalla("Buzzer Pitido");
  tone(BUZZER, 1000, 200);
  delay(500);

  // 3. TEST MOTORES (Un giro cortito para no desarmar la protoboard)
  testPantalla("Motores N20");
  // Motor A
  digitalWrite(AIN1, HIGH); digitalWrite(AIN2, LOW); analogWrite(PWMA, 150);
  delay(500); analogWrite(PWMA, 0);
  // Motor B
  digitalWrite(BIN1, HIGH); digitalWrite(BIN2, LOW); analogWrite(PWMB, 150);
  delay(500); analogWrite(PWMB, 0);

  // 4. TEST ENCODER (Mira el Monitor Serial)
  testPantalla("Gira el Encoder!");
  unsigned long t = millis();
  while(millis() - t < 5000) { // Testear por 5 segundoss
    if(digitalRead(PIN_SW) == LOW) {
      Serial.println("¡Botón presionado! 🔘");
      tone(BUZZER, 2000, 50);
    }
    // (Lectura simple de CLK para ver si hay señal)
    if(digitalRead(PIN_CLK) == LOW) Serial.println("Giro detectado 🔄");
    delay(50);
  }
}