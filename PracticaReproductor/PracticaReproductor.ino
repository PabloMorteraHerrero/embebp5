#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include "pitches.h"

// Configuración del LCD I2C: dirección 0x27 y tamaño 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definición del codificador rotativo
Encoder knob(2, 3);
#define BUTTON_PIN 4

// Definición del zumbador
#define BUZZER_PIN 8

#define MELODY_NAME_LEN 15
#define MELODY_NOTES 10
#define MELODIES 2

typedef struct {
    int frequency;  // Hz
    int duration;   // ms
} note;

typedef struct {
    char name[MELODY_NAME_LEN + 1];
    note notes[MELODY_NOTES];
} melody;

// Ejemplo de lista de melodías
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

int melodyIndex = 0;
int playbackSpeed = 100;  // Porcentaje de la velocidad de reproducción

// Variables para el manejo del estado
enum State { SELECT_MELODY, PLAY_MELODY };
State currentState = SELECT_MELODY;

long oldPosition = -999;
bool buttonPressed = false;

void setup() {
    lcd.init();
    lcd.backlight();
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);
    Serial.begin(9600);

    // Mostrar la primera melodía
    displayMelodySelection();
}

void loop() {
    long newPosition = knob.read();
    if (newPosition != oldPosition) {
        if (currentState == SELECT_MELODY) {
            handleKnobTurn(newPosition);
        } else if (currentState == PLAY_MELODY) {
            adjustPlaybackSpeed(newPosition);
        }
        oldPosition = newPosition;
    }

    if (digitalRead(BUTTON_PIN) == LOW) {
        if (!buttonPressed) {
            buttonPressed = true;
            delay(50);  // Debounce delay
            if (currentState == SELECT_MELODY) {
                currentState = PLAY_MELODY;
                displayPlaybackInfo();
                playMelody();
            } else {
                currentState = SELECT_MELODY;
                displayMelodySelection();
            }
        }
    } else {
        buttonPressed = false;
    }
}

void displayMelodySelection() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(melodyIndex + 1);
    lcd.setCursor(0, 1);
    lcd.print(melodies[melodyIndex].name);
}

void displayPlaybackInfo() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(melodies[melodyIndex].name);
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.print(MELODY_NOTES);
    lcd.print(" notas, ");
    lcd.print(playbackSpeed);
    lcd.print("%");
}

void handleKnobTurn(long newPosition) {
    melodyIndex = (melodyIndex + newPosition + MELODIES) % MELODIES;
    displayMelodySelection();
}

void adjustPlaybackSpeed(long newPosition) {
    playbackSpeed = constrain(playbackSpeed + newPosition, 50, 150);
    displayPlaybackInfo();
}

void playMelody() {
    const melody& mel = melodies[melodyIndex];
    for (int i = 0; i < MELODY_NOTES && currentState == PLAY_MELODY; i++) {
        int freq = mel.notes[i].frequency;
        int dur = mel.notes[i].duration;
        if (freq < 0) break;
        int adjustedDur = dur * 100 / playbackSpeed;
        if (freq == 0) {
            delay(adjustedDur);
        } else {
            tone(BUZZER_PIN, freq, adjustedDur);
            delay(adjustedDur + 25);
        }
    }
    noTone(BUZZER_PIN);
}

