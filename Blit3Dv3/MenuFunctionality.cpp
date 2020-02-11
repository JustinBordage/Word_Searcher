
#include <chrono>
#include "MenuFunctionality.h"
#include "MenuManager.h"
#include "WordFinder.h"

#include "Blit3D.h"

extern Blit3D *blit3D;
extern Menu* menu;

extern bool paused;

extern void clearHighlight();
extern void wipeFoundWords();
extern std::vector<HashNode*> wordCoords;
extern float solveTime;

//---------------------------------------------------------------------------------------------------------------------------------------------------
//															MENUS
//---------------------------------------------------------------------------------------------------------------------------------------------------

void scoreMenuText(AngelcodeFont *textFont, glm::vec2 menuPosition)
{
	//Add text display functionality for the menu here
}

void gameOverMenuText(AngelcodeFont *textFont, glm::vec2 menuPosition)
{
	//Add text display functionality for the menu here
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//														   GENERAL BUTTONS
//---------------------------------------------------------------------------------------------------------------------------------------------------

void pause(int number)
{
	//Pauses the game
	paused = true;

	menu->menuStatus = Menu::MenuConfig::PAUSEMENU;
}

void resume(int number)
{
	//Unpauses the game
	paused = false;

	//Switches the UI and Menu Configurations
	menu->menuStatus = Menu::MenuConfig::NONE;
	menu->UI_Status = Menu::UI_Config::INGAME1;
}

void back(int number)
{
	//Returns to the previousMenu
	menu->menuStatus = menu->prevMenu;
}

void closeApp(int number)
{
	//Exits the application
	blit3D->Quit();
}

void mainMenu(int number)
{
	//Switches over to the start menu
	menu->menuStatus = Menu::MenuConfig::PAUSEMENU;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//															UNIQUE BUTTONS
//---------------------------------------------------------------------------------------------------------------------------------------------------

void highlightWord(int index)
{
	//Clears any previous highlighting
	clearHighlight();

	if (wordCoords.size() > index)
	{
		HashNode* tail = wordCoords[index];

		while (tail != NULL)
		{
			tileMap[tail->value.x][tail->value.y] = 1;
			tail = tail->next;
		}
	}
}

void switchUI(int number)
{
	if (menu->UI_Status == Menu::UI_Config::INGAME1)
		menu->UI_Status = Menu::UI_Config::INGAME2;
	else if (menu->UI_Status == Menu::UI_Config::INGAME2)
		menu->UI_Status = Menu::UI_Config::INGAME1;
}

void basicMethod(int number)
{
	short solve;
	std::chrono::steady_clock::time_point start, finish;
	BasicFinder* basicFinder = new BasicFinder();
	solveTime = 0.f;

	for (solve = 0; solve < 150; solve++)
	{
		wipeFoundWords();

		start = std::chrono::high_resolution_clock::now();								// start timer on sort function

		basicFinder->solvePuzzle();

		finish = std::chrono::high_resolution_clock::now();								// end timer on sort function

		solveTime += std::chrono::duration_cast<std::chrono::nanoseconds>				// calculate elapsed time for sort
			(finish - start).count() * 0.000001f;
	}
		
	solveTime /= 150.f;

	delete basicFinder;
}

void improvedMethod(int number)
{
	short solve;
	std::chrono::steady_clock::time_point start, finish;
	ImprovedFinder* improvedFinder = new ImprovedFinder();
	solveTime = 0.f;

	for (solve = 0; solve < 150; solve++)
	{
		wipeFoundWords();

		start = std::chrono::high_resolution_clock::now();									//start timer on sort function

		improvedFinder->solvePuzzle();

		finish = std::chrono::high_resolution_clock::now();									//end timer on sort function

		solveTime += std::chrono::duration_cast<std::chrono::nanoseconds>					//calculate elapsed time for sort
			(finish - start).count() * 0.000001f;
	}

	solveTime /= 150.f;

	delete improvedFinder;
}

void hashMethod(int number)
{
	short solve;
	std::chrono::steady_clock::time_point start, finish;
	HashFinder* hashFinder = new HashFinder();
	solveTime = 0.f;

	for(solve = 0; solve < 150; solve++)
	{
		wipeFoundWords();

		start = std::chrono::high_resolution_clock::now();									// start timer on sort function

		hashFinder->solvePuzzle();

		finish = std::chrono::high_resolution_clock::now();									// end timer on sort function

		solveTime += std::chrono::duration_cast<std::chrono::nanoseconds>					// calculate elapsed time for sort
			(finish - start).count() * 0.000001f;
	}

	solveTime /= 150.f;

	delete hashFinder;
}