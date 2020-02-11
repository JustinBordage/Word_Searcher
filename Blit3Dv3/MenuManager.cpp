#include "MenuManager.h"
#include "MenuFunctionality.h"
#include "Blit3D.h"

#include <cassert>

extern Blit3D *blit3D;
extern CharMap* charMap;

Menu::Menu()
{
	//Loads in the text fonts
	defaultTextFont[TextSize::SMALL] = blit3D->MakeAngelcodeFontFromBinary32("Media\\AsteroidText20.bin");
	defaultTextFont[TextSize::MEDIUM] = blit3D->MakeAngelcodeFontFromBinary32("Media\\AsteroidText45.bin");
	defaultTextFont[TextSize::LARGE] = blit3D->MakeAngelcodeFontFromBinary32("Media\\AsteroidText100.bin");

	//Loads in the button sprites
	for (int btn = 0; btn < 3; btn++)
	{
		defaultBtnSprites[btn] = blit3D->MakeSprite(0, btn * 130, 460, 130, "Media\\DefaultBtnSprites.png");
		miniBtnSprites[btn] = blit3D->MakeSprite(5, 5 + btn * 65, 365, 65, "Media\\Mini Buttons.png");
	}

	//defaultBtnSprites[3] = blit3D->MakeSprite(0, 0, 100, 100, "Media\\Button Box.png");


	//Loads in the menu backsplashes
	menuBacksplash[SMALL] = blit3D->MakeSprite(0, 0, 700, 400, "Media\\DefaultSmallMenu.png");
	menuBacksplash[MEDIUM] = blit3D->MakeSprite(610, 40, 700, 1000, "Media\\DefaultMenuSprite.png");
	menuBacksplash[LARGE] = blit3D->MakeSprite(0, 0, 1920, 1080, "Media\\DefaultMenuSprite.png"); //TODO: Add Large Menu

	glm::vec2 centerPos = glm::vec2(blit3D->screenWidth / 2, blit3D->screenHeight / 2);

	//Since the sprites are optimised for 1080p screens, 1920p X 1080p will become reference points
	//glm::vec2 scaling = glm::vec2(blit3D->screenWidth / 1920.f, blit3D->screenHeight / 1080.f);


	//---------------------------------------------------------------------------------------------------------------------------------------------------
	//															GAME UI
	//---------------------------------------------------------------------------------------------------------------------------------------------------
	
	ReloadGameUIs();
		
	//---------------------------------------------------------------------------------------------------------------------------------------------------
	//															PAUSE MENU
	//---------------------------------------------------------------------------------------------------------------------------------------------------

	BlitMenu *startMenu = new BlitMenu("Paused", menuBacksplash[SMALL], centerPos, glm::vec2(700.f, 400.f));
	startMenu->newButton("Exit", 0.f, -90.f, 450.f, 120.f, closeApp, 0);
	//startMenu->newButton("Exit", 0.f, -360.f, 450.f, 120.f, closeApp, 0);
	menuList.push_back(startMenu);
}

