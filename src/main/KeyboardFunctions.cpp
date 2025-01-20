#include "keymaps.h"
#include "KeyboardFlags.h"
#include "KeyboardFunctions.h"

void add_char_to_message(char ch) {
    if (message.length() < MAX_MESSAGE_LENGTH) {
        message += ch;
    }
}

void delete_last_char_from_message() {
    if (message.length() > 0) {
        message.remove(message.length() - 1);
    }
}

void handle_enter_key() {
    //Serial.println("Message sent:");
    //Serial.println(message);

    //message[0] = '\0';
}
