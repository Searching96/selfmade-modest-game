#include "Star.h"

CStar::CStar(float x, float y) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = STAR_GRAVITY;
	this->vx = -STAR_SPEED_X; // not a good practice, but just try
}

void CStar::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_STAR)->Render(x, y);

	RenderBoundingBox();
}

void CStar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CStar::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CStar::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0)
	{
		vy = -vy;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CStar::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - STAR_BBOX_WIDTH / 2;
	t = y - STAR_BBOX_HEIGHT / 2;
	r = l + STAR_BBOX_WIDTH;
	b = t + STAR_BBOX_HEIGHT;
}