#pragma once
class TextNode
{
public:
	char data;
	TextNode* left;
	TextNode* right;
	TextNode* up;
	TextNode* down;
	TextNode();
	TextNode(char d);
	~TextNode();
};

