#include "CharMap.h"
#include <iostream>
#include <fstream>
#include "CharNode.h"
#include <iostream>

PointNode::PointNode()
{
	dataNode = NULL;
	next = NULL;
}

PointNode::~PointNode()
{
	if (dataNode != NULL)
		delete dataNode;

	if (next != NULL)
		delete next;
}

CharMap::CharMap(std::string filename)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	CharMap
	//
	//Method parameters	:	std::string filename
	//
	//Method return		:	void
	//
	//Synopsis			:   Constructor for the CharMap Object
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/16/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	head = new PointNode();																						//Creates a charmap head node

	loadPuzzle(filename);																						//Loads a new CharMap														
}

CharMap::~CharMap()
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	~CharMap
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Deconstructor for the CharMap Object
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/16/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	delete head;																								//Starts deleting the map from the head
}

bool CharMap::loadPuzzle(std::string filename)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	loadPuzzle
	//
	//Method parameters	:	std::string filename
	//
	//Method return		:	void
	//
	//Synopsis			:   Loads CharMap value from a puzzle file
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						11/27/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	std::ifstream mapfile;																						//Map File Reader Variables
	short charX, charY;
	char charData;
	short numberOfWords, words;
	std::string word;
	CharNode* node;

	mapfile.open(filename);																						//Opens Map File

	if (!mapfile.is_open())																						//Checks to see if the File was successfully opened
	{
		//messageState = MessageState::CANTLOAD;

		std::cout << " Can't open " + filename + "!" << std::endl;												//Displays error message and terminates the application
		return false;
	}

	mapfile >> width;																							//Retrieves the size of the map that's being loaded
	mapfile >> height;
	mapfile >> numberOfWords;

	makeMap(width, height);																						//Generates the remaining map nodes

	for (charY = 0; charY < height; charY++)																	//Cycles through each of the tiles in the file
	{
		for (charX = 0; charX < width; charX++)
		{
			mapfile >> charData;																				//Retrieves the character from the text file

			node = getNode(charX, charY);

			setData(node, charData);
		}
	}

	for (words = 0; words < numberOfWords; words++)
	{
		mapfile >> word;
		wordList.push_back(word);
	}

	mapfile.close();																							//Closes the Map File

	//messageState = MessageState::MAPLOADED;

	return true;																								//Returns a bool indicating weather or not the map
																													//file was successfully loaded
}

void CharMap::makeMap(short sizeX, short sizeY)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	makeMap
	//
	//Method parameters	:	short sizeX, short sizeY
	//
	//Method return		:	void
	//
	//Synopsis			:   Creates all the mapNodes
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						11/26/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	short x, y;
	PointNode* node = head;
	CharNode* charNode;

	for (x = 0; x < sizeX; x++)
	{
		node->dataNode = new CharNode(' ');
		charNode = node->dataNode;

		for (y = 0; y < sizeY - 1; y++)
		{
			charNode->next = new CharNode(' ');
			charNode = charNode->next;
		}

		if (x != sizeX - 1)
		{
			node->next = new PointNode();
			node = node->next;
		}
	}
}

CharNode* CharMap::getNode(short posX, short posY)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	getNode
	//
	//Method parameters	:	short posX, short posY
	//
	//Method return		:	CharNode*
	//
	//Synopsis			:   Retrieves the CharNode at the given position in the CharMap
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/16/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	short x, y;
	PointNode* pNode = head;
	CharNode* node;

	for (x = 0; x < posX; x++)
	{
		if (pNode->next != NULL)
			pNode = pNode->next;
	}

	node = pNode->dataNode;

	for (y = 0; y < posY; y++)
	{
		if (node->next != NULL)
			node = node->next;
	}

	return node;
}

char CharMap::getData(CharNode* node)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	getData
	//
	//Method parameters	:	CharNode* node
	//
	//Method return		:	char
	//
	//Synopsis			:   Returns the CharNode's value
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/16/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	return node->charData;
}

void CharMap::setData(CharNode* node, char value)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	setData
	//
	//Method parameters	:	CharNode* node, char value
	//
	//Method return		:	void
	//
	//Synopsis			:   Sets the value of the CharNode that was passed in
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/22/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	node->charData = value;
}

short CharMap::getWidth()
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	getWidth
	//
	//Method parameters	:	none
	//
	//Method return		:	short
	//
	//Synopsis			:   Returns the width of the CharMap
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/22/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	return width;
}

short CharMap::getHeight()
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	getHeight
	//
	//Method parameters	:	none
	//
	//Method return		:	short
	//
	//Synopsis			:   Returns the height of the CharMap
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/22/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	return height;
}

