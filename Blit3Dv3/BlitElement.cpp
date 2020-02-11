#include "BlitElement.h"
#include "Blit3D.h"

extern Blit3D *blit3D;

extern bool LMB;

void BlitElement::Collision(glm::vec2 &cursorPos)
{
	prev_BE_State = BE_State;

	if (cursorPos.x > BE_Edge.x && cursorPos.x < BE_Edge.y)
	{
		if (cursorPos.y > BE_Edge.z && cursorPos.y < BE_Edge.w)
		{
			if (LMB)
			{
				BE_State = BlitStatus::SELECTED;
			}
			else
			{
				BE_State = BlitStatus::IN_FOCUS;
			}
		}
		else BE_State = BlitStatus::OUT_OF_FOCUS;
	}
	else BE_State = BlitStatus::OUT_OF_FOCUS;


	if (LMB && BE_State == BlitStatus::SELECTED)
	{
		blit3D->ShowCursor(false);
		SetCursorPos(BE_Pos.x, 1080 - BE_Pos.y);
	}
	else
	{
		blit3D->ShowCursor(true);
	}
}

void BlitElement::UpdateBE(glm::vec2 &cursorPos)
{
	Collision(cursorPos);
}

void BlitElement::DrawBE(Sprite *BlitSprite)
{
	assert(BlitSprite != NULL && "No Sprite is assigned to the button");
	assert(BE_Font != NULL && "No text font is assigned to the button");

	BlitSprite->Blit(BE_Pos.x, BE_Pos.y, BE_Scale, BE_Scale);
	BE_Font->BlitText(BE_txtPos.x, BE_txtPos.y, BE_Scale, BE_Scale, BE_Text);
}

void BlitElement::SetText(std::string text)
{
	BE_Text = text;
}

void BlitElement::SetTextPos(float pos_x, float pos_y)
{
	BE_txtPos.x = pos_x + BE_txtOffset.x;
	BE_txtPos.y = pos_y + BE_txtOffset.y;
}

void BlitElement::SetTextOffset(float offset_x, float offset_y)
{
	BE_txtOffset.x = -offset_x;
	BE_txtOffset.y = offset_y;
}

void BlitElement::SetBounds(float pos_x, float pos_y, float width, float height)
{
	//Position setting
	BE_Pos = glm::vec2(pos_x, pos_y);

	//Edge finding
	glm::vec2 halfSize = glm::vec2(width / 2, height / 2);
	float leftEdge = pos_x - halfSize.x;
	float rightEdge = pos_x + halfSize.x;
	float bottomEdge = pos_y - halfSize.y;
	float topEdge = pos_y + halfSize.y;

	//Edge Setting
	BE_Edge = glm::vec4(leftEdge, rightEdge, bottomEdge, topEdge);
}

void BlitElement::SetFont(AngelcodeFont *txtFont)
{
	BE_Font = txtFont;
}