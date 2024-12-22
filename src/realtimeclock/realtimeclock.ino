#include <Wire.h> // Library for I2C communication protocol
#include <RTClib.h> // Library for DS3231 RTC module

RTC_DS3231 rtc;          // Create an RTC object

int lastPrintedDay = -1;   // Tracks the last printed day
int lastPrintedMonth = -1; // Tracks the last printed month
int lastPrintedYear = -1;  // Tracks the last printed year

void setup() {
  Serial.begin(115200); // Start Serial Monitor

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC to compile time
  }
}

void loop() {
  // Example message for testing
  printChatMessage("This is a test message.");
  delay(5000);
}

// Print time infront of a string (date is printed if it is different than from the last message)
void printChatMessage(const char* message) {
  DateTime now = rtc.now(); // Get the current time

  // Check if the date has changed
  if (now.year() != lastPrintedYear || now.month() != lastPrintedMonth || now.day() != lastPrintedDay) {
    // Print the date
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.println(now.day(), DEC);

    // Update the last printed date
    lastPrintedYear = now.year();
    lastPrintedMonth = now.month();
    lastPrintedDay = now.day();
  }

  // Print the time and message
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(" - ");
  Serial.println(message);
}
