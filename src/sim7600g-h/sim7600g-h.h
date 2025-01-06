#ifndef SIM7600G_H
#define SIM7600G_H

#include <SoftwareSerial.h>
#include <Arduino.h>

#define RX_PIN 7
#define TX_PIN 8
#define RI_PIN 2

extern SoftwareSerial sim7600;
extern volatile bool newMessage;
extern const char simPin[];

enum SimStatus {READY, SIM_PIN, SIM_PUK, UNKNOWN};

void setupSim7600();
void loopSim7600();
void readWhileAvailableMessage();
void displayLastMessage(String rawResponse);
bool responseEqualsGiven(const char *expected);
SimStatus checkCPINStatus();
String sendCommandAndGetResponse(const char *command, unsigned long timeout = 1000);
void readWhileAvailable();
void isrRI();
bool sendSMS(const String &phoneNumber, const String &message);

#endif
