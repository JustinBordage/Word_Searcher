#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "CharNode.h"

class PointNode
{
public:
	PointNode* next;
	CharNode* dataNode;

	PointNode();
	~PointNode();
};

class CharMap
{
private:
	PointNode* head;
	short width, height;

	void makeMap(short sizeX, short sizeY);

public:
	std::vector<std::string> wordList;

	CharMap(std::string filename);
	~CharMap();

	bool loadPuzzle(std::string fileName);

	CharNode* getNode(short posX, short posY);
	char getData(CharNode* node);
	void setData(CharNode* node, char value);
	short getWidth();
	short getHeight();
};