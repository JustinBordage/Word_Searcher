/*
Example program that shows how to display tile maps
*/
#include "Blit3D.h"
#include "HashNode.h"
#include "CharNode.h"
#include "CharMap.h"
#include "Definitions.h"
#include "WordFinder.h"

Blit3D *blit3D = NULL;

//memory leak detection
#define CRTDBG_MAP_ALLOC
#ifdef _DEBUG
	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif  // _DEBUG

#include <stdlib.h>
#include <crtdbg.h>

//GLOBAL DATA
//sprite pointers
Sprite *selection = NULL;
std::vector<Sprite *> tileList;
AngelcodeFont *afont = NULL;

MessageState messageState = MessageState::NONE;
MessageState prevMsgState = MessageState::NONE;

std::vector<std::vector<short>> tileMap;

CharMap* charMap = NULL;
HashMap* hashMap = NULL;

bool debugMode = false;

bool renewMsgTimer = false;
float messageTimer = 0.f;

//=-=-=-=-=-=-=-=-=-=-=-=-=
// Menu Builder Global Data
//=-=-=-=-=-=-=-=-=-=-=-=-=
#include "MenuManager.h"

Menu* menu;
bool LMB = false;
bool RMB = false;
bool paused = false;
glm::vec2 cursorPos = glm::vec2(0.f, 0.f);

AngelcodeFont * defaultTextFont[3] = { NULL };
Sprite *defaultBtnSprites[3] = { NULL };
Sprite *miniBtnSprites[3] = { NULL };
Sprite *defaultSliderSprites[2] = { NULL };
Sprite *defaultBacksplash = NULL;

//=-=-=-=-=-=-=-=-=-=-=-=-=
// Word Finder Global Data
//=-=-=-=-=-=-=-=-=-=-=-=-=
void clearHighlight();
std::vector<HashNode*> wordCoords;
void wipeFoundWords();
float solveTime = 0.f;

void Init()
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

	afont = blit3D->MakeAngelcodeFontFromBinary32("Media\\Oswald_72.bin");										//Angelcode font

	tileList.push_back(blit3D->MakeSprite(0, 0, 100, 100, "Media\\Default Tile.png"));							//Loads the tile sheet sprite
	tileList.push_back(blit3D->MakeSprite(0, 0, 100, 100, "Media\\Selection Tile.png"));						//Loads the tile sheet sprite

	charMap = new CharMap("Puzzle_1.txt");
	hashMap = new HashMap();
	hashMap->populateHashMap();

	std::vector<short> column;

	for (int x = 0; x < charMap->getWidth(); x++)
	{
		for (int y = 0; y < charMap->getHeight(); y++)
		{
			column.push_back(0);
		}

		tileMap.push_back(column);
	}

	//Menu Builder Initializer Method
	menu = new Menu();
}

void DeInit(void)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	DeInit
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Deconstructs all the objects in the wordfinder application
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//Menu Builder Deinitializer Method
	delete menu;

	delete charMap;
	delete hashMap;

	wipeFoundWords();
}

void Update(double seconds)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	Update
	//
	//Method parameters	:	double seconds
	//
	//Method return		:	void
	//
	//Synopsis			:   Updates all the visual element data in the application
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	if (messageState != MessageState::NONE)
		messageTimer += seconds;

	menu->Update(cursorPos, paused);
}

//Clears the list of found words
void wipeFoundWords()
{
	for (int i = 0; i < wordCoords.size(); i++)
	{
		if (wordCoords[i] != NULL)
			delete wordCoords[i];
	}

	wordCoords.clear();
}

//Clears the highlight of any searched words
void clearHighlight()
{
	short xIndex, yIndex;

	for (xIndex = 0; xIndex < charMap->getWidth(); ++xIndex)
	{
		for (yIndex = 0; yIndex < charMap->getHeight(); ++yIndex)
		{
			tileMap[xIndex][yIndex] = 0;
		}
	}
}

// Fetches a width for each character
// so it can be centered within the box
float getOffset(const char &letter)
{
	switch (letter)
	{
	case 'M':
	case 'W':
		return 17.5f;
		break;
	case 'F':
	case 'E':
	case 'L':
		return 10.f;
		break;
	case 'I':
	case 'J':
		return 7.5f;
		break;
	default:
		return 12.5f;
		break;
	}
}

void drawTile(float posX, float posY, char letter, short tileValue)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	drawTile
	//
	//Method parameters	:	float posX, float posY, char letter, short tileValue
	//
	//Method return		:	void
	//
	//Synopsis			:   Draws an individual tile in the map
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/23/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	std::string text = " ";
	text[0] = letter;
	float offset = getOffset(letter);

	tileList[tileValue]->Blit(posX, 1080.f - posY, 0.4f, 0.4f);
	afont->BlitText(posX + offset - afont->WidthText(text) * 0.5f, 1090.f -  posY, 0.3f, 0.3f, text);
}

