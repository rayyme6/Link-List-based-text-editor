#include "TextNode.h"
TextNode::TextNode()
{
	data = '\0';
	left = nullptr;
	right = nullptr;
	up = nullptr;
	down = nullptr;
}
TextNode::TextNode(char d)
{
	data = d;
	left = nullptr;
	right = nullptr;
	up = nullptr;
	down = nullptr;
}

TextNode::~TextNode()
{
	delete left;
	delete right;
	delete up;
	delete down;
}
