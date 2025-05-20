#include <SoftwareSerial.h>

#define dataPin 8
#define latchPin 9
#define clockPin 10

SoftwareSerial btSerial(2, 3); // RX, TX

// Mapa de caracteres 0-9 y A-Z
const byte charMap[36] = {
  B00111111, B00000110, B01011011, B01001111, B01100110,
  B01101101, B01111101, B00000111, B01111111, B01101111,
  B01110111, B01111100, B00111001, B01011110, B01111001,
  B01110001, B01111101, B01110110, B00000110, B00011110,
  B01110000, B00111000, B00010101, B00110111, B00111111,
  B01110011, B01100111, B00110001, B01101101, B01111000,
  B00111110, B00011100, B00011101, B01000110, B01101110,
  B01011011
};

String mensaje = "";

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  btSerial.begin(9600);
}

void loop() {
  while (btSerial.available()) {
    char c = btSerial.read();
    if (c == '\n') {
      mostrarMensaje();
      mensaje = "";
    } else {
      mensaje += c;
    }
  }
}

void mostrarMensaje() {
  if (mensaje.length() < 2) return;

  for (int i = 0; i < mensaje.length() - 1; i++) {
    byte seg1 = getSegmentCode(mensaje[i]);
    byte seg2 = getSegmentCode(mensaje[i + 1]);

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, seg2);
    shiftOut(dataPin, clockPin, MSBFIRST, seg1);
    digitalWrite(latchPin, HIGH);
    delay(800);
  }
}

byte getSegmentCode(char c) {
  if (c >= '0' && c <= '9') return charMap[c - '0'];
  if (c >= 'A' && c <= 'Z') return charMap[c - 'A' + 10];
  if (c >= 'a' && c <= 'z') return charMap[c - 'a' + 10];
  return B00000000;
}