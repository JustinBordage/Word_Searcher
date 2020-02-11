#include "WordFinder.h"

#define UP(length) (posY - length >= 0)
#define DOWN(length) (posY + length < charMap->getHeight())
#define REVERSE(length) (posX - length >= 0)
#define FORWARD(length) (posX + length < charMap->getWidth())

bool BasicFinder::recurseWord(const short &posX, const short &posY, const short &order)
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

		bool wordFound = ((order == wordToFind.length() - 1) || (recurseWord(posX + dirX, posY + dirY, order + 1)));		//Checks if the end of the word has been reached or
																															//if the word was found and the recursion is exiting

		if (wordFound)
		{
			saveWordCoords(posX, posY);																						//Saves the current char position

			return true;
		}
	}

	return false;																											//If any of the characters were incorrect
}

bool BasicFinder::bruteSearching(short &posX, short &posY)
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

	short wordLength = wordToFind.length() - 1;

	//----------------------
	//	 Bounds Checking
	//----------------------

	possibleDirX.clear();
	possibleDirY.clear();

	if (REVERSE(wordLength)) addPossibleDirection(-1, 0);																	//Pushes back the directions for each axis (X, Y)

	if (FORWARD(wordLength)) addPossibleDirection(1, 0);																	//Pushes back the directions for each axis (X, Y)

	if (UP(wordLength))
	{
		addPossibleDirection(0, -1);																						//Pushes back the directions for Up (X, Y)

		if (REVERSE(wordLength)) addPossibleDirection(-1, -1);																//Pushes back the directions for Diagonal Up & Reverse (X, Y)

		if (FORWARD(wordLength)) addPossibleDirection(1, -1);																//Pushes back the directions for Diagonal Up & Forward (X, Y)
	}

	if (DOWN(wordLength))
	{
		addPossibleDirection(0, 1);																							//Pushes back the directions for down (X, Y)

		if (REVERSE(wordLength)) addPossibleDirection(-1, 1);																//Pushes back the directions for Diagonal Down & Reverse (X, Y)

		if (FORWARD(wordLength)) addPossibleDirection(1, 1);																//Pushes back the directions for Diagonal Down & Forward (X, Y)
	}

	//----------------------
	// Directional Searching
	//----------------------

	for (short direction = 0; direction < possibleDirX.size(); direction++)													//Cycles through each of the possible directions and
	{																														//recursively verifies each of them for the target word

		dirX = possibleDirX[direction];																						//Retrieves the direction for each axis
		dirY = possibleDirY[direction];																						//from the list possible directions to search

		if (recurseWord(posX, posY, 0))																						//Recursively checks the possible direction
			return true;
	}

	return false;
}

void BasicFinder::findWord(std::string word)
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

	short posX, posY;																											//For loop variables

	CharNode* node;																												//Pointer variable for the node in the CharMap

	short width = charMap->getWidth();																							//Gets the map width and height
	short height = charMap->getHeight();

	wordToFind = word;																											//Sets the word to find

	for (posX = 0; posX < width; posX++)																						//Cycles through the x and y axes
	{
		for (posY = 0; posY < height; posY++)
		{
			node = charMap->getNode(posX, posY);																				//Retrieves the CharNode at the given position in the map

			if (wordToFind[0] == charMap->getData(node))																		//Checks if the char in the given node matches the
			{																													//first char of the word that is to be found

				if (bruteSearching(posX, posY))																					//Searches for the word starting from the 
				{																												//position of the first character

					posY = height;																								//If the word is found, it exits out of the loops
					posX = width;
				}
			}
		}
	}
}

void BasicFinder::solvePuzzle()
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






















/*
bool BasicFinder::tryDiagonals(const short &posX, const short &posY)
{
	//Variable
	short wordLength = wordToFind.length() - 1;

	//Checks if there's was enough space to the
	//left of the first char to fit the given word
	if (posX - wordLength >= 0)
	{
		dirX = -1;

		if (recurseWord(posX, posY, 0))
			return true;
	}

	//Checks if there's was enough space to the
	//right of the first char to fit the given word
	if (posX + wordLength < charMap->getWidth())
	{
		dirX = 1;

		if (recurseWord(posX, posY, 0))
			return true;
	}

	return false;
}


bool BasicFinder::bruteSearching(short &posX, short &posY)
{
	//Variables
	short wordLength = wordToFind.length() - 1;

	//Sets the y axis direction of the searching algorithm
	dirY = 0;

	//Checks if there's was enough space to the
	//left of the first character to fit the given word
	if (posX - wordLength >= 0)
	{
		//Sets the x axis direction of the searching algorithm
		dirX = -1;

		//Recurses through the map towards the left edge of the puzzle
		if (recurseWord(posX, posY, 0))
			return true;
	}

	//Checks if there's was enough space to the
	//left of the first character to fit the given word
	if (posX + wordLength < charMap->getWidth())
	{
		//Sets the x axis direction of the searching algorithm
		dirX = 1;

		//Recurses upwards through the map towards upper edge of the puzzle.
		if (recurseWord(posX, posY, 0))
			return true;
	}

	//Checks if there's was enough space above
	//the first character to fit the given word
	if (posY + wordLength < charMap->getHeight())
	{
		//Resets the x axis direction & sets the y axis direction
		//of the searching algorithm.
		dirX = 0, dirY = 1;

		//Recurses upwards through the map towards upper edge of the puzzle.
		if (recurseWord(posX, posY, 0))
			return true;

		//Since the upper direction has been already confirmed to
		//have enough space, it then checks the upper diagonals
		if (tryDiagonals(posX, posY))
			return true;
	}

	//Checks if there's was enough space below
	//the first character to fit the given word
	if (posY - wordLength >= 0)
	{
		//Resets the x axis direction & sets the y axis direction
		//of the searching algorithm.
		dirX = 0, dirY = -1;

		//Recurses downwards through the map towards lower edge of the puzzle.
		if (recurseWord(posX, posY, 0))
			return true;

		//Since the upper direction has been already confirmed to
		//have enough space, it then checks the upper diagonals
		if (tryDiagonals(posX, posY))
			return true;
	}

	return false;
}*/