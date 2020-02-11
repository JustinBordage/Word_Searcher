#pragma once
#include "Blit3D.h"
#include "BlitMenu.h"
#include "BlitUI.h"
#include "CharMap.h"

extern Sprite *miniBtnSprites[3];
extern AngelcodeFont *defaultTextFont[3];
extern Sprite *defaultBtnSprites[3];
extern Sprite *defaultSliderSprites[2];

extern float sensitivity;
extern int inverted;

class Menu
{
private:
	Sprite * menuBacksplash[3];

public:
	enum UI_Config { INMENU = -1, INGAME1, INGAME2 };
	enum MenuConfig { NONE = -1, PAUSEMENU };

	int prevMenu = NONE;
	int menuStatus = NONE;
	UI_Config UI_Status = UI_Config::INGAME1;

	enum TextSize { SMALL = 0, MEDIUM, LARGE };
	std::vector<BlitMenu *> menuList;
	std::vector<BlitUI *> UIList;

	Menu();
	~Menu();
	void Draw(bool paused);
	void Update(glm::vec2 &cursorPos, bool paused);
	bool checkElementFocus(glm::vec2 &cursorPos, bool paused);
	void ReloadGameUIs();
};