#include <SoftwareSerial.h>

#define RX_PIN 7 // Connect to TX of SIM7600
#define TX_PIN 8  // Connect to RX of SIM7600
SoftwareSerial sim7600(RX_PIN, TX_PIN);

const char simPin[] = "0135"; // Replace with your SIM PIN

bool waitForResponse(const char*);

void setup() {
  Serial.begin(115200);         // For Arduino Serial Monitor
  sim7600.begin(115200);        // For SIM7600 communication

  Serial.println("Initializing SIM7600...");
  delay(1000);

  // Send AT command to check connection
  sim7600.println("AT");
  /*Serial.println(sim7600.available());
  Serial.println(sim7600.readString());
  Serial.println(sim7600.available());
  */

  if (waitForResponse("OK")) {
    Serial.println("SIM7600 module is responsive.");
  } else {
    Serial.println("No response from SIM7600. Check connections.");
    return; // Stop setup if the module doesn't respond
  }

  // Check SIM PIN status
  sim7600.println("AT+CPIN?");
  if (waitForResponse("SIM PIN")) {
    // Send SIM PIN if required
    sim7600.print("AT+CPIN=\"");
    sim7600.print(simPin);
    sim7600.println("\"");
    if (waitForResponse("OK")) {
      Serial.println("SIM PIN accepted.");
    } else {
      Serial.println("Failed to unlock SIM. Check PIN.");
      return; // Stop setup if the PIN is incorrect
    }
  } else if (waitForResponse("READY")) {
    Serial.println("SIM is already unlocked.");
  }

  // Check if the SIM is ready
  sim7600.println("AT+CPIN?");
  if (waitForResponse("+CPIN READY")) {
    Serial.println("SIM is ready and connection established.");
  } else {
    Serial.println(sim7600.readString());
    Serial.println("Failed to initialize SIM. Check SIM card.");
  }
}

void loop() {
  // Your main code here
}

bool waitForResponse(const char *expected = nullptr) {
  unsigned long timeout = 5000;
  unsigned long start = millis();
  while (millis() - start < timeout) {
      String response = sim7600.readString();
      Serial.println(response); // Echo response to Serial Monitor
      if (expected && response.indexOf(expected) != -1) {
        return true;
      }
  }
  return false;
}
