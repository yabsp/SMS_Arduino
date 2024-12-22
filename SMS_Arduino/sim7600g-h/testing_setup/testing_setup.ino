#include <SoftwareSerial.h>

#define RX_PIN 7 // Connect to TX of SIM7600, should be 7
#define TX_PIN 8  // Connect to RX of SIM7600, should be 8
SoftwareSerial sim7600(RX_PIN, TX_PIN);
int counter = 0;
const char simPin[] = "0135";

bool responseEqualsGiven(const char*);
void readWhileAvailable();

void setup() {
  Serial.begin(4800);        
  sim7600.begin(115200); //initialize at 115200 which is the default of the sim7600g-h module

  Serial.println("Initializing SIM7600...");
  delay(1000);

  // Routine to change baud rate, from default to 9600.
  // This is done as the default baud rate causes decoding errors.
  int count = 0;
  do{
    sim7600.println("AT");
    Serial.println("Checking connection, please wait...");
    delay(100);
    count++;
  } while(!responseEqualsGiven("OK") && count < 4);

  count = 0;
  do {
    Serial.println("Configuring baud rate to 9600, please wait...");
    sim7600.println("AT+IPREX=9600");
    delay(100);
    sim7600.begin(9600);
    count++;
  } while(!responseEqualsGiven("OK") && count < 4);
  
  Serial.println("AT");
  sim7600.println("AT");
  delay(1000);
  readWhileAvailable();
  
  Serial.println("AT+CPIN?");
  sim7600.println("AT+CPIN?");
  delay(1000);
  readWhileAvailable();

  Serial.println("AT+CPIN=\"0135\""); 
  sim7600.println("AT+CPIN=\"0135\"");
  delay(1000);
  readWhileAvailable();
  
}

void loop() {
  while(counter < 1) {
  sim7600.println("AT+CMGS=\"+41786939406\"");
  delay(100);
  sim7600.println("Test1\r");
  delay(100);
  sim7600.println((char) 26);
  delay(1000);
  counter++;
  }
}

void readWhileAvailable() {
  while(sim7600.available()) {
    Serial.println(sim7600.readString());
  }
  
}

bool responseEqualsGiven(const char *expected = nullptr) {
  unsigned long timeout = 500;
  unsigned long start = millis();
  while (millis() - start < timeout) {
      String response = sim7600.readString();
      Serial.println(response);
      if (expected && response.indexOf(expected) != -1) {
        return true;
      }
  }
  return false;
}
