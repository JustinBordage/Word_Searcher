#include "BlitMenu.h"
#include "MenuManager.h"

BlitMenu::BlitMenu(std::string menuTitleName, Sprite *menuSprite, glm::vec2 menuPosition, glm::vec2 menuBoundaries) : BlitUI(menuPosition.x, menuPosition.y)
{
	//Sets the default fonts
	menuTitleFont = defaultTextFont[LARGE];
	menuTextFont = defaultTextFont[MEDIUM];

	//Sets the menu values
	menuName = menuTitleName;
	menuBacksplash = menuSprite;
	menuBounds = menuBoundaries;

	//Sets the default title offset (Centered, 100 pixels below top edge)
	titleOffset.y = (menuBounds.y / 2.f) - 100.f;
	titleOffset.x = 0.f;
}

BlitMenu::~BlitMenu()
{

}

void BlitMenu::UpdateMenu(glm::vec2 &cursorPos)
{
	//Updates the functionality of all the UI Elements
	UpdateUI(cursorPos);
}

void BlitMenu::DrawMenu()
{
	//Displays the backsplash of the menu
	menuBacksplash->Blit(UI_Pos.x, UI_Pos.y);

	//Displays the title of the menu
	menuTitleFont->BlitAutoText(UI_Pos.x + titleOffset.x, UI_Pos.y + titleOffset.y,
		menuTitleScale.x, menuTitleScale.y, menuName);

	//Displays all of the UI Elements
	DrawUI();

	//Calls the functionality of the menu
	if (MenuFunction != NULL)
	{
		MenuFunction(menuTextFont, UI_Pos);
	}
}

void BlitMenu::SetMenuFunction(void(*func)(AngelcodeFont *textFont, glm::vec2 menuPosition))
{
	MenuFunction = func;
}

void BlitMenu::SetTitleFont(AngelcodeFont *titleFont)
{
	menuTitleFont = titleFont;
}

void BlitMenu::SetTextFont(AngelcodeFont *textFont)
{
	menuTextFont = textFont;
}

void BlitMenu::SetTitleOffset(glm::vec2 title_Offset)
{
	//Sets the title offset
	titleOffset.y = (menuBounds.y / 2.f) - title_Offset.y; //Offsets from top to bottom (smallest to largest)
	titleOffset.x = title_Offset.x - (menuBounds.x / 2.f);	//Offsets from left to right (smallest to largest)
}

void BlitMenu::SetTitleScale(glm::vec2 title_Scale)
{
	menuTitleScale = title_Scale;
}
