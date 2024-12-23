#define CLOCK 7 //D-
#define DATA 6  //D+

#define MAX_MESSAGE_LENGTH 1024

#include <LiquidCrystal.h>  //Best imported by library manager

const char keymapUS[] = {   // US Keyboard
  0, 'F9',  0,  'F5',  'F3',  'F1',  'F6',  'F12',      // 08
  0, 'F10',  'F8',  'F6',  'F4',  'TAB', '`', 0,        // 10
  0, 'ALT' , 'LSHIFT' , 0,  'LCTRL', 'q','1', 0,        // 18
  0, 0, 'z','s','a','w','2', 0,                         // 20
  0,'c','x','d','e','4','3', 0,                         // 28
  0,' ','v','f','t','r','5', 0,                         // 30
  0,'n','b','h','g','y','6', 0,                         // 38
  0, 0, 'm','j','u','7','8', 0,                         // 40
  0,',','k','i','o','0','9', 0,                         // 48
  0,'.','/','l',';','p','-', 0,                         // 50
  0, 0,'\'', 0,'[', '=', 0, 0,                          // 58
  'CAPSLOCK', 'RSHIFT','ENTER', ']', 0, '\\', 0, 0,     // 60
  0, 0, 0, 0, 0, 0, 'BACKSPACE', 0,                     // 68
  0,'1', 0,'4','7', 0, 'ESC', 0,                        // 70
  '0','.','2','5','6','8', 0, 'NUMLOCK',                // 78
  'F11','+','3','-','*','9', 0, 0,                      // 80
  0, 0, 0, 'F7' };                                      // 84

  const char keymapGER[] = {   // German Keyboard
  0, 'F9',  0,  'F5',  'F3',  'F1',  'F6',  'F12',      // 08
  0, 'F10',  'F8',  'F6',  'F4',  'TAB', '^', 0,        // 10
  0, 'ALTGR' , 'LSHIFT' , 0,  'LCTRL', 'q','1', 0,      // 18
  0, 0, 'y','s','a','w','2', 0,                         // 20
  0,'c','x','d','e','4','3', 0,                         // 28
  0,' ','v','f','t','r','5', 0,                         // 30
  0,'n','b','h','g','z','6', 0,                         // 38
  0, 0, 'm','j','u','7','8', 0,                         // 40
  0,',','k','i','o','0','9', 0,                         // 48
  0,'.','-','l','ö','p','ß', 0,                         // 50
  0, 0,'ä', 0,'ü', '`', 0, 0,                           // 58 
  'CAPSLOCK', 'RSHIFT','ENTER', '+', 0, '#', 0, 0,      // 60 Maybo 'ENTER' should be 0 or '#'
  0, '<', 0, 0, 0, 0, 'BACKSPACE', 0,                   // 68
  0,'1', 0,'4','7', 0, 'ESC', 0,                        // 70
  '0','.','2','5','6','8', 0, 'NUMLOCK',                // 78
  'F11','+','3','-','*','9', 0, 0,                      // 80
  0, 0, 0, 'F7' };                                      // 84

const char keymapGERShift[] = {    // for pressing ALTGR
  0, 0,  0,  0,  0, 0, 0,  0,                           // 08
  0, 0, 0, 0, 0, 0, '°', 0,                             // 10
  0, 0, 0, 0, 0, 'Q', '!', 0,                           // 18
  0, 0, 'Y', 'S', 'A', 'W', '"', 0,                     // 20
  0, 'C', 'X', 'D', 'E', '$', '§', 0,                   // 28
  0, ' ', 'V', 'F', 'T', 'R', '%', 0,                   // 30
  0, 'N', 'B', 'H', 'G', 'Z', '&', 0,                   // 38
  0, 0, 'M', 'J', 'U', '/', '(', 0,                     // 40
  0, ';', 'K', 'I', 'O', '=', ')', 0,                   // 48
  0, ':', '_', 'L', 'Ö', 'P', '?', 0,                   // 50
  0, 0, 'Ä', 0, 'Ü', '`', 0, 0,                         // 58
  'CAPSLOCK', 0, 0, '*', 0, '`', 0, 0,                  // 60
  0, '>', 0, 0, 0, 0, 0, 0,                             // 68
  0, 0, 0, 0, 0, 0, 0, 0,                               // 70
  0, 0, 0, 0, 0, 0, 0, 0,                               // 78
  0, 0, 0, 0, 0, 0, 0, 0,                               // 80
  0, 0, 0, 0};                                          // 84 

const char keymapGERAltGR [] = {  // for pressing shift
  0, 0,  0,  0, 0, 0, 0,  0,                            // 08
  0, 0, 0, 0, 0, 0, '|', 0,                             // 10
  0, 0, 0, 0, 0, '@', 0, 0,                             // 18
  0, 0, 0, 0, 0, 0, '²', 0,                             // 20
  0, 0, 0, 0, '€', 0, 0, '³',                           // 28
  0, 0, 0, 0, 0, 0, 0, 0,                               // 30
  0, 0, 0, 0, 0, 0, 0, 0,                               // 38
  0, 0, 'µ', 0, 0, '{', '[', 0,                         // 40
  0, 0, 0, 0, 0, '}', ']', 0,                           // 48
  0, 0, 0, 0, 0, 0, '\\', 0,                            // 50
  0, 0, 0, 0, 0, 0, 0, 0,                               // 58
  0, 0, 0, '~', 0, 0, 0, 0,                             // 60
  0, '|', 0, 0, 0, 0, 0, 0,                             // 68
  0, 0, 0, 0, 0, 0, 0, 0,                               // 70
  0, 0, 0, 0, 0, 0, 0, 0,                               // 78
  0, 0, 0, 0, 0, 0, 0, 0,                               // 80
  0, 0, 0, 0};                                          // 84

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

bool altgrActive = false;
bool shiftActive = false;
bool capsActive = false;

unsigned long shiftLastTime = 0;
unsigned long altgrLastTime = 0;

const unsigned long timeout = 100; // Timeout for Shift and AltGR in ms

char message[MAX_MESSAGE_LENGTH + 1] = ""; // Message buffer

void setup()
{
  Serial.begin(115200);
  pinMode(CLOCK, INPUT_PULLUP); //For most keyboards the builtin pullups are sufficient, so the 10k pullups can be omitted
  pinMode(DATA, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  lcd.begin(20, 4);
  lcd.cursor();
  lcd.blink();
  bitSet(PCICR, PCIE2); // Enable pin change interrupts on pin D0-D7
  bitSet(PCMSK2, CLOCK); // Pin change interrupt on Clock pin
 }

uint8_t lastscan = 0;
uint8_t lastlastscan = 0;
uint8_t line = 0, col = 0;
char input = NULL;


ISR(PCINT2_vect)
{
  uint16_t scanval = 0;
  for(int i = 0; i<11; i++)
  {
    while(digitalRead(CLOCK));
    scanval |= digitalRead(DATA) << i; // 1 start, 8 data bits, 1 parity, 1 stop bit
    while(!digitalRead(CLOCK));
  }

  scanval >>= 1; // ignore the start bit
  scanval &= 0xFF; // ignore the parity and stop bit, isolate 8 data bits
  
  if(lastscan != 0xF0 && scanval != 0xF0){
  //Serial.println(scanval, HEX);
    if (scanval == 0x12 || scanval == 0x59) { // Shift press
      shiftLastTime = millis(); // Reset Shift timer
      shiftActive = true;
      //output = keymapGERShift[scanval];
    } else if (scanval == 0x11 && lastscan == 0xE0) { // AltGR press
      altgrActive = true;
      altgrLastTime = millis();
    } else if (scanval == 0x58){
      capsActive = !capsActive;
    }else {
      char output;
    }
	  switch(scanval)
	  {
		case 0x5A: //Enter
      handle_enter_key();
		  lcd.setCursor(0, ++line & 0x03);
		  col = 0;
		  break;
		case 0x66: //Backspace
      if (strlen(message) > 0) {
        delete_last_char_from_message();
        Serial.println("DELETE_LAST_CHAR_ON_LCD_SCREEN");
        lcd.setCursor(--col, line);
        lcd.write(' ');
        lcd.setCursor(col, line);
      }
		break;
    /*
    case 0x4C: //Ö
      input = "oe";
      col++;
    case 0x52: //Ä
      input = "ae";
      col++;
    case 0x54: //Ü
      input = "ue";
      col++;
    */
		default:
      if (shiftActive) {
        input = keymapGERShift[scanval]; // input = shift + key
      } else if (altgrActive) {
        input = keymapGERAltGR[scanval]; // input = AltGR + key
      } else if (capsActive) {
        input = keymapGERShift[scanval]; 
      }else {
        if (scanval == 0x58) {
          input = 0;
        } else {
          input = keymapGER[scanval];      // input = key
        }
      }
      if (input != 0) { // only print if valid key is pressed
        if (scanval == 0x58) {
          input = 0;
        } else {
          add_char_to_message(input);
          lcd.write(input);
          Serial.println(input);
          col++;
        }
      }
	  }
  }

  // Combine input to String

  

  lastlastscan = lastscan;
  lastscan = scanval;
  bitSet(PCIFR, PCIF2);
}

// char combining tests

void add_char_to_message(char ch) {
    int len = strlen(message);
    if (len < MAX_MESSAGE_LENGTH) { // Ensure we don't exceed buffer size
        message[len] = ch;
        message[len + 1] = '\0';
    }
}

void delete_last_char_from_message() {
    int len = strlen(message);
    if (len > 0) {
        message[len - 1] = '\0';
    }
}

void handle_enter_key() {
    // Simulate sending the message
    Serial.println("Message sent:");
    Serial.println(message);

    // Reset the message
    message[0] = '\0';
}








void loop()
{
  unsigned long currentTime = millis();
  
  // Deactivate Shift if no input for the timeout duration
  if (shiftActive && (currentTime - shiftLastTime > timeout)) {
    shiftActive = false;
  }

  // Deactivate AltGR if no input for the timeout duration
  if (altgrActive && (currentTime - altgrLastTime > timeout)) {
    altgrActive = false;
  }


  digitalWrite(13, LOW);
  delay(500);  
  digitalWrite(13, HIGH);
  delay(500);
}