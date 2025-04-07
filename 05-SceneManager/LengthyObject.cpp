#include "LengthyObject.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

#include "debug.h"

void CLengthyObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = x - this->cellWidth / 2 + rect.right / 2;
	float yy = y + this->cellHeight / 2 - rect.bottom / 2;

	CGame::GetInstance()->Draw(xx - cx, yy - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CLengthyObject::Render()
{
	if (this->length <= 0) return;
	float xx = x;
	float yy = y;
	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteIdBegin)->Draw(xx, yy);
	if (axis == 0)
		xx += this->cellWidth;
	else if (axis == 1)
		yy -= this->cellHeight;

	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdMiddle)->Draw(xx, yy);
		if (axis == 0)
			xx += this->cellWidth;
		else if (axis == 1)
			yy -= this->cellHeight;
	}
	if (length > 1)
		s->Get(this->spriteIdEnd)->Draw(xx, yy);

	RenderBoundingBox();
}

void CLengthyObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (axis == 0)
	{
		float cellWidth_div_2 = this->cellWidth / 2;
		l = x - cellWidth_div_2;
		t = y - this->cellHeight / 2;
		r = l + this->cellWidth * this->length;
		b = t + this->cellHeight;
	}
	else if (axis == 1)
	{
		float cellHeight_div_2 = this->cellHeight / 2;
		l = x - this->cellWidth / 2;
		b = y + cellHeight_div_2;
		r = l + this->cellWidth;
		t = b - this->cellHeight * this->length;
	}
	else
	{
		l = t = r = b = 0;
		DebugOut(L"[ERROR] LengthyObject::GetBoundingBox: Invalid axis\n");
	}
}

int CLengthyObject::IsDirectionColliable(float nx, float ny)
{
	return 1; // every direction is collidable
}