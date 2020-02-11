#include "BlitUI.h"
#include "BlitButton.h"
#include "Blit3D.h"
#include "MenuManager.h"
#include "BlitElement.h"

#include <cassert>

extern Blit3D *blit3D;
extern Menu menu;
class BlitButton;
class BlitSlider;

extern bool LMB;

BlitUI::BlitUI(float userInterfacePos_X, float userInterfacePos_Y)
{
	UI_Pos.x = userInterfacePos_X;
	UI_Pos.y = userInterfacePos_Y;
}

BlitUI::~BlitUI()
{
	BlitButton *button;

	for (auto &element : elementList)
	{
		button = (BlitButton *)element;

		delete button;
	}

	elementList.clear();
}

BlitElement* BlitUI::updateElement(BlitElement *element, glm::vec2 &cursorPos)
{
	switch (element->elementID)
	{
	case ElementID::BUTTON:
	{
		BlitButton *button = (BlitButton *)element;
		button->Update(cursorPos);
		element = (BlitElement *)button;
		break;
	}
	}

	return element;
}

void BlitUI::drawElement(BlitElement *element)
{
	switch (element->elementID)
	{
	case ElementID::BUTTON:
	{
		BlitButton *button = (BlitButton *)element;
		button->Draw();
		break;
	}
	}
}

void BlitUI::UpdateUI(glm::vec2 &cursorPos)
{
	static bool selected = false;

	for (auto &element : elementList)
	{
		//Checks if another element has been selected already
		//before updating, (to prevent multiple UI elements
		//from being activated simultaneously
		if (!selected || element->BE_State == BlitStatus::SELECTED
			|| element->prev_BE_State == BlitStatus::SELECTED)
		{
			element = updateElement(element, cursorPos);
		}

		//
		if (element->BE_State != BlitStatus::SELECTED
			&& element->prev_BE_State == BlitStatus::SELECTED)
		{
			selected = false;
		}
		else if (!selected && element->BE_State == BlitStatus::SELECTED)
		{
			selected = true;
		}
	}
}

bool BlitUI::checkElementFocus(glm::vec2 &cursorPos)
{
	if (!LMB)
		for (auto element : elementList)
		{
			element->Collision(cursorPos);

			if (element->elementID == ElementID::BUTTON)
			{
				BlitButton* btn = (BlitButton*)element;
				if (btn->animated)
					return true;
			}

			if (element->BE_State == BlitStatus::IN_FOCUS)
				return true;
		}

	return false;
}

void BlitUI::DrawUI()
{
	for (auto &element : elementList)
	{
		drawElement(element);
	}
}

void BlitUI::newButton(std::string name, float posOffset_x, float posOffset_y, float btnWidth, float btnHeight, void(*input)(int), short wordIndex)
{
	glm::vec2 btnPos = glm::vec2(UI_Pos.x + posOffset_x, UI_Pos.y + posOffset_y);

	BlitButton *blitBtn = new BlitButton(name, btnPos.x, btnPos.y, btnWidth, btnHeight, input);

	blitBtn->index = wordIndex;

	elementList.push_back(blitBtn);
}

void BlitUI::SetButtonFont(AngelcodeFont *btnTxtFont)
{
	BlitButton* button;

	for (auto &element : elementList)
	{
		button = (BlitButton*) element;
		button->SetFont(btnTxtFont);
	}
}

void BlitUI::SetButtonSprites(Sprite *simpleBtnSprite, Sprite *hoverBtnSprite, Sprite *pressedBtnSprite)
{
	BlitButton* button;

	for (auto &element : elementList)
	{
		if (element->elementID == ElementID::BUTTON)
		{
			button = (BlitButton*) element;
			button->SetBtnSprites(simpleBtnSprite, hoverBtnSprite, pressedBtnSprite);
		}
	}
}

void BlitUI::AnimateButtons(bool animated)
{
	BlitButton* button;

	for (auto &element : elementList)
	{
		button = (BlitButton*)element;

		button->ShowAnimation(animated);
	}
}
