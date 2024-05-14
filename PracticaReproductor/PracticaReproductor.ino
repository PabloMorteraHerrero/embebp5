#include <LiquidCrystal_I2C.h>
#include <RotaryEncoder.h>

// Definiciones para las melodías
#define MELODY_NAME_LEN 15
#define MELODY_NOTES 10

// Estructura para una nota
typedef struct {
  int frequency;  // Hz
  int duration;   // ms
} note;

// Estructura para una melodía
typedef struct {
  char name[MELODY_NAME_LEN + 1];
  note notes[MELODY_NOTES];
} melody;

// Ejemplo de lista de melodías almacenadas
#include "pitches.h"

#define MELODIES 2

const melody melodies[MELODIES] = {
  {
    "Melodia A",
    { {NOTE_C4, 250}, {NOTE_G3, 125}, {NOTE_G3, 125}, {NOTE_A3, 250}, {NOTE_G3, 250}, {0, 250}, {NOTE_B3, 250}, {NOTE_C4, 250}, {0, 250}, {-1, 0} }
  },
  {
    "Melodia B",
    { {NOTE_C3, 125}, {NOTE_C3, 125}, {NOTE_D3, 250}, {NOTE_C3, 250}, {NOTE_F3, 250}, {NOTE_E3, 500}, {0, 250}, {-1, 0}, {-1, 0}, {-1, 0} }
  }
};

// Inicialización del visualizador LCD y el codificador rotativo
LiquidCrystal_I2C lcd(0x27, 16, 2);
RotaryEncoder encoder(2, 3);

int currentMelody = 0; // Índice de la melodía actual
int playbackSpeed = 100; // Velocidad de reproducción en porcentaje

void setup() {
  // Inicializar el LCD
  lcd.init();
  lcd.backlight();

  // Configurar pines del codificador rotativo
  pinMode(4, INPUT_PULLUP);  // Botón del codificador
  encoder.begin();

  // Configurar pin del zumbador
  pinMode(8, OUTPUT);
}

void loop() {
  static int lastPosition = -1;
  int newPosition = encoder.getPosition();

  if (newPosition < lastPosition) {
    // Girar a la izquierda: anterior melodía
    currentMelody = (currentMelody - 1 + MELODIES) % MELODIES;
    displayMelody();
  } else if (newPosition > lastPosition) {
    // Girar a la derecha: siguiente melodía
    currentMelody = (currentMelody + 1) % MELODIES;
    displayMelody();
  }

  lastPosition = newPosition;

  // Comprobar si se ha pulsado el botón del codificador
  if (digitalRead(4) == LOW) {
    // Reproducir la melodía seleccionada
    playMelody(melodies[currentMelody]);
  }
}

void displayMelody() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(currentMelody + 1);
  lcd.setCursor(0, 1);
  lcd.print(melodies[currentMelody].name);
}

void playMelody(melody m) {
  for (int i = 0; i < MELODY_NOTES; i++) {
    if (m.notes[i].frequency == -1) break; // Fin de la melodía
    if (m.notes[i].frequency == 0) { // Silencio
      delay(m.notes[i].duration);
    } else { // Nota musical
      tone(8, m.notes[i].frequency, m.notes[i].duration);
      delay(m.notes[i].duration); // Duración de la nota
    }
    delay(25); //
