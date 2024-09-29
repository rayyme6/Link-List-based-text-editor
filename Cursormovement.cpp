#include <iostream>
#include <Windows.h>
#include "TextStorage.h"

using namespace std;

// Function to set the cursor position in the console
void gotoxy(int x, int y) {
    COORD c = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// Function to clear the screen and re-render the text after changes
void renderText(TextStorage& textStorage) {
    system("cls"); // Clear the screen

    // Traverse the linked list from the head and print all the characters
    TextNode* line = textStorage.gethead();
    while (line != nullptr) {
        TextNode* temp = line;
        while (temp != nullptr) {
            cout << temp->data; // Output the character
            temp = temp->right;
        }
        cout << endl; // Move to the next line
        line = line->down; // Move to the next line in the linked list
    }
}

int main(int argc, char* argv[]) {
    system("cls"); // Clear the console at the start
    HANDLE rhnd = GetStdHandle(STD_INPUT_HANDLE);  // handle to read console

    DWORD Events = 0;     // Event count
    DWORD EventsRead = 0; // Events read from console

    bool Running = true;

    int x = 0, y = 0; // Cursor position
    gotoxy(x, y);

    // Initialize TextStorage object to manage the text
    TextStorage textStorage;

    const int fixedWidth = 100; // Fixed line width

    // Program's main loop
    while (Running) {
        // Gets the system's current "event" count
        GetNumberOfConsoleInputEvents(rhnd, &Events);

        if (Events != 0) { // If something happened, handle the events
            // Create event buffer for the events
            INPUT_RECORD eventBuffer[200];

            // Fills the event buffer with the events and saves count in EventsRead
            ReadConsoleInput(rhnd, eventBuffer, Events, &EventsRead);

            // Loop through the event buffer using the saved count
            for (DWORD i = 0; i < EventsRead; ++i) {
                // Check if the event is a key event && if so, it is a press not a release
                if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown) {
                    // Check if the key press was an arrow key
                    switch (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode) {
                    case VK_UP: // Up arrow
                        y = max(0, y - 1); // Prevent moving above the console
                        break;
                    case VK_DOWN: // Down arrow
                        y++;
                        break;
                    case VK_RIGHT: // Right arrow
                        x++;
                        break;
                    case VK_LEFT: // Left arrow
                        x = max(0, x - 1); // Prevent moving left beyond console limits
                        break;
                    case VK_ESCAPE: // Escape key to exit
                        Running = false;
                        break;
                    default:
                        // If the input is a character
                        if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar != 0) {
                            char inputChar = eventBuffer[i].Event.KeyEvent.uChar.AsciiChar;

                            // Handle newline character
                            if (inputChar == '\n' || inputChar == '\r') {
                                x = 0; // Reset x to the beginning
                                y++; // Move cursor to the next line
                            }
                            else {
                                // Insert the character into the TextStorage object at (x, y)
                                textStorage.insertChar(inputChar, x, y);

                                // Increment x and check for word wrap
                                if (++x >= fixedWidth) {
                                    x = 0; // Move to the next line if fixed width is exceeded
                                    y++;
                                }
                            }

                            // Clear the console and re-render the text after insertion
                            renderText(textStorage);

                            // Move the cursor to the updated position
                            gotoxy(x, y);
                        }
                        break;
                    }
                }
            } // End EventsRead loop
        }
    } // End program loop

    return 0;
}