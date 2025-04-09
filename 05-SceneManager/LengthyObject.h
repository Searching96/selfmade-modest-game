#pragma once

#include "GameObject.h"

class CLengthyObject : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	string spriteIdBegin, spriteIdMiddle, spriteIdEnd;
	int axis;				// 0: horizontal, 1: vertical

public:
	CLengthyObject(float x, float y,
		float cell_width, float cell_height, int length,
		string sprite_id_begin, string sprite_id_middle, string sprite_id_end,
		int axis) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
		this->axis = axis;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsDirectionColliable(float nx, float ny);
};

typedef CLengthyObject* LPLENGTHYOBJECT;