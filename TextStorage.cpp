#include "TextStorage.h"

void TextStorage::wrapWord(TextNode* temp)
{
    int countwrap = 1; // To count the number of characters we need to wrap around

    // First, we traverse temp backwards till we find the space or start of the line
    while (temp->left != nullptr && temp->left->data != ' ')
    {
        temp = temp->left;
        countwrap++;
    }

    // Now we have to unlink the current word from the line
    TextNode* wordstart = temp;
    TextNode* prev = wordstart->left; // Unlink the wordstart from the previous part of the line
    if (prev != nullptr) {
        prev->right = nullptr; // Ensure the previous node's right pointer is set to nullptr
    }

    // Traverse to the first node of the current line
    while (temp->left != nullptr) {
        temp = temp->left;
    }

    // Create the first node of the new line with the first character of the word
    TextNode* newLineHead = new TextNode;
    newLineHead->data = wordstart->data;
    newLineHead->right = nullptr;
    newLineHead->left = nullptr;
    newLineHead->down = nullptr;

    // Link the first node of the current line to the new line
    TextNode* firstNodeOfLine = temp;
    firstNodeOfLine->down = newLineHead;
    newLineHead->up = firstNodeOfLine;

    // Move to the next character in the word
    wordstart = wordstart->right;
    countwrap--;

    // Now, create the rest of the nodes for the wrapped word in the new line
    TextNode* newLineTemp = newLineHead;
    while (countwrap > 0 && wordstart != nullptr)
    {
        TextNode* newNode = new TextNode;
        newNode->data = wordstart->data;
        newNode->right = nullptr;   // It will be linked later
        newNode->left = newLineTemp; // Link to the previous node in the new line
        newNode->down = nullptr;
        newNode->up = nullptr;

        newLineTemp->right = newNode; // Link the previous node to the current node
        newLineTemp = newNode;        // Move to the next node in the new line

        wordstart = wordstart->right; // Move to the next character in the original word
        countwrap--;
    }
}
void TextStorage::createNewLine()
{
    // Create the head of the new line
    TextNode* newLineHead = new TextNode;
    newLineHead->data = ' ';  // Start with a space
    newLineHead->right = nullptr;
    newLineHead->left = nullptr;
    newLineHead->down = nullptr;

    // Link the new line to the last line in the text storage
    if (head == nullptr) // If there's no head, this is the first line
    {
        head = newLineHead;
        newLineHead->up = nullptr; // First line has no line above
    }
    else
    {
        // Traverse to the last line
        TextNode* lastLine = head;
        while (lastLine->down != nullptr)
        {
            lastLine = lastLine->down;
        }

        // Link the new line to the last line
        lastLine->down = newLineHead; // Link down
        newLineHead->up = lastLine;    // Link up to the last line
    }

    // Create the rest of the spaces for the new line
    TextNode* currentNode = newLineHead;

    // Traverse the last line to link the up pointers correctly
    TextNode* lastLineNode = head;
    while (lastLineNode->down != nullptr)
    {
        lastLineNode = lastLineNode->down;
    }

    for (int i = 1; i < maxLineLength; ++i)
    {
        TextNode* newSpaceNode = new TextNode;
        newSpaceNode->data = ' ';  // Fill with space
        newSpaceNode->right = nullptr;
        newSpaceNode->left = currentNode;
        newSpaceNode->down = nullptr;

        // Link the current node to the new space node
        currentNode->right = newSpaceNode;

        // Find the node in the last line to link up
        TextNode* correspondingNode = lastLineNode;
        for (int j = 0; j < i; ++j) {
            if (correspondingNode != nullptr) {
                correspondingNode = correspondingNode->right;
            }
        }

        // Set the up pointer if the corresponding node exists
        if (correspondingNode != nullptr) {
            newSpaceNode->up = correspondingNode; // Link to the corresponding node in the last line
        }
        else {
            newSpaceNode->up = nullptr; // No corresponding node (in case of uneven line lengths)
        }

        currentNode = newSpaceNode;  // Move to the new node
    }
}
TextStorage::TextStorage()
{
	head = nullptr;
	cursorX = 0;
	cursorY = 0;
	maxLineLength = 100;
	maxNumberofLines = 25;
}
TextStorage::~TextStorage()
{

}
void TextStorage::insertChar(char c, int& x, int& y)
{
    // Step 1: Only input valid characters (letters and spaces)
    if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == ' ')))
    {
        return; // Ignore invalid characters
    }

    if (head == nullptr)
    {
        TextNode* temp = new TextNode;
        temp->data = c;
        temp->right = nullptr;
        temp->left = nullptr;
        temp->down = nullptr;
        temp->up = nullptr;
        head = temp;
        return;
    }

    // Step 2: Traverse to the y-th line (vertical traversal)
    TextNode* headtemp = head;
    for (int i = 0; i < y; i++)
    {
        if (headtemp->down == nullptr)
        {
            // Create new lines if necessary
            TextNode* newLine = new TextNode;
            newLine->data = '\0';  // Placeholder for empty node
            newLine->right = nullptr;
            newLine->left = nullptr;
            newLine->up = headtemp;
            newLine->down = nullptr;
            headtemp->down = newLine;
            headtemp = newLine;
        }
        else
        {
            headtemp = headtemp->down;
        }
    }

    // Step 3: Traverse to the x-th character position in the y-th line (horizontal traversal)
    TextNode* temp = headtemp;
    int currinline = 0;

    while (currinline < x && temp->right != nullptr)
    {
        temp = temp->right;
        currinline++;
    }

    // Step 4: If x exceeds the current line length, fill the gap with spaces
    while (currinline < x)
    {
        TextNode* newNode = new TextNode;
        newNode->data = ' ';  // Fill with spaces to reach the x position
        newNode->right = nullptr;
        newNode->left = temp;
        newNode->down = nullptr; // No vertical connection needed
        temp->right = newNode;
        temp = newNode;
        currinline++;
    }

    // Step 5: Insert the new character and shift the rest of the line to the right
    TextNode* newNode = new TextNode;
    newNode->data = c;
    newNode->left = temp;
    newNode->right = temp->right;
    newNode->down = nullptr;  // Since we are inserting on the current line
    newNode->up = nullptr;    // No vertical connection needed for now

    // Adjust the links for the adjacent nodes
    if (temp->right != nullptr)
    {
        temp->right->left = newNode;
    }
    temp->right = newNode;

    // Step 6: Shift all characters after the inserted one to the right
    TextNode* next = newNode->right;
    while (next != nullptr)
    {
        TextNode* tempNext = next->right; // Save the next pointer
        next->left = newNode;              // Move left link
        newNode->right = next;             // Move right link
        newNode = next;                    // Move the newNode pointer forward
        next = tempNext;                   // Move to the next character
    }

    // Handle word wrapping if needed, in case maxLineLength is exceeded
    if (currinline + 1 >= maxLineLength)
    {
        wrapWord(temp); // Ensure wrapWord function is properly defined elsewhere
    }
}
void TextStorage::deleteChar()
{

}
void TextStorage::moveCursor()
{

}
char TextStorage::getCharat(int x, int y)
{
	return '0';
}
char* TextStorage::getCurrentLine()
{
	return 0;
}
char* TextStorage::getAllText()
{
	return 0;
}
void TextStorage::printlist()
{
    // Start from the head node
    TextNode* lineNode = head;

    // Traverse through each line
    while (lineNode != nullptr)
    {
        // Traverse the current line using the right pointers
        TextNode* temp = lineNode;
        while (temp != nullptr)
        {
            // Print the character stored in the current node
            std::cout << temp->data;
            temp = temp->right;
        }

        // Move to the next line
        std::cout << std::endl;
        lineNode = lineNode->down;
    }
}
