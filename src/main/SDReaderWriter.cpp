#include "SDReaderWriter.h"
#include "Menu.h"

extern volatile bool sdCardBusy = false;

void setupSDAndFolderStruct(){
  sdCardBusy = true;
  if (!SD.begin(CHIPSELECT, SD_SCK_MHZ(50))) {
    Serial.println("SdFat initialization failed!");
    sdCardBusy = false;
    return;
  } else {
    Serial.println("SdFat initialized.");
  }

  if (!SD.exists("/contacts/")) {
    SD.mkdir("/contacts/");
  }
  sdCardBusy = false; 
}

void storeMessage(const char* phoneNumber, const char* timestamp, const char* messageContent) {
  sdCardBusy = true;
  String filePath = String("/contacts/") + String(phoneNumber) + String("/");

  if (!SD.exists(filePath)) {
    if (SD.mkdir(filePath)) {
      Serial.println("contacts/phoneNumber directory created");
    } else {
      Serial.println("Failed to create contacts/phoneNumber directory");
      sdCardBusy = false;
      return;
    }
  }

  filePath = filePath + String("msgs/");

  if (!SD.exists(filePath)) {
    if (SD.mkdir(filePath)) {
      Serial.println("Message directory created");
    } else {
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
  sdCardBusy = false;
}

void storeContact(const char *phoneNumber, const char *contactName) {
    sdCardBusy = true;
    String folderPath = String("/contacts/") + String(phoneNumber) + "/";
    
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
    sdCardBusy = false;
}

void loadMessages(String phoneNumber, int startIndex, int messageAmount) {
  sdCardBusy = true;
  SdFile dir;
  String path = String("/contacts/") + phoneNumber + String("/msgs/");

  Serial.println("Attempting to open file: " + path);

  if (!dir.open(path.c_str())) {
    Serial.println("Failed to open messages directory.");
    sdCardBusy = false;
    return;
  } else if (startIndex < 0){
    sdCardBusy = false;
    return;
  }

  Serial.println("Iterating through messages:");
  SdFile file;

  int count = 0;
  while (file.openNext(&dir, O_RDONLY)) {
    if (count >= startIndex && count < startIndex + messageAmount) {
      char fileName[50];
      file.getName(fileName, sizeof(fileName));
      Serial.print("Message ");
      Serial.print(count);
      Serial.print(": ");
      Serial.println(fileName);

      char buffer[MAX_MESSAGE_LENGTH]; 
      while (file.available()) {
        size_t bytesRead = file.fgets(buffer, sizeof(buffer));
        if (bytesRead > 0) {
          Serial.print(buffer); 
        }
      }
      
      if (String(fileName).endsWith("_0")) { // incoming message

        Serial.println("in case _0");

        tft.setTextColor(BLACK);

        tft.setCursor(message_Cursor_X, message_Cursor_Y);
        // Display timestamp + name
        tft.print(formatDateString(String(fileName)) + ": "  + getNameByPhoneNumber(phoneNumber.c_str()));
        //message_Cursor_X += 8;
        message_Cursor_Y += 12;
        
        tft.setCursor(message_Cursor_X, message_Cursor_Y);
        // Display message
        tft.print(buffer);
        //message_Cursor_X += 10;
        message_Cursor_Y += 15;

      } else if (String(fileName).endsWith("_1")) { // outgoing messages

        Serial.println("in case _1");

        tft.setTextColor(BLACK);
        tft.fillRect(0, message_Cursor_Y - 3, 320, 27, LIGHTGREY);

        tft.setCursor(message_Cursor_X, message_Cursor_Y);
        // Display timestamp + name
        tft.print(formatDateString(String(fileName)) + ": "  + "You");
        //message_Cursor_X += 8;
        message_Cursor_Y += 12;
        
        tft.setCursor(message_Cursor_X, message_Cursor_Y);
        // Display message
        tft.print(buffer);
        //message_Cursor_X += 10;
        message_Cursor_Y += 15;

      }
      file.close();
      count++;
    } else {
      file.close();
      count++;
    }
  }
  dir.rewind();
  dir.close();
  sdCardBusy = false;
}

void loadContacts() {
  sdCardBusy = true;
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
  sdCardBusy = false;
}

String getNameByPhoneNumber(const char* phoneNumber) {
  sdCardBusy = true;
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
          sdCardBusy = false;
          return String(contactName);

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
    entry.rewind();
    entry.close();
  }
  contactsDir.rewind();
  contactsDir.close();
  sdCardBusy = false;
  return "";
}

int getStoredMessagesCount(const char* phoneNumber) {
  sdCardBusy = true;
  SdFile dir;
  String path = "/contacts/" + String(phoneNumber) + "/msgs/";
  int count = 0;

  Serial.println("Attempting to open file: " + path);

  if (!dir.open(path.c_str())) {
    Serial.println("Failed to open messages directory.");
    sdCardBusy = false;
    return count;
  }

  Serial.println("Iterating through messages:");
  SdFile file;
  
  while (file.openNext(&dir, O_RDONLY)) {
    count++;
    file.close();
  }
  dir.rewind();
  dir.close();
  sdCardBusy = false;
  return count;
}

String formatDateString(String filename) {

  if (filename.endsWith("_0") || filename.endsWith("_1")) {
    // Remove the "_0" or "_1" part
    filename = filename.substring(0, filename.length() - 2);

    // Extract the date and time parts
    String datePart = filename.substring(0, 10);  // "2025-01-03"
    String timePart = filename.substring(11, 19); // "14-30-00"

    // Reformat the date
    String day = datePart.substring(8, 10);  // "03"
    String month = datePart.substring(5, 7); // "01"
    String year = datePart.substring(0, 4);  // "2025"

    // Reformat the time
    String hours = timePart.substring(0, 2); // "14"
    String minutes = timePart.substring(3, 5); // "30"

    // Combine into the final format
    String formattedString = day + "." + month + "." + year + ", " + hours + ":" + minutes;
    return formattedString;
  }

  // If not ending with "_0" or "_1", return an empty string or error
  return "";
}


int getContactsCount() {
  sdCardBusy = true;
  SdFile dir, entry;
  
  int counter = 0;

  if (!dir.open("/contacts/")) {
    Serial.println("Failed to open directory: " + String("/contacts/"));
    sdCardBusy = false;
    return 0;
  }

  while (entry.openNext(&dir, O_RDONLY)) {
    if (entry.isDir()) { 
      counter++;
    }
    entry.close();
  }
  dir.rewind();
  dir.close();
  sdCardBusy = false;
  return counter;
}


Chat getContactByIndex(int index) {
  sdCardBusy = true;
  SdFile dir, entry;
  int currentIndex = 0;
  Chat contact = {"", ""};

  if (!dir.open("/contacts/")) {
    Serial.println("Failed to open directory: " + String("/contacts/"));
    sdCardBusy = false;
    return contact;
  }

  while (entry.openNext(&dir, O_RDONLY)) {
    if (entry.isDir()) { 
      if (currentIndex == index) {
        char dirName[50];
        entry.getName(dirName, sizeof(dirName));
        contact.phoneNumber = String(dirName);

        String infoFilePath = String("/contacts/") + String(dirName) + "/info";
        SdFile infoFile;
        if (infoFile.open(infoFilePath.c_str(), O_RDONLY)) {
          char name[100];
          infoFile.fgets(name, sizeof(name)); 
          String nameString = String(name);
          nameString.trim();
          contact.contactName = nameString;
          infoFile.close();
        } else {
          Serial.println("Failed to open info file: " + infoFilePath);
        }
        entry.rewind();
		    dir.rewind();
        entry.close();
        dir.close();
        sdCardBusy = false;
        return contact;
      }
      currentIndex++;
    }
    entry.close();
  }
  dir.rewind();
  dir.close();
  sdCardBusy = false;
  return contact;
}


String getPhoneNumberByIndex(int index) {
  sdCardBusy = true;
  SdFile dir, entry;
  int currentIndex = 0;

  if (!dir.open("/contacts/")) {
    Serial.println("Failed to open directory: " + String("/contacts/"));
    sdCardBusy = false;
    return "";
  }

  while (entry.openNext(&dir, O_RDONLY)) {
    if (entry.isDir()) { 
      if (currentIndex == index) {
        char dirName[50];
        entry.getName(dirName, sizeof(dirName));
        String phoneNumber = String(dirName);

        entry.close();
        dir.close();
        sdCardBusy = false;
        return phoneNumber;
      }
      currentIndex++;
    }
    entry.close();
  }
  dir.close();
  sdCardBusy = false;
  return "";
}

String getPhoneNumberAndContactNameByIndex(int index) {
  sdCardBusy = true;
  SdFile dir, entry;
  int currentIndex = 0;

  if (!dir.open("/contacts/")) {
    Serial.println("Failed to open directory: " + String("/contacts/"));
    sdCardBusy = false;
    return "";
  }

  while (entry.openNext(&dir, O_RDONLY)) {
    if (entry.isDir()) { 
      if (currentIndex == index) {
        char dirName[50];
        entry.getName(dirName, sizeof(dirName));
        String phoneNumber = String(dirName);

        String infoFilePath = String("/contacts/") + String(dirName) + "/info";
        SdFile infoFile;
        String contactName = "";

        if (infoFile.open(infoFilePath.c_str(), O_RDONLY)) {
          char name[100];
          infoFile.fgets(name, sizeof(name)); 
          contactName = String(name);
          contactName.trim();
          infoFile.close();
        } else {
          Serial.println("Failed to open info file: " + infoFilePath);
        }
        entry.rewind();
		    dir.rewind();
        entry.close();
        dir.close();
        sdCardBusy = false;

        return phoneNumber + "_" + contactName;
      }
      currentIndex++;
    }
    entry.close();
  }
  dir.rewind();
  dir.close();
  sdCardBusy = false;
  return "";
}