#include "WordFinder.h"

void WordFinder::addPossibleDirection(short pDirX, short pDirY)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	addPossibleDirection
	//
	//Method parameters	:	short posX, short posY
	//
	//Method return		:	void
	//
	//Synopsis			:   Adds the possible directions that the algorithms can search into two vectors
	//						
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						12/02/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	possibleDirX.push_back(pDirX);
	possibleDirY.push_back(pDirY);
}

void WordFinder::saveWordCoords(short posX, short posY)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	saveWordCoords
	//
	//Method parameters	:	short posX, short posY
	//
	//Method return		:	void
	//
	//Synopsis			:   Saves the coords of the characters that were found by the algorithm
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						11/28/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	if (wordCoords.size() > wordNum)
	{
		HashNode* charLoc = new HashNode(Coords(posX, posY));

		charLoc->next = wordCoords[wordNum];

		wordCoords[wordNum] = charLoc;
	}
	else
	{
		//Saves the final char position since it was confirmed before the recursion
		wordCoords.push_back(new HashNode(Coords(posX, posY)));
	}

}