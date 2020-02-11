#include <iostream>
#include <string>
#include "WordFinder.h"

#define UP(length) (posY - length >= 0)
#define DOWN(length) (posY + length < charMap->getHeight())
#define REVERSE(length) (posX - length >= 0)
#define FORWARD(length) (posX + length < charMap->getWidth())

extern HashMap* hashMap;

bool HashFinder::recurseWord(const short &posX, const short &posY, const short &order)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	Init
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Constructs and initializes all the objects and variables in the application
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	CharNode* node = charMap->getNode(posX, posY);																			//Gets the CharMap node for comparing the
																															//map character with the word character

	if (wordToFind[order] == charMap->getData(node))																		//Checks if the char in the word is identical to the
	{																														//char at it's supposed position in relation to the map

		bool wordFound = ((order == 1) || (recurseWord(posX - dirX, posY - dirY, order - 1)));								//Checks if the end of the word has been reached or
																															//if the word was found and the recursion is exiting

		if (wordFound)
		{
			if (order == 1)
				saveWordCoords(posX - dirX, posY - dirY);																	//Saves the final char position since it was confirmed before the recursion

			saveWordCoords(posX, posY);																						//Saves the current char position

			return true;
		}
	}

	return false;
}

bool HashFinder::improvedSearching(short &posX, short &posY)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	Init
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Constructs and initializes all the objects and variables in the application
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	short wordLength = wordToFind.length() - 1;																					//Variable

	//----------------------
	//	 Bounds Checking
	//----------------------

	possibleDirX.clear();
	possibleDirY.clear();

	if (REVERSE(wordLength)) addPossibleDirection(-1, 0);																		//Pushes back the directions for each axis (X, Y)

	if (FORWARD(wordLength)) addPossibleDirection(1, 0);																		//Pushes back the directions for each axis (X, Y)

	if (UP(wordLength))
	{
		addPossibleDirection(0, -1);																							//Pushes back the directions for Up (X, Y)

		if (REVERSE(wordLength)) addPossibleDirection(-1, -1);																	//Pushes back the directions for Diagonal Up & Reverse (X, Y)

		if (FORWARD(wordLength)) addPossibleDirection(1, -1);																	//Pushes back the directions for Diagonal Up & Forward (X, Y)
	}

	if (DOWN(wordLength))
	{
		addPossibleDirection(0, 1);																								//Pushes back the directions for down (X, Y)

		if (REVERSE(wordLength)) addPossibleDirection(-1, 1);																	//Pushes back the directions for Diagonal Down & Reverse (X, Y)

		if (FORWARD(wordLength)) addPossibleDirection(1, 1);																	//Pushes back the directions for Diagonal Down & Forward (X, Y)
	}

	//----------------------
	// Directional Searching
	//----------------------

	for (short direction = 0; direction < possibleDirX.size(); direction++)														//Cycles through each of the possible directions and
	{																															//recursively verifies each of them for the target word

		dirX = possibleDirX[direction];																							//Retrieves the direction for each axis
		dirY = possibleDirY[direction];																							//from the list possible directions to search

		if (recurseWord(posX + wordLength * dirX, posY + wordLength * dirY, wordLength))										//Recursively checks the possible direction
			return true;
	}

	return false;
}

void HashFinder::findWord(std::string word)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	Init
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Constructs and initializes all the objects and variables in the application
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	bool wordFound = false;																										//Boolean for exiting the loop if the word is found

	HashNode* node;																												//Pointer variable for the node in the CharMap

	wordToFind = word;																											//Sets the word to find

	node = hashMap->getKeyHash(wordToFind[0]);																					//Retrieves the key node of the given char key

	while (node != NULL && !wordFound)																							//Cycles through the list of char positions until the
	{																															//word is either found, or all the positions are exhausted

		wordFound = improvedSearching(node->value.x, node->value.y);															//Searches for the word at each hashkey position

		node = node->next;																										//Switches to the next node
	}
}

void HashFinder::solvePuzzle()
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	Init
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Constructs and initializes all the objects and variables in the application
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	for (wordNum = 0; wordNum < charMap->wordList.size(); wordNum++)
	{
		findWord(charMap->wordList[wordNum]);																					//Attempts to find each of the words in the wordlist
	}
}