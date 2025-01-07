#include "ChatHandler.h"
#include <SD.h>

// A structure to hold message details
struct Message {
    String timestamp;
    String phoneNumber;
    String content;
};

struct StoredChat {
    String phoneNumber; // Key
    Message messages[MAX_MESSAGES]; // Messages for this chat
    uint8_t messageCount; // Number of messages
};

StoredChat storedChats[MAX_CHATS]; // Simulate a map
uint8_t storedChatCount = 0; // Current number of stored chats



// initialize variables -----------------------------------------------------------------------
Chat chatList[MAX_CHATS];               // Storage of all chats
uint8_t chatCount = 0;                  // Current amount of chats
uint8_t unknownChatCounter = 0;         // Current amount of unknown chats

extern int testing;

// Add a new chat -----------------------------------------------------------------------------
void newContact(String phoneNumber, String contactName) {
  // Check if the phone number or contact name already exists
    for (uint8_t i = 0; i < chatCount; i++) {
        if (chatList[i].phoneNumber == phoneNumber) {
            Serial.println("Chat with this phone number already exists!");
            return;
        }
        if (chatList[i].contactName == contactName) {
            Serial.println("Chat with this contact name already exists!");
            return;
        }
    }
    // If Storage is not full add
    if (chatCount < MAX_CHATS) {
        chatList[chatCount].phoneNumber = phoneNumber;
        chatList[chatCount].contactName = contactName;
        chatList[chatCount].messageCount = 0; // Initialize with no messages
        chatCount++;
    } else {
        Serial.println("Max chats reached!");
    }
}


// Set the active chat -------------------------------------------------------------------------
void setChat(String phoneNumber, String contactName) {
    uint8_t index = findChatIndexByPhoneNumber(phoneNumber);
    if (index < chatCount) {
        chatList[index].contactName = contactName;
    } else {
        Serial.println("Chat not found. Adding new chat.");
        newContact(phoneNumber, contactName);
    }
}


// Saves a new received message to the memory ----------------------------------------------------
void saveMessageInMemory(const String &phoneNumber, const String &message, const String &timestamp) {
    
  testing++; //for deleting later...

  for (uint8_t i = 0; i < storedChatCount; i++) {
    if (storedChats[i].phoneNumber == phoneNumber) {
      if (storedChats[i].messageCount >= MAX_MESSAGES) {  // MAX_MESSAGES for maximal number of messages in a chat
        for (uint8_t j = 0; j < MAX_MESSAGES - 1; j++) {    // shifts messages for new space
          storedChats[i].messages[j] = storedChats[i].messages[j + 1];
        }
      storedChats[i].messages[MAX_MESSAGES - 1] = {timestamp, phoneNumber, message};  // adds new message at the end
      Serial.println("Oldest message removed. New message added to chat for: " + phoneNumber);
      } else {
        storedChats[i].messages[storedChats[i].messageCount++] = {timestamp, phoneNumber, message}; // adds message, if there is space
        Serial.println("Message added to existing chat for: " + phoneNumber);
      }
    return;
  }
}
  // Create new chat if not found
  if (storedChatCount < MAX_CHATS) {
    storedChats[storedChatCount].phoneNumber = phoneNumber;
    storedChats[storedChatCount].messageCount = 0;
    storedChats[storedChatCount].messages[storedChats[storedChatCount].messageCount++] = {timestamp, phoneNumber, message};
    storedChatCount++;
    Serial.println("New chat created and message added for: " + phoneNumber);
  } else {
      Serial.println("Max chats reached, cannot save message!");
  }
}


// Returns all messages for a specific phonenumber -----------------------------------------------
String getChatMessages(const String &phoneNumber) {
  for (uint8_t i = 0; i < storedChatCount; i++) { // finds the correct chat
    if (storedChats[i].phoneNumber == phoneNumber) {
      String messages = "Chat history for: " + phoneNumber + "\n";
      messages += "--------------------------------\n";

    // Concatenate all messages
    for (uint8_t j = 0; j < storedChats[i].messageCount; j++) {
      messages += storedChats[i].messages[j].timestamp + ": " + storedChats[i].messages[j].phoneNumber + "\n";
      messages += storedChats[i].messages[j].content + "\n";
    }
    messages += "--------------------------------\n";
    return messages;
    }
  }
  return "No chat history found for: " + phoneNumber + "\n"; // If no chat found
}


// Increment unread message counter by phone number ---------------------------------------------
void incrementUnreadMessages(const String& phoneNumber) {
    uint8_t index = findChatIndexByPhoneNumber(phoneNumber);
    if (index < chatCount) {
        chatList[index].unreadMessages++;
        Serial.print("Unread messages for ");
        Serial.print(phoneNumber);
        Serial.print(": ");
        Serial.println(chatList[index].unreadMessages);
    } else {
        Serial.println("Chat not found!");
    }
}


// Retrieve chat by phone number -----------------------------------------------------------------
Chat* getChatByPhoneNumber(const String& phoneNumber) {
    uint8_t index = findChatIndexByPhoneNumber(phoneNumber);
    if (index < chatCount) {
        return &chatList[index];
    } else {
        return nullptr;
    }
}


// Retrieve chat by contact name -----------------------------------------------------------------
Chat* getChatByName(const String& contactName) {
    uint8_t index = findChatIndexByName(contactName);
    if (index < chatCount) {
        return &chatList[index];
    } else {
        return nullptr;
    }
}


// List all chats --------------------------------------------------------------------------------
void listChats() {
    Serial.println("Chat List:");
    for (uint8_t i = 0; i < chatCount; i++) {
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(chatList[i].contactName);
        Serial.print(" (Phone: ");
        Serial.print(chatList[i].phoneNumber);
        Serial.println(")");
    }
}


// Find chat index by phone number ---------------------------------------------------------------
uint8_t findChatIndexByPhoneNumber(const String& phoneNumber) {
    for (uint8_t i = 0; i < chatCount; i++) {
        if (chatList[i].phoneNumber == phoneNumber) {
            return i;
        }
    }
    return chatCount; // Return chatCount as an invalid index
}


// Find chat index by contact name ---------------------------------------------------------------
uint8_t findChatIndexByName(const String& contactName) {
    for (uint8_t i = 0; i < chatCount; i++) {
        if (chatList[i].contactName == contactName) {
            return i;
        }
    }
    return chatCount; // Return chatCount as an invalid index
}
