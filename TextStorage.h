#pragma once
#include <iostream>
using namespace std;
#include "TextNode.h"
class TextStorage
{
	TextNode* head;
	int cursorX;
	int cursorY;
	int maxLineLength;
	int maxNumberofLines;
	void wrapWord(TextNode* temp);
	void createNewLine();
public:
	TextStorage();
	~TextStorage();
	void insertChar(char c, int& x, int& y);
	TextNode* gethead()
	{
		return head;
	}
	void deleteChar();
	void moveCursor();
	char getCharat(int x, int y);
	char* getCurrentLine();
	char* getAllText();
	void printlist();
};

