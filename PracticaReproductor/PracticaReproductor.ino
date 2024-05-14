#include "pitches.h"

#define MELODY_NAME_LEN 15
#define MELODY_NOTES 10
#define MELODIES 2

typedef struct {
    int frequency;  /* Hz */
    int duration;   /* ms */
} note;

typedef struct {
    char name[MELODY_NAME_LEN + 1];
    note notes[MELODY_NOTES];
} melody;

const melody melodies[MELODIES] = {
  { "Melodia A",
    { { NOTE_C4, 250 }, { NOTE_G3, 125 }, { NOTE_G3, 125 }, { NOTE_A3, 250 }, { NOTE_G3, 250 }, { 0, 250 }, { NOTE_B3, 250 }, { NOTE_C4, 250 }, { 0, 250 }, { -1, 0 } } },
  { "Melodia B",
    { { NOTE_C3, 125 }, { NOTE_C3, 125 }, { NOTE_D3, 250 }, { NOTE_C3, 250 }, { NOTE_F3, 250 }, { NOTE_E3, 500 }, { 0, 250 }, { -1, 0 }, { -1, 0 }, { -1, 0 } } },
