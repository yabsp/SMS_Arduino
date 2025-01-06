#ifndef CHATHANDLER_H
#define CHATHANDLER_H

#include <Arduino.h>
#include "Config.h"

// Structs ------------------------------------------------------------------------------------
struct Chat {
  String phoneNumber;               // Phone number of the chat contact
  String contactName;               // Name of the contact
  String messages[MAX_MESSAGES];    // Array to store chat messages
  uint8_t messageCount;             // Number of messages in the chat
  uint8_t unreadMessages;           // Counter for unread messages
};

// Global variables ---------------------------------------------------------------------------
extern Chat chatList[MAX_CHATS]; // Storage for chat data
extern uint8_t chatCount; // Counter for amount of overall chats
extern uint8_t unknownChatCounter; // Counter for unknown chats

// Function prototyping -----------------------------------------------------------------------
void newContact(String phoneNumber, String contactName);
void setChat(String phoneNumber, String contactName);
bool addMessageToChat(const String& phoneNumber, const String& message);
bool sendMessage(const String& phoneNumber, const String& message);
Chat* getChatByPhoneNumber(const String& phoneNumber);
Chat* getChatByName(const String& contactName);
void listChats();
uint8_t findChatIndexByPhoneNumber(const String& phoneNumber);
uint8_t findChatIndexByName(const String& contactName);

#endif