void drawMap()
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	drawMap
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Draws all the tilemap sprites
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//						10/22/2018		J. Bordage				Optimized tilemap node retrieval
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);	//clear colour: r,g,b,a
	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	short xIndex, yIndex, tile;
	char letter;

	CharNode* charNode;
	
	for (xIndex = 0; xIndex < charMap->getWidth(); ++xIndex)
	{
		charNode = charMap->getNode(xIndex, 0);

		for (yIndex = 0; yIndex < charMap->getHeight(); ++yIndex)
		{
			tile = tileMap[xIndex][yIndex];
			letter = charMap->getData(charNode);

			drawTile(20.f + 40.f * xIndex, 20.f + 40.f * yIndex, letter, tile);
			
			if(charNode->next != NULL)
				charNode = charNode->next;
		}
	}
}

void displayMessages()
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	displayMessages
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Displays the various info messages caused by various events
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/22/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	static std::string textMsg = "";
	static float msgFade;

	if (msgFade == 1.f && messageState != prevMsgState)
		messageTimer = 0.5f;

	if (messageTimer < 0.5f)
		msgFade = messageTimer * 2;
	else
	{
		if (messageTimer >= 0.5f && messageTimer <= 2.5f)
		{
			msgFade = 1.f;
		}
		else if (messageTimer > 2.5f)
		{
			msgFade = (3.f - messageTimer) * 2;

			if (messageTimer >= 3.f)
			{
				msgFade = 0.f;
				messageTimer = 0.f;
				messageState = MessageState::NONE;
				prevMsgState = MessageState::NONE;
			}
		}
	}

	switch (messageState)
	{
	case MessageState::CANTLOAD:
		textMsg = "Error - Cannot load file";
		break;
	case MessageState::MAPLOADED:
		textMsg = "Map Loaded";
		break;
	default:
		textMsg = "";
		break;
	}

	afont->alpha = msgFade;
	afont->BlitText(960 - afont->WidthText(textMsg) * 0.5f, 100, textMsg);
	prevMsgState = messageState;
}

void Draw(void)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	Draw
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:	Draws the visual elements of the application
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	drawMap();

	if(messageState != MessageState::NONE)
		displayMessages();
	
	afont->BlitText(20.f, 75.f, "Time To Solve: " + std::to_string(solveTime) + "ms");

	menu->Draw(paused);
}

void loadMap(std::string filename)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	loadMap
	//
	//Method parameters	:	std::string filename
	//
	//Method return		:	void
	//
	//Synopsis			:	Loads a new word puzzle
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/02/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	solveTime = 0.f;

	clearHighlight();
	wipeFoundWords();

	if (charMap != NULL)
		delete charMap;

	charMap = new CharMap(filename);

	menu->ReloadGameUIs();

	if (hashMap != NULL)
		delete hashMap;

	hashMap = new HashMap();
	hashMap->populateHashMap();
}

//the key codes/actions/mods for DoInput are from GLFW: check its documentation for their values
void DoInput(int key, int scancode, int action, int mods)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	DoInput
	//
	//Method parameters	:	int key, int scancode, int action, int mods
	//
	//Method return		:	void
	//
	//Synopsis			:   Performs various actions based on the key input of the user
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		paused = !paused;

		if (paused)
		{
			menu->menuStatus = Menu::MenuConfig::PAUSEMENU;
			menu->UI_Status = Menu::UI_Config::INMENU;
		}
		else
		{
			menu->menuStatus = Menu::MenuConfig::NONE;
			menu->UI_Status = Menu::UI_Config::INGAME1;
		}
	}

	if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
	{
		loadMap("Puzzle_1.txt");
	}

	if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
	{
		loadMap("Puzzle_2.txt");
	}
}

void DoCursor(double x, double y)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	DoCursor
	//
	//Method parameters	:	double x, double y
	//
	//Method return		:	void
	//
	//Synopsis			:   Tracks the position of the cursor on the screen
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	cursorPos.x = blit3D->screenWidth / blit3D->trueScreenWidth * (int)x;
	cursorPos.y = 1080.f - blit3D->screenHeight / blit3D->trueScreenHeight * (int)y;
}

void DoMouseButton(int button, int action, int mods)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	DoMouseButton
	//
	//Method parameters	:	int button, int action, int mods
	//
	//Method return		:	void
	//
	//Synopsis			:   Does mouse button input
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		LMB = true;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		LMB = false;
	}
}

int main(int argc, char *argv[])
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	void main()
	//
	//Method parameters	:	int argc, char *argv[]
	//
	//Method return		:	int (error code)
	//
	//Synopsis			:   Initiates the application
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(8369);

	blit3D = new Blit3D(Blit3DWindowModel::BORDERLESSFULLSCREEN_1080P, 1920, 1080);

	//set our callback funcs
	blit3D->SetInit(Init);
	blit3D->SetDeInit(DeInit);
	blit3D->SetUpdate(Update);
	blit3D->SetDraw(Draw);
	blit3D->SetDoInput(DoInput);
	blit3D->SetDoCursor(DoCursor);
	blit3D->SetDoMouseButton(DoMouseButton);

	//Run() blocks until the window is closed
	blit3D->Run(Blit3DThreadModel::SINGLETHREADED);
	if (blit3D) delete blit3D;
}