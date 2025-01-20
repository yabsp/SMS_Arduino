#include "ChatHandler.h"
#include "Utils.h"
#include "Menu.h"


//#include <SD.h>

// A structure to hold message details
struct Message {
  String timestamp;
  String phoneNumber;
  String content;
  /*
  char timestamp[25]; 
  char phoneNumber[16];
  char content[64];
  */
};

struct StoredChat {
  String phoneNumber; // Key
  //char phoneNumer[16];
  //Message messages[MAX_MESSAGES]; // Messages for this chat
  uint8_t messageCount; // Number of messages
};

StoredChat storedChats[MAX_CHATS]; // Simulate a map
uint8_t storedChatCount = 0; // Current number of stored chats


// initialize variables -----------------------------------------------------------------------
Chat chatList[MAX_CHATS];               // Storage of all chats
uint8_t chatCount = 0;                  // Current amount of chats
uint8_t unknownChatCounter = 0;         // Current amount of unknown chats

extern int testing;


// Add a new contact ---------------------------------------------------------------------------
void newContact(String phoneNumber, String contactName) {
  phoneNumber.trim(); // Entfernt führende/nachfolgende Leerzeichen
  Serial.print("Debug: Neue Telefonnummer nach Trim: '");
  Serial.print(phoneNumber);
  Serial.println("'");

  // Check if the phone number already exists in chatList
  for (uint8_t i = 0; i < chatCount; i++) {
    Serial.print("Debug: Vergleich mit gespeicherter Nummer: '");
    Serial.print(chatList[i].phoneNumber);
    Serial.println("'");

    if (chatList[i].phoneNumber == phoneNumber) {
      // Wenn die Nummer gefunden wird, prüfe, ob der Name "Unknown_" ist
      if (chatList[i].contactName.startsWith("Unknown_")) {
        Serial.println("Debug: Aktualisiere existierenden 'Unknown' Kontakt...");
        chatList[i].contactName = contactName;
        Serial.print("Aktualisiert: Telefonnummer: ");
        Serial.print(phoneNumber);
        Serial.print(", Kontaktname: ");
        Serial.println(contactName);
        return;
      } else {
        Serial.println("Debug: Kontakt mit dieser Telefonnummer existiert bereits und hat einen Namen!");
        return;
      }
    }
  }

    // Prüfen, ob der Kontaktname bereits existiert
  for (uint8_t i = 0; i < chatCount; i++) {
    if (chatList[i].contactName == contactName) {
      Serial.println("Debug: Kontakt mit diesem Namen existiert bereits!");
      return;
    }
  }

    // Neuer Kontakt, wenn kein Match gefunden wurde
  if (chatCount < MAX_CHATS) {
    chatList[chatCount].phoneNumber = phoneNumber;
    chatList[chatCount].contactName = contactName;
    chatList[chatCount].messageCount = 0; // Initialisiere ohne Nachrichten
    chatCount++;
    Serial.println("Neuer Kontakt erstellt:");
    Serial.print("Telefonnummer: ");
    Serial.print(phoneNumber);
    Serial.print(", Kontaktname: ");
    Serial.println(contactName);
  } else {
    Serial.println("Maximale Anzahl an Kontakten erreicht!");
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

/*
// Saves a new received message to the memory ----------------------------------------------------
void saveMessageInMemory(const String &phoneNumber, const String &message, const String &timestamp) {
  testing++;
  for (uint8_t i = 0; i < storedChatCount; i++) {
    if (storedChats[i].phoneNumber == phoneNumber) {
      // Add message to existing chat
      if (storedChats[i].messageCount >= MAX_MESSAGES) {
        for (uint8_t j = 0; j < MAX_MESSAGES - 1; j++) {
          storedChats[i].messages[j] = storedChats[i].messages[j + 1];
        }
        storedChats[i].messages[MAX_MESSAGES - 1] = {timestamp, phoneNumber, message};
        Serial.println("Oldest message removed. New message added to chat for: " + phoneNumber);
      } else {
        storedChats[i].messages[storedChats[i].messageCount++] = {timestamp, phoneNumber, message};
        Serial.println("Message added to existing chat for: " + phoneNumber);
      }
      //Draw_Chat_Viewer(phoneNumber, findContactNameByPhoneNumber(phoneNumber));
      Refresh_Chat_Viewer();
    return;
  }
}

  // Create new chat
  if (storedChatCount < MAX_CHATS) {
    storedChats[storedChatCount].phoneNumber = phoneNumber;
    storedChats[storedChatCount].messageCount = 0;
    storedChats[storedChatCount].messages[storedChats[storedChatCount].messageCount++] = {timestamp, phoneNumber, message};
    storedChatCount++;
    Serial.println("New chat created and message added for: " + phoneNumber);

  // Check if the phone number is already in chatList
    if (findChatIndexByPhoneNumber(phoneNumber) == chatCount) {
    // Assign sequential "Unknown" name
      uint8_t unknownIndex = 1;
      for (uint8_t i = 0; i < chatCount; i++) {
        if (chatList[i].contactName.startsWith("Unknown_")) {
          unknownIndex++;
        }
      }
      String unknownName = "Unknown_" + String(unknownIndex);
      newContact(phoneNumber, unknownName);
    }
    //Draw_Chat_Viewer(phoneNumber, findContactNameByPhoneNumber(phoneNumber));
    Refresh_Chat_Viewer();
  } else {
    Serial.println("Max chats reached, cannot save message!");
  }
}
*/


// Returns all messages for a specific phonenumber -----------------------------------------------
/*
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
*/


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


// List all chats (debug)--------------------------------------------------------------------------------
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

// Find chat index by phone number ---------------------------------------------------------------
String findContactNameByPhoneNumber(const String& phoneNumber) {
  for (uint8_t i = 0; i < chatCount; i++) {
    if (chatList[i].phoneNumber == phoneNumber) {
      return chatList[i].contactName;
    }
  }
  return "xx"; // Return chatCount as an invalid index
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

// Creates a new Chat from Serial Input, needs to be changed later (debug, testing) -------------------------------
void createChatFromSerialInput() {
    // Prompt for phone number
    Serial.println("Bitte geben Sie die Telefonnummer ein:");
    String phoneNumber = buildStringFromInput(); // Read phone number input
    Serial.print("Eingegebene Telefonnummer: ");
    Serial.println(phoneNumber); // Debugging: Print entered phone number

    // Prompt for contact name
    Serial.println("Bitte geben Sie den Kontaktnamen ein:");
    String contactName = buildStringFromInput(); // Read contact name input
    Serial.print("Eingegebener Kontaktname: ");
    Serial.println(contactName); // Debugging: Print entered contact name

    // Create new contact
    Serial.println("Versuche, einen neuen Kontakt zu erstellen...");
    newContact(phoneNumber, contactName); // Create the contact

    // Confirm contact creation
    Serial.print("Neuer Kontakt erstellt: ");
    Serial.print("Telefonnummer: ");
    Serial.print(phoneNumber);
    Serial.print(", Kontaktname: ");
    Serial.println(contactName);
}


