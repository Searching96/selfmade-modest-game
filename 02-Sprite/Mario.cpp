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

	CheckDoorCollision(door);
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

void CMario::CheckDoorCollision(CDoor* door)
{
	float doorX = door->GetX();
	float doorY = door->GetY();
	DebugOutTitle(L"Mario Position: (%f, %f) | Door Position: (%f, %f)", x, y, doorX, doorY);
	if (x >= doorX && x <= doorX + 16 && y >= doorY - 6 && y <= doorY + 22)
	{
		DebugOutTitle(L"Collision Detected! Mario Position: (%f, %f) | Door Position: (%f, %f)", x, y, doorX, doorY);
		door->SetIsOpened(true);
	}
}

void CBrick::Render() {

	LPANIMATION ani = CAnimations::GetInstance()->Get(510);

	ani->Render(x, y);

}