#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

class CDoor : public CGameObject
{
protected:
	bool isOpened = false;
public:
	CDoor(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void SetIsOpened(bool isOpened) { this->isOpened = isOpened; }
};

