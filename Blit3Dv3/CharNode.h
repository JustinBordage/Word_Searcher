#pragma once

#define NULL 0

class CharNode
{
public:
	CharNode* next;

	char charData;

	CharNode(char letter);

	~CharNode();
};