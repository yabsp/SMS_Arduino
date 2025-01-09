#include "keymaps.h"
#include "KeyboardFlags.h"
#include "KeyboardFunctions.h"

void add_char_to_message(char ch) {
    int len = strlen(message);
    if (len < MAX_MESSAGE_LENGTH) {
        message[len] = ch;
        message[len + 1] = '\0';
    }
}

void delete_last_char_from_message() {
    int len = strlen(message);
    if (len > 0) {
        message[len - 1] = '\0';
    }
}

void handle_enter_key() {
    Serial.println("Message sent:");
    //Serial.println(message);

    //message[0] = '\0';
}
