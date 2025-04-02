#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_STAR 20000

#define	STAR_WIDTH 10
#define STAR_BBOX_WIDTH 10
#define STAR_BBOX_HEIGHT 10

class CStar : public CGameObject {
public:
	CStar(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};