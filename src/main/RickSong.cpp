#include "RickSong.h"
#include "KeyboardFlags.h"

extern uint8_t soundLength;

void playRick() {
  // This method is extracted from Robson Couto's code. It is not property of group 13.
  // See https://github.com/robsoncouto/arduino-songs.
  
  int tempo = 114;

  // change this to whichever pin you want to use
  int buzzer = A8;

  // notes of the moledy followed by the duration.
  // a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
  // !!negative numbers are used to represent dotted notes,
  // so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
  /*
  int melody[] = { // Melody as it was implemented

    // Never Gonna Give You Up - Rick Astley
    // Score available at https://musescore.com/chlorondria_5/never-gonna-give-you-up_alto-sax
    // Arranged by Chlorondria

    NOTE_D5,-4, NOTE_E5,-4, NOTE_A4,4, //1
    
    NOTE_E5,-4, NOTE_FS5,-4, NOTE_A5,16, NOTE_G5,16, NOTE_FS5,8,
    NOTE_D5,-4, NOTE_E5,-4, NOTE_A4,2,
    NOTE_A4,16, NOTE_A4,16, NOTE_B4,16, NOTE_D5,8, NOTE_D5,16,
    

    
    NOTE_D5,-4, NOTE_E5,-4, NOTE_A4,4, //repeat from 1
    NOTE_E5,-4, NOTE_FS5,-4, NOTE_A5,16, NOTE_G5,16, NOTE_FS5,8,
    NOTE_D5,-4, NOTE_E5,-4, NOTE_A4,2,
    NOTE_A4,16, NOTE_A4,16, NOTE_B4,16, NOTE_D5,8, NOTE_D5,16,
    
  };
  */

  const int* melody;
  int melodyLength = 0;

  switch(soundLength)
  {
    case 1:
      melody = new int[6]{ NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4 };
      melodyLength = 6; // Length of the array
      break;

    case 2:
      melody = new int[12]{ 
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4,
        NOTE_E5, -4, NOTE_FS5, -4, NOTE_A5, 16
      };
      melodyLength = 12;
      break;

    case 3:
      melody = new int[18]{ 
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4,
        NOTE_E5, -4, NOTE_FS5, -4, NOTE_A5, 16, NOTE_G5, 16, NOTE_FS5, 8 
      };
      melodyLength = 18;
      break;

    case 4:
      melody = new int[24]{ 
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4,
        NOTE_E5, -4, NOTE_FS5, -4, NOTE_A5, 16, NOTE_G5, 16, NOTE_FS5, 8,
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 2
      };
      melodyLength = 24;
      break;

    case 5:
      melody = new int[32]{ 
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4,
        NOTE_E5, -4, NOTE_FS5, -4, NOTE_A5, 16, NOTE_G5, 16, NOTE_FS5, 8,
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 2,
        NOTE_A4, 16, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 8, NOTE_D5, 16
      };
      melodyLength = 32;
      break;

    case 6:
      melody = new int[38]{ 
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4,
        NOTE_E5, -4, NOTE_FS5, -4, NOTE_A5, 16, NOTE_G5, 16, NOTE_FS5, 8,
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 2,
        NOTE_A4, 16, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 8, NOTE_D5, 16,
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4
      };
      melodyLength = 38;
      break;

    case 7:
      melody = new int[42]{ 
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4,
        NOTE_E5, -4, NOTE_FS5, -4, NOTE_A5, 16, NOTE_G5, 16, NOTE_FS5, 8,
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 2,
        NOTE_A4, 16, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 8, NOTE_D5, 16,
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4,
        NOTE_E5, -4, NOTE_FS5, -4
      };
      melodyLength = 42;
      break;

    case 8:
      melody = new int[48]{ 
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4,
        NOTE_E5, -4, NOTE_FS5, -4, NOTE_A5, 16, NOTE_G5, 16, NOTE_FS5, 8,
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 2,
        NOTE_A4, 16, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 8, NOTE_D5, 16,
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4,
        NOTE_E5, -4, NOTE_FS5, -4, NOTE_A5, 16, NOTE_G5, 16, NOTE_FS5, 8
      };
      melodyLength = 48;
      break;

    case 9:
      melody = new int[54]{ 
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4,
        NOTE_E5, -4, NOTE_FS5, -4, NOTE_A5, 16, NOTE_G5, 16, NOTE_FS5, 8,
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 2,
        NOTE_A4, 16, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 8, NOTE_D5, 16,
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4,
        NOTE_E5, -4, NOTE_FS5, -4, NOTE_A5, 16, NOTE_G5, 16, NOTE_FS5, 8,
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 2
      };
      melodyLength = 54;
      break;

    case 10:
      melody = new int[64]{ 
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4,
        NOTE_E5, -4, NOTE_FS5, -4, NOTE_A5, 16, NOTE_G5, 16, NOTE_FS5, 8,
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 2,
        NOTE_A4, 16, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 8, NOTE_D5, 16,
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4,
        NOTE_E5, -4, NOTE_FS5, -4, NOTE_A5, 16, NOTE_G5, 16, NOTE_FS5, 8,
        NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 2,
        NOTE_A4, 16, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 8, NOTE_D5, 16
      };
      melodyLength = 64;
      break;

    default: // soundLength = 0
      melody = nullptr; // No melody
      melodyLength = 0;
      break;
  }

  if (melodyLength == 0) { // skip if melody is empty
    return;
  }

  // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
  // there are two values per note (pitch and duration), so for each note there are four bytes
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;

  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;

  int divider = 0, noteDuration = 0;
  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }

  delete[] melody; // Free allocated memory
}