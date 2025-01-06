#include "ChatHandler.h"

// initialize variables -----------------------------------------------------------------------
Chat chatList[MAX_CHATS];               // Storage of all chats
uint8_t chatCount = 0;                  // Current amount of chats
uint8_t unknownChatCounter = 0;         // Current amount of unknown chats

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


// Add a message to a chat ----------------------------------------------------------------------
bool addMessageToChat(const String& phoneNumber, const String& message) {
    uint8_t index = findChatIndexByPhoneNumber(phoneNumber);
    if (index >= chatCount) {
        // Add unknown chat if not found
        String unknownName = "Unknown_" + String(++unknownChatCounter);
        newContact(phoneNumber, unknownName);
        index = findChatIndexByPhoneNumber(phoneNumber); // Recalculate the index after adding
    }

    Chat& chat = chatList[index];
    if (chat.messageCount < MAX_MESSAGES) {
        chat.messages[chat.messageCount] = message;
        chat.messageCount++;
        return true;
    } else {
        Serial.println("Max messages reached for this chat!");
        return false;
    }
}


// Send a message -------------------------------------------------------------------------------
bool sendMessage(const String& phoneNumber, const String& message) {
    uint8_t index = findChatIndexByPhoneNumber(phoneNumber);
    if (index < chatCount) {
        Serial.print("Sending message to ");
        Serial.print(chatList[index].contactName);
        Serial.print(" (");
        Serial.print(phoneNumber);
        Serial.println(")...");

        // Simulate sending the message
        bool success = true; // Assume the message is sent successfully

        if (success) {
            Serial.println("Message sent successfully!");
            return addMessageToChat(phoneNumber, message); // Add to chat history
        } else {
            Serial.println("Failed to send message.");
            return false;
        }
    } else {
        Serial.println("Chat not found. Adding a new chat.");
        newContact(phoneNumber, "Unknown");
        return sendMessage(phoneNumber, message); // Retry sending the message
    }
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
