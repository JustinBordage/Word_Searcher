#pragma once
#include <vector>
#include <string>
#include "CharMap.h"
#include "HashNode.h"

extern CharMap* charMap;
extern std::vector<std::vector<short>> tileMap;
extern std::vector<HashNode*> wordCoords;

class WordFinder
{
protected:
	std::vector<short> possibleDirX, possibleDirY;
	void addPossibleDirection(short pDirX, short pDirY);

	short wordNum;
	std::string wordToFind;
	short dirX, dirY;

	void saveWordCoords(short posX, short posY);
};

class BasicFinder : public WordFinder
{
private:
	void findWord(std::string word);
	bool bruteSearching(short &posX, short &posY);
	bool recurseWord(const short &posX, const short &posY, const short &order);
	
public:
	void solvePuzzle();
};

class ImprovedFinder : public WordFinder
{
private:
	bool improvedSearching(short &posX, short &posY);
	bool recurseWord(const short &posX, const short &posY, const short &order);
	void findWord(std::string word);
public:
	void solvePuzzle();
};

class HashFinder : public WordFinder
{
	bool improvedSearching(short &posX, short &posY);
	bool recurseWord(const short &posX, const short &posY, const short &order);
	void findWord(std::string word);
public:
	void solvePuzzle();
};