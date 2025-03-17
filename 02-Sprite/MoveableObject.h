#pragma once

#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

class CMoveableObject : public CGameObject
{
protected:
	float baseSpeed;
	float vx;
	float vy;
public:
	CMoveableObject(float x, float y, float baseSpeed) : CGameObject(x, y)
	{
		this->baseSpeed = baseSpeed;
		this->vx = 0;
		this->vy = 0;
	}
	void Update(DWORD dt);
	void Render();
};

