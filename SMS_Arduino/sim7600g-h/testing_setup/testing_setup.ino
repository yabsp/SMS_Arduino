#include <SoftwareSerial.h>

#define RX_PIN 7 // Connect to TX of SIM7600, should be 7
#define TX_PIN 8  // Connect to RX of SIM7600, should be 8
#define RI_PIN 2 // SMS interrupt pin

SoftwareSerial sim7600(RX_PIN, TX_PIN);
const char simPin[] = "0135";
volatile bool newMessage = false;

int counter = 1;

enum SimStatus {READY, SIM_PIN, SIM_PUK, UNKNOWN};

bool responseEqualsGiven(const char*);
void readWhileAvailable();
SimStatus checkCPINStatus();
String sendCommandAndGetResponse(const char* command, unsigned long timeout = 1000);


void setup() {
  Serial.begin(4800);        
  sim7600.begin(115200); //initialize at 115200 which is the default of the sim7600g-h module

  pinMode(RI_PIN, INPUT); 
  attachInterrupt(digitalPinToInterrupt(RI_PIN), isrRI, FALLING);

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

  Serial.println("Checking CPIN status...");
  SimStatus cpinStatus = checkCPINStatus();

  switch(cpinStatus) {
    case READY:
      Serial.println("SIM is ready!");
    break;
    case SIM_PIN:
      Serial.println("SIM PIN required. Entering PIN...");
      sim7600.print("AT+CPIN=\"");
      sim7600.print(simPin);
      sim7600.println("\"");
      delay(1000);
      readWhileAvailable();
    break;
    case SIM_PUK:
      Serial.println("SIM PUK required. Cannot proceed.");
    break;
    default:
      Serial.println("CPIN status: " + cpinStatus);
    break;
  }
}

void loop() {

  if(newMessage) {
    newMessage = false;

    sim7600.println("AT+CMGL=\"ALL\"");
    delay(1000);
  
    readWhileAvailable();  
  }

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

SimStatus checkCPINStatus() {
  String response = sendCommandAndGetResponse("AT+CPIN?");
  if (response.indexOf("READY") != -1) {
    return READY;
  } else if (response.indexOf("SIM PIN") != -1) {
    return SIM_PIN;
  } else if (response.indexOf("SIM PUK") != -1) {
    return SIM_PUK;
  }
  return UNKNOWN;
}

String sendCommandAndGetResponse(const char* command, unsigned long timeout) {
  sim7600.println(command);
  unsigned long start = millis();
  String response = "";

  while (millis() - start < timeout) {
    if (sim7600.available()) { 
      char c = sim7600.read();
      response += c;
    }
  }
  return response;
}

void isrRI() {
  newMessage = true;
}