void Menu::ReloadGameUIs()
{
	glm::vec2 centerPos = glm::vec2(blit3D->screenWidth / 2, blit3D->screenHeight / 2);

	//---------------------------------------------------------------------------------------------------------------------------------------------------
	//															Clears previous lists
	//---------------------------------------------------------------------------------------------------------------------------------------------------

	//Deletes any previous user interfaces that were created, then clears the UI vector list
	for (int listNum = 0; listNum < UIList.size(); listNum++)
	{
		delete UIList[listNum];
	}
	UIList.clear();
	UI_Status = UI_Config::INGAME1;

	//---------------------------------------------------------------------------------------------------------------------------------------------------
	//															Creates new UIs
	//---------------------------------------------------------------------------------------------------------------------------------------------------


	int size = charMap->wordList.size();

	//Builds the in-game UI object
	BlitUI *gameUI = new BlitUI(centerPos.x, centerPos.y);

	//Creates the objects that are managed by the above UI
	for (int i = 0; i < size; i++)
	{
		if (i < 18)
			gameUI->newButton(charMap->wordList[i], 290.f, 510.f - 60.f * i, 355.f, 55.f, highlightWord, i);
		else if (i < 36)
			gameUI->newButton(charMap->wordList[i], 730.f, 510.f - 60.f * (i - 18), 355.f, 55.f, highlightWord, i);
	}

	gameUI->newButton("Basic", -700.f, -300.f, 355.f, 55.f, basicMethod, 0);
	gameUI->newButton("Improved", -700.f, -360.f, 355.f, 55.f, improvedMethod, 0);
	gameUI->newButton("Hash", -700.f, -420.f, 355.f, 55.f, hashMethod, 0);

	gameUI->SetButtonSprites(miniBtnSprites[0], miniBtnSprites[1], miniBtnSprites[2]);

	//Pushes the game UI to the update vector
	UIList.push_back(gameUI);

	if (size >= 36)
	{
		BlitUI *gameUI2 = new BlitUI(centerPos.x, centerPos.y);

		gameUI->newButton("Next Page", -200.f, -380.f, 450.f, 120.f, switchUI, 0);

		//Creates the objects that are managed by the above UI
		for (int i = 36; i < size; i++)
		{
			if (i < 54)
				gameUI2->newButton(charMap->wordList[i], 290.f, 510.f - 60.f * (i - 36), 355.f, 55.f, highlightWord, i);
			else if (i < 72)
				gameUI2->newButton(charMap->wordList[i], 730.f, 510.f - 60.f * (i - 54), 355.f, 55.f, highlightWord, i);
		}

		gameUI2->newButton("Basic", -700.f, -300.f, 355.f, 55.f, basicMethod, 0);
		gameUI2->newButton("Improved", -700.f, -360.f, 355.f, 55.f, improvedMethod, 0);
		gameUI2->newButton("Hash", -700.f, -420.f, 355.f, 55.f, hashMethod, 0);

		gameUI2->SetButtonSprites(miniBtnSprites[0], miniBtnSprites[1], miniBtnSprites[2]);

		gameUI2->newButton("Previous Page", -200.f, -380.f, 450, 120.f, switchUI, 0);

		//Pushes the game UI to the update vector
		UIList.push_back(gameUI2);
	}
}

Menu::~Menu()
{
	//Deletes all the menus that have been created, then clears the menu vector list
	for (int listNum = 0; listNum < menuList.size(); listNum++)
	{
		delete menuList[listNum];
	}
	menuList.clear();

	//Deletes all the user interfaces that have been created, then clears the UI vector list
	for (int listNum = 0; listNum < UIList.size(); listNum++)
	{
		delete UIList[listNum];
	}
	UIList.clear();
}

bool Menu::checkElementFocus(glm::vec2 &cursorPos, bool paused)
{
	//Checks if the game is paused and that there's an active menu interface
	if (paused)
	{
		if (menuList.size() > 0)
		{
			assert(menuStatus != NONE);

			//Checks if a menu interface even exists, if not the program crashes
			assert(menuList[menuStatus] != NULL && "This menu does not exist");

			//Updates the presently active menu interface
			return menuList[menuStatus]->checkElementFocus(cursorPos);
		}
	}
	//If the game isn't paused and an in-game UI object exists
	else
	{
		if (UIList.size() > 0)
		{
			assert(UI_Status != INMENU);

			//Updates the presently active user interface
			return UIList[UI_Status]->checkElementFocus(cursorPos);
		}
	}
}

void Menu::Update(glm::vec2 &cursorPos, bool paused)
{
	//Checks if the game is paused and that there's an active menu interface
	if (paused && menuStatus != NONE)
	{
		//Checks if a menu interface even exists, if not the program crashes
		assert(menuList[menuStatus] != NULL && "This menu does not exist");

		//Updates the presently active menu interface
		menuList[menuStatus]->UpdateMenu(cursorPos);
	}
	//If the game isn't paused and an in-game UI object exists
	else if (UIList.size() > 0)
	{
		//Updates the presently active user interface
		UIList[UI_Status]->UpdateUI(cursorPos);
	}
}

void Menu::Draw(bool paused)
{
	//Checks if the game is paused and that there's an active menu interface
	//Redesign to nullify the need for the menustatus check
	if (paused && menuStatus != NONE)
	{
		//Checks if a menu interface even exists, if not the program crashes (This shouldn't be needed)
		//assert(menuList[menuStatus] != NULL && "This menu object does not exist");

		//Draws the presently active menu interface
		menuList[menuStatus]->DrawMenu();
	}
	//If the game isn't paused and an in-game UI object exists
	else if (UIList.size() > 0)
	{
		//Draws the presently active user interface
		UIList[UI_Status]->DrawUI();
	}
}