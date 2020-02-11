#include "CharNode.h"

CharNode::CharNode(char letter)
{
	next = NULL;
	charData = letter;
}

CharNode::~CharNode()
{
	if (next != NULL)
		delete next;
}