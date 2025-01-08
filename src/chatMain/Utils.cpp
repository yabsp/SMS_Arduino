#include "Utils.h"

// Calculates the left position of a button given its width
int calculateLeft(int width) {
    // Half the width to get the left edge
    return width / 2;
}

// Calculates the right position of a button given its width
int calculateRight(int width) {
    // Subtract the half of the width to get the right edge
    return 0 - (width / 2);
}

// Calculates the top position of a button given its height
int calculateTop(int height) {
    // Half the height to get the top edge
    return height / 2;
}

// Calculates the bottom position of a button given its height
int calculateBottom(int height) {
    // Subtract the half of the height to get the bottom edge
    return 0 - (height / 2);
}


// Method to build a string from keyboard input
String buildStringFromInput() {
    String inputString = "";    // Temporary string to store the input
    bool inputComplete = false; // Flag to track when Enter is pressed

    while (!inputComplete) {
        if (Serial.available()) {
            char key = Serial.read(); // Read a character from the serial buffer

            switch (key) {
                case '\b': // Backspace
                    if (inputString.length() > 0) {
                        inputString.remove(inputString.length() - 1); // Remove last character
                        Serial.print("\b \b"); // Simulate backspace on serial monitor
                    }
                    break;

                case '\n': // Enter key
                    inputComplete = true; // Mark input as complete
                    Serial.println(); // Move to the next line
                    break;

                default:
                    if (inputString.length() < MAX_INPUT_LENGTH) {
                        inputString += key; // Append character to string
                        Serial.print(key);  // Echo character to serial monitor
                    } else {
                        Serial.println("\nMax input length reached!");
                    }
                    break;
            }
        }
    }
    return inputString; // Return the completed string
}


