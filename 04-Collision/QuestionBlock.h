#pragma once
#include "GameObject.h"

#define QUESTION_BLOCK_BBOX_WIDTH  16
#define QUESTION_BLOCK_BBOX_HEIGHT 16

class CQuestionBlock : public CGameObject
{
protected:
	bool isHit = false;

public:
	CQuestionBlock(float x, float y) : CGameObject(x, y) {}

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT*> coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};

