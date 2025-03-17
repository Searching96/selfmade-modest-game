#include "Game.h"
#include "Mario.h"

CMario::CMario(float x, float y, float baseSpeed) : CMoveableObject(x, y, baseSpeed)
{
	this->vx = baseSpeed;
	this->vy = 0;
}

void CMario::Update(DWORD dt)
{
	if (this->vx > 0)
		dir = 1;
	else if (this->vx < 0)
		dir = -1;

	if (GetAsyncKeyState('A') & 0x8000) // Move left
	{
		vx = -baseSpeed;
	}
	else if (GetAsyncKeyState('D') & 0x8000) // Move right
	{
		vx = baseSpeed;
	}
	else // Idling
	{
		vx = 0;
	}

	x += vx*dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth - MARIO_WIDTH) {

		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - MARIO_WIDTH)
		{
			x = (float)(BackBufferWidth - MARIO_WIDTH);
		}


	}
}

void CMario::Render()
{
	LPANIMATION ani = NULL;

	//[RED FLAG][TODO]: Student needs to think about how to associate this animation/asset to Mario!!
	if (vx > 0) 
		ani = CAnimations::GetInstance()->Get(500);
	else if (vx < 0) 
		ani = CAnimations::GetInstance()->Get(501);
	else if (vx == 0 && dir == 1) 
		ani = CAnimations::GetInstance()->Get(502);
	else if (vx == 0 && dir == -1) 
		ani = CAnimations::GetInstance()->Get(503);

	ani->Render(x, y);
}

void CBrick::Render() {

	LPANIMATION ani = CAnimations::GetInstance()->Get(510);

	ani->Render(x, y);

}