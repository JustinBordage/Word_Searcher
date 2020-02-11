#pragma once
#include "Blit3D.h"
#include "BlitElement.h"

//To what extend is bloated code tolerated, or is it not tolerated at all?

class BlitButton : public BlitElement
{
private:
	//Redesign so that the in-focus is actually just a transparent color overlay
	Sprite* btnSprite[3] = { NULL };

	void(*Input)(int);

	//Hover color overlay when cursor is above the button while the LMB is not pressed
	//not togglable -1, false = 0, true = 1
	//int toggleState = -1;

	void SetTextScaleOffset(float scale_x, float scale_y);

protected:
	void Animation();

public:

	short index;
	//Variables
	enum BtnSprite { SIMPLE = 0, HOVER, PRESSED };
	bool animated = true;

	//Constructor
	BlitButton(std::string name, float btnPos_x, float btnPos_y, float btnWidth, float btnHeight, void(*input)(int));
	BlitButton::~BlitButton();

	//Functionality
	void Update(glm::vec2 &cursorPos);
	void Draw();
	void Functionality();

	//Setters
	void SetBtnSprites(Sprite *simpleBtnSprite, Sprite *hoverBtnSprite, Sprite *pressedBtnSprite);
	void SetInput(void(*func)(int));
	void ShowAnimation(bool show);
};