#include "sim7600g-h.h"
#include "KeyboardFlags.h"

//SoftwareSerial sim7600(RX_PIN, TX_PIN);
volatile bool newMessage = false;
const char simPin[] = "0135";
const String const_phoneNumber = "+41794410255";
int counter = 1;

void setupSim7600() {

    sim7600.begin(115200);

    pinMode(RI_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(RI_PIN), isrRI, FALLING);

    Serial.println("Initializing SIM7600...");
    delay(500);

    // Change baud rate to 9600
    int count = 0;
    do {
        Serial.println("Configuring baud rate to 9600, please wait...");
        sim7600.println("AT+IPREX=9600");
        delay(100);
        sim7600.begin(9600);
        count++;
    } while (!responseEqualsGiven("OK") && count < 4);

    // Checking Connection
    Serial.println("Checking Connection");
    sim7600.println("AT");
    delay(200);
    readWhileAvailable();

    // Checks if SIM card is unlocked, if not, it unlocks it
    Serial.println("Checking CPIN status...");
    SimStatus cpinStatus = checkCPINStatus();

    switch (cpinStatus) {
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
            Serial.println("CPIN status: " + String(cpinStatus));
            break;
    }
    readWhileAvailable();
}

void loopSim7600() {
  if (newMessage) {
      newMessage = false;
      sim7600.println("AT+CMGL=\"REC UNREAD\"");
      readWhileAvailableMessage();
  }
  if (enterKeyPressed) {
    Serial.println(message);
    enterKeyPressed = false; // Reset the flag
    sendSMS(const_phoneNumber);
    message = "";
   }
}

void readWhileAvailableMessage() {
    String rawResponse = "";
    while (!sim7600.available()){
      Serial.println("waiting");
    }
    while (sim7600.available()) {
        rawResponse = sim7600.readString();
    }
    displayLastMessage(rawResponse);

}


void displayLastMessage(String rawResponse) {
  Serial.println("You received a new message:");
  int currentIndex = 0;
  Serial.println("RAW:" + rawResponse);

  String lastPhoneNumber = "";
  String lastDate = "";
  String lastTime = "";
  String lastMessageContent = "";

  while (currentIndex < rawResponse.length()) {
    int cmglIndex = rawResponse.indexOf("+CMGL:", currentIndex);
    if (cmglIndex == -1) break;

    int headerEnd = rawResponse.indexOf('\n', cmglIndex);
    if (headerEnd == -1) break;
    String headerLine = rawResponse.substring(cmglIndex, headerEnd);

    String phoneNumber = headerLine.substring(23, 35);
    String date = headerLine.substring(41, 49);
    String time = headerLine.substring(50, 61);

    int messageStart = headerEnd + 1;
    int nextHeader = rawResponse.indexOf("+CMGL:", messageStart);
    int messageEnd = (nextHeader != -1) ? nextHeader - 1 : rawResponse.length();
    String messageContent = rawResponse.substring(messageStart, messageEnd);

    lastPhoneNumber = phoneNumber;
    lastDate = date;
    lastTime = time;
    lastMessageContent = messageContent;

    currentIndex = messageEnd + 1;
  }

  Serial.println("Last Message Details:");
  Serial.println("Phone: " + lastPhoneNumber);
  Serial.println("Date: " + lastDate);
  Serial.println("Time: " + lastTime);
  Serial.println("Message: " + lastMessageContent);
  Serial.println("-------------------------");
}


bool responseEqualsGiven(const char *expected) {
    unsigned long timeout = 500;
    unsigned long start = millis();
    String response = "";

    while (millis() - start < timeout) {
        if (sim7600.available()) {
            response += sim7600.readString();
            if (response.indexOf(expected) != -1) {
                return true;
            }
        }
    }
    Serial.println(response);
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

String sendCommandAndGetResponse(const char *command, unsigned long timeout) {
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

void readWhileAvailable() {
    while (sim7600.available()) {
        Serial.println(sim7600.readString());
    }
}

void isrRI() {
    newMessage = true;
}

bool sendSMS(const String &phoneNumber) {
  counter++;

  Serial.println("Checking Connection...");
  sim7600.println("AT");
  delay(200);
  if (!responseEqualsGiven("OK")){
    Serial.println("There is no connection");
    return false;
  }
  Serial.println("OK");

  Serial.println("Sending SMS..."); 
  
  sim7600.println("AT+CMGF=1");   // Set SMS to text mode
  delay(100);
  if (!responseEqualsGiven("OK")){
    Serial.println("Failed to set SMS mode to text.");
    return false;
  } 

  Serial.println("Message in method: " + message);


  sim7600.print("AT+CMGS=\"");
  sim7600.print(phoneNumber);
  sim7600.println("\"");
  delay(300);

  sim7600.print(message);

  /*for (unsigned int i = 0; i < message.length(); i++) {
    sim7600.print(message[i]);
    delay(10); // Small delay to avoid buffer overflow
  }
  */

  delay(300);

  sim7600.write(26); // Send termination character
  delay(10000);


  if (responseEqualsGiven("OK")) {
      Serial.println("SMS sent successfully.");
      return true;
  } else {
      Serial.println("Failed to send SMS.");
      return false;
  }
}
