#include "SDReaderWriter.h"
#include "Menu.h"


void setupSDAndFolderStruct(){
  if (!SD.begin(CHIPSELECT, SD_SCK_MHZ(50))) {
    Serial.println("SdFat initialization failed!");
    return;
  } else {
    Serial.println("SdFat initialized.");
  }

  if (!SD.exists("/contacts/")) {
    SD.mkdir("/contacts/");
  } 
}

void storeMessage(const char* phoneNumber, const char* timestamp, const char* messageContent) {

  String filePath = String("/contacts/") + String(phoneNumber);

  if (!SD.exists(filePath)) {
    if (SD.mkdir(filePath)) {
      Serial.println("contacts/phoneNumber directory created");
    } else {
      Serial.println("Failed to create contacts/phoneNumber directory");
      return;
    }
  }

  filePath = filePath + String("/msgs/");

  if (!SD.exists(filePath)) {
    if (SD.mkdir(filePath)) {
      Serial.println("Message directory created");
    }else {
    Serial.println("Could not create messages directory");
    } 
  } 

  filePath = filePath + String(timestamp);

  SdFile file;
  if (file.open(filePath.c_str(), O_WRITE | O_CREAT)) {
    file.println(messageContent);
    file.close();
    Serial.println("Message created: " + filePath);
  } else {
    Serial.println("Failed to create message: " + filePath);
  }
}

void storeContact(const char *phoneNumber, const char *contactName) {

    String folderPath = String("/contacts/") + phoneNumber +"/";
    
    if (!SD.exists(folderPath)) {
      SD.mkdir(folderPath);
    }

    folderPath += String("/info");
    SdFile file;
    if (file.open(folderPath.c_str(), O_WRITE | O_CREAT)) {
      file.println(contactName);
      file.close();
      Serial.println("Contact stored: " + String(contactName));
    }
}

void loadMessages(String phoneNumber, int startIndex, int messageAmount) {
  SdFile dir;
  String path = "/contacts/" + phoneNumber + "/msgs/";

  Serial.println("Attempting to open file: " + path);

  if (!dir.open(path.c_str())) {
    Serial.println("Failed to open messages directory.");
  } else if (startIndex < 0){
    return;
  }

  Serial.println("Iterating through messages:");
  SdFile file;
  
  int offset = startIndex;
  while (file.openNext(&dir, O_RDONLY) && (offset - startIndex < messageAmount)) {
    char fileName[50];
    file.getName(fileName, sizeof(fileName));
    Serial.print("Message ");
    Serial.print(offset);
    Serial.print(": ");
    Serial.println(fileName);

    char buffer[MAX_MESSAGE_LENGTH]; 
    while (file.available()) {
      size_t bytesRead = file.fgets(buffer, sizeof(buffer));
      if (bytesRead > 0) {
        Serial.print(buffer); 
      }
  }

    file.close();
    offset++;
    
    tft.setCursor(message_Cursor_X, message_Cursor_Y);
    // Display timestamp + name
    message_Cursor_X += 8;
    message_Cursor_Y += 8;
    
    tft.setCursor(message_Cursor_X, message_Cursor_Y);
    // Display message
    message_Cursor_X += 8;
    message_Cursor_Y += 8;

  }
  dir.close();
}

void loadContacts() {
  SdFile contactsDir;

  if (!contactsDir.open("/contacts/")) {
    Serial.println("Failed to open /contacts directory.");
  }

  SdFile entry;
  while (entry.openNext(&contactsDir, O_RDONLY)) {
    if (entry.isDir()) {
    
      char dirName[50];
      entry.getName(dirName, sizeof(dirName));
      Serial.println("Contact Directory: " + String(dirName));

      String infoFilePath = String("/contacts/") + String(dirName) + "/info";
      SdFile infoFile;
      if (infoFile.open(infoFilePath.c_str(), O_RDONLY)) {
        char infoContent[100];
        infoFile.fgets(infoContent, sizeof(infoContent));
        infoFile.close();

// Optional, trims first and last name
        String infoLine = String(infoContent);
        int spaceIndex = infoLine.indexOf(' ');
        String firstName = infoLine.substring(0, spaceIndex);
        String lastName = infoLine.substring(spaceIndex + 1);
        lastName.trim();

        Serial.print("Info: ");
        Serial.println(infoContent);
//
      } else {
        Serial.println("Failed to open info file: " + infoFilePath);
      }
    }
    entry.close();
  }

  contactsDir.close();
}

void getNameByPhoneNumber(const char* phoneNumber) {
  bool contactFound = false;

  SdFile contactsDir;

  if (!contactsDir.open("/contacts/")) {
    Serial.println("Failed to open /contacts directory.");
  }

  SdFile entry;
  while (entry.openNext(&contactsDir, O_RDONLY) && contactFound == false) {
    if (entry.isDir()) {

      char dirName[50];
      entry.getName(dirName, sizeof(dirName));
      String dirNameString = String(dirName);
      Serial.println("Contact Directory: " + String(dirName));

      if (dirNameString.equals(String(phoneNumber))) {
        String infoFilePath = String("/contacts/") + dirNameString + "/info";
        SdFile infoFile;

        if (infoFile.open(infoFilePath.c_str(), O_RDONLY)) {
          contactFound = true;
          char contactName[100];
          infoFile.fgets(contactName, sizeof(contactName));
          infoFile.close();

  // Optional, trims first and last name
          String contactNameString = String(contactName);
          int spaceIndex = contactNameString.indexOf(' ');
          String firstName = contactNameString.substring(0, spaceIndex);
          String lastName = contactNameString.substring(spaceIndex + 1);
          lastName.trim();

          Serial.print("Info: ");
          Serial.println(contactName);
        }
      }
    }
    entry.close();
  }
   contactsDir.close();
}

int getStoredMessagesCount(const char* phoneNumber) {
  SdFile dir;
  String path = "/contacts/" + String(phoneNumber) + "/msgs/";
  int count = 0;

  Serial.println("Attempting to open file: " + path);

  if (!dir.open(path.c_str())) {
    Serial.println("Failed to open messages directory.");
    return count;
  }

  Serial.println("Iterating through messages:");
  SdFile file;
  
  while (file.openNext(&dir, O_RDONLY)) {
    count++;
  }
  dir.close();
  return count;
}