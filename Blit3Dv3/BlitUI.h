#pragma once
#include "BlitButton.h"

class BlitUI
{
private:
	enum BlitStatus { OUT_OF_FOCUS, IN_FOCUS, SELECTED };

protected:
	std::vector<BlitElement *> elementList;
	glm::vec2 UI_Pos;

public:
	enum TextSize { SMALL = 0, MEDIUM, LARGE };

	BlitUI(float userInterfacePos_X, float userInterfacePos_Y);

	void newButton(std::string displayName, float positionOffset_x, float positionOffset_y, float btnWidth, float btnHeight, void(*input)(int), short wordIndex);
	~BlitUI();


	BlitElement* updateElement(BlitElement *element, glm::vec2 &cursorPos);
	void BlitUI::drawElement(BlitElement *element);
	void UpdateUI(glm::vec2 &cursorPos);
	void DrawUI();

	void SetButtonFont(AngelcodeFont *btnTxtFont);
	void SetButtonSprites(Sprite *simpleBtn, Sprite *hoverBtn, Sprite *pressedBtn);
	void AnimateButtons(bool animated);

	bool checkElementFocus(glm::vec2 &cursorPos);
};