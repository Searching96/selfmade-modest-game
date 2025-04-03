#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_STAR 20000

#define	STAR_WIDTH 10
#define STAR_BBOX_WIDTH 10
#define STAR_BBOX_HEIGHT 10

#define STAR_GRAVITY 0.0001f
#define STAR_SPEED_X 0.05f

class CStar : public CGameObject {
protected:
	float ax;
	float ay;
public:
	CStar(float x, float y);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; };
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnNoCollision(DWORD dt);
};