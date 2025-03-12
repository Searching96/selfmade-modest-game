#include <d3dx9.h>
#include <vector>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"

using namespace std;

/*
	Initialize game object 
*/
CGameObject::CGameObject(float x, float y, LPTEXTURE tex)
{
	this->x = x;
	this->y = y;
	this->texture = tex;
}

void CGameObject::Render()
{
	CGame::GetInstance()->Draw(x, y, texture);
}

CGameObject::~CGameObject()
{
	if (texture != NULL) delete texture;
}

#define MARIO_WIDTH 14
#define SHIP_WIDTH 16
#define SHIP_HEIGHT 16

void CPlayer::Update(DWORD dt)
{
    // Handle WASD input to change direction
    if (GetAsyncKeyState('A') & 0x8000) // Move left
    {
        x += -vx * dt;
    }
    if (GetAsyncKeyState('D') & 0x8000) // Move right
    {
        x += vx * dt;
    }
    if (GetAsyncKeyState('W') & 0x8000) // Move up
    {
        y += vy * dt;
    }
    if (GetAsyncKeyState('S') & 0x8000) // Move down
    {
        y += -vy * dt;
    }

    int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
    int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();

    // Handle horizontal boundary collisions
    if (x <= 0 || x >= BackBufferWidth - SHIP_WIDTH)
    {
        if (x <= 0)
        {
            x = 0;
        }
        else if (x >= BackBufferWidth - SHIP_WIDTH)
        {
            x = (float)(BackBufferWidth - SHIP_WIDTH);
        }
    }

    // Handle vertical boundary collisions
    if (y <= 0 || y >= BackBufferHeight - SHIP_HEIGHT)
    {
        if (y <= 0)
        {
            y = 0;
        }
        else if (y >= BackBufferHeight - SHIP_HEIGHT)
        {
            y = (float)(BackBufferHeight - SHIP_HEIGHT);
        }
    }

    // Handle shooting
    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        if (!bullet->GetIsActive())
        {
            bullet->SetIsActive(true);
            bullet->SetX(x);
            bullet->SetY(y);
        }
    }

    if (bullet->GetIsActive())
    {
        if (bullet->GetY() <= 0)
        {
            bullet->SetIsActive(false);
        }
    }
    bullet->Update(dt);

	vector<CEnemy*> enemies = CGame::GetInstance()->GetEnemies();
	// Check collioion with enemy bullet
	for (int i = 0; i < enemies.size(); i++)
	{
		CheckCollision(enemies[i]->GetBullet());
	}

	if (health <= 0)
	{
		isAlive = false;
        x = y = vx = vx = 0;
	}
}

void CPlayer::Render()
{
    if (isAlive)
    {
        CGame::GetInstance()->Draw(x, y, texture);
        bullet->Render();
    }
}

void CPlayer::CheckCollision(CBullet* bullet)
{
    float width = texture->getWidth();
	float height = texture->getHeight();
	if (!bullet->GetIsActive())
	{
		return;
	}
    if ((bullet->GetX() >= x - width / 2 && bullet->GetX() <= x + width / 2)
        && (bullet->GetY() >= y - height / 2 && bullet->GetY() <= y + height / 2))
    {
		bullet->SetIsActive(false);
        health--;
    }
}

void CBullet::Update(DWORD dt)
{
    if (isActive)
    {
        y += vy * dt;
    }
}

void CBullet::Render()
{
    if (isActive)
    {
        CGame::GetInstance()->Draw(x, y, texture);
    }
}

void CEnemy::Update(DWORD dt)
{
    x += vx * dt;
    int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
    int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();
    if (x <= 0 || x >= BackBufferWidth)
    {
        vx = -vx; // Reverse horizontal direction

        if (x <= 0)
        {
            x = 0;
        }
        else if (x >= BackBufferWidth)
        {
            x = (float)(BackBufferWidth);
        }
    }

    if (!bullet->GetIsActive())
    {
		bullet->SetIsActive(true);
		bullet->SetX(x);
		bullet->SetY(y);
    }

    if (bullet->GetIsActive())
    {
        int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();
        if (bullet->GetY() >= BackBufferHeight)
        {
            bullet->SetIsActive(false);
        }
    }

    bullet->Update(dt);
    CPlayer* player = CGame::GetInstance()->GetPlayer();
    CheckCollision(player->GetBullet());
}

void CEnemy::CheckCollision(CBullet* bullet)
{
	float width = texture->getWidth();
	float height = texture->getHeight();
	if (!bullet->GetIsActive())
	{
		return;
	}
    if ((bullet->GetX() >= x - width / 2 && bullet->GetX() <= x + width / 2) 
        && (bullet->GetY() >= y - height / 2 && bullet->GetY() <= y + height / 2))
    {
		bullet->SetIsActive(false);
        isAlive = false;
        x = y = vx = vy = 0;
    }
    // DebugOutTitle(L"bullet x, y: %d %d; enemy x, y: %d %d, alive = %d", bullet->GetX(), bullet->GetY(), x, y, alive);
}

void CEnemy::Render()
{
    if (isAlive)
    {
        CGame::GetInstance()->Draw(x, y, texture);
        bullet->Render();
    }
}