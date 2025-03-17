#pragma once

#include "MoveableObject.h"

#include "Animation.h"
#include "Animations.h"

#define MARIO_WIDTH 14

class CBrick : public CGameObject {
public: 
	CBrick(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
};

class CMario : public CMoveableObject
{
protected:
	int dir; // -1: left, 1: right
public:
	CMario(float x, float y, float baseSpeed);
	void Update(DWORD dt);
	void Render();
};


