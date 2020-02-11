#pragma once
#pragma once
#include "Blit3D.h"

extern AngelcodeFont *defaultTextFont[3];
extern Sprite *defaultBtnSprites[3];
extern Sprite *defaultSliderSprites[2];

enum class ElementID { BUTTON = 0, SLIDER, TOGGLEBUTTON, COOLDOWN };

class BlitElement
{
protected:
	AngelcodeFont* BE_Font = NULL;
	float txtWidth = 0.f;

	enum BlitStatus { OUT_OF_FOCUS, IN_FOCUS, SELECTED };

	std::string BE_Text;
	glm::vec2 BE_txtOffset = glm::vec2(0.f, 0.f);
	glm::vec2 BE_txtPos;

	float BE_Scale = 1.f;
	glm::vec2 BE_Pos;
	glm::vec4 BE_Edge;

	void UpdateBE(glm::vec2 &cursorPos);
	void DrawBE(Sprite *BlitSprite);

public:
	void Collision(glm::vec2 &cursorPos);

	ElementID elementID;

	BlitStatus BE_State = OUT_OF_FOCUS;
	BlitStatus prev_BE_State = OUT_OF_FOCUS;

	enum TextSize { SMALL = 0, MEDIUM, LARGE };

	void SetText(std::string text);
	void SetTextPos(float pos_x, float pos_y);
	void SetTextOffset(float offset_x, float offset_y);
	void SetBounds(float pos_x, float pos_y, float width, float height);
	void SetFont(AngelcodeFont *txtFont);
};