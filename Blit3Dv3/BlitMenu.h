#pragma once
#include "BlitUI.h"

class BlitMenu : public BlitUI
{
private:
	//Datamembers
	std::string menuName;
	AngelcodeFont *menuTitleFont = NULL;
	AngelcodeFont *menuTextFont = NULL;
	Sprite *menuBacksplash = NULL;
	glm::vec2 menuBounds;
	glm::vec2 titleOffset;
	glm::vec2 menuTitleScale = glm::vec2(1.f, 1.f);
	void(*MenuFunction)(AngelcodeFont *textFont, glm::vec2 menuPosition) = NULL;

public:
	BlitMenu(std::string menuTitleName, Sprite *menuSprite, glm::vec2 menuPosition, glm::vec2 menuBoundaries);
	~BlitMenu();

	void SetMenuFunction(void(*func)(AngelcodeFont *textFont, glm::vec2 menuPosition));
	void SetTitleFont(AngelcodeFont *titleFont);
	void SetTextFont(AngelcodeFont *textFont);
	void SetTitleOffset(glm::vec2 title_Offset);
	void SetTitleScale(glm::vec2 title_Scale);

	void UpdateMenu(glm::vec2 &cursorPos);
	void DrawMenu();
};