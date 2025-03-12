#include <d3dx9.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>

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
    if (GetAsyncKeyState('W') & 0x8000) // Move up
    {
        y += vy * dt;
        dir = 0;
    }
    else if (GetAsyncKeyState('S') & 0x8000) // Move down
    {
        y += -vy * dt;
        dir = 1;
    }
    else if (GetAsyncKeyState('A') & 0x8000) // Move left
    {
        x += -vx * dt;
        dir = 2;
    }
    else if (GetAsyncKeyState('D') & 0x8000) // Move right
    {
        x += vx * dt;
        dir = 3;
    }

	// Set dir for bullet according to player's direction
    if (!bullet->GetIsActive())
    {
        bullet->SetDir(dir);
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

    bullet->Update(dt);

	vector<CEnemy*> enemies = CGame::GetInstance()->GetEnemies();
	// Check collioion with enemies and their bullets
	for (int i = 0; i < enemies.size(); i++)
	{
		CheckCollision(enemies[i]->GetBullet());
        CheckBulletCollisionWithEnemyBullet(enemies[i]->GetBullet());
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
        texture = listTexture[dir];

        CGame::GetInstance()->Draw(x, y, texture);
        bullet->Render();
    }
}

void CPlayer::CheckCollision(CBullet* bullet)
{
    int width = texture->getWidth();
	int height = texture->getHeight();
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

void CPlayer::CheckBulletCollisionWithEnemyBullet(CBullet* eBullet)
{
    if (!this->bullet || !eBullet) {
        return;
    }

    if (!this->bullet->GetIsActive() || !eBullet->GetIsActive()) {
        return;
    }

    float pBulletWidth = static_cast<float>(this->bullet->GetTexture()->getWidth());
    float pBulletHeight = static_cast<float>(this->bullet->GetTexture()->getHeight());
    float pBulletX = this->bullet->GetX();
    float pBulletY = this->bullet->GetY();

    float eBulletWidth = static_cast<float>(eBullet->GetTexture()->getWidth());
    float eBulletHeight = static_cast<float>(eBullet->GetTexture()->getHeight());
    float eBulletX = eBullet->GetX();
    float eBulletY = eBullet->GetY();

    float pTop = pBulletY - pBulletHeight / 2.0f;
    float pBottom = pBulletY + pBulletHeight / 2.0f;
    float pLeft = pBulletX - pBulletWidth / 2.0f;
    float pRight = pBulletX + pBulletWidth / 2.0f;

    float eTop = eBulletY - eBulletHeight / 2.0f;
    float eBottom = eBulletY + eBulletHeight / 2.0f;
    float eLeft = eBulletX - eBulletWidth / 2.0f;
    float eRight = eBulletX + eBulletWidth / 2.0f;

    if (!(pRight < eLeft || pLeft > eRight || pBottom < eTop || pTop > eBottom))
    {
        this->bullet->SetIsActive(false);
        eBullet->SetIsActive(false);
    }
}

void CBullet::Update(DWORD dt)
{
	int width = CGame::GetInstance()->GetBackBufferWidth();
	int height = CGame::GetInstance()->GetBackBufferHeight();
	float bullet_v = max(fabs(vx), fabs(vy));
    if (isActive)
    {
        switch (dir)
        {
		case 0:
			vy = -bullet_v;
            vx = 0;
			break;
		case 1:
			vy = bullet_v;
			vx = 0;
			break;
		case 2:
			vx = -bullet_v;
			vy = 0;
			break;
		case 3:
			vx = bullet_v;
			vy = 0;
			break;
        }

        if (y <= 0 || y >= height || x <= 0 || x >= width)
        {
            isActive = false;
        }
        x += vx * dt;
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
    DWORD currentTime = GetTickCount();
    if (currentTime - lastChangeTime >= duration)
    {
        // Change direction and duration for this enemy instance
        currentDirection = getRandomInt(0, 3);
        duration = getRandomInt(250, 1500);
        lastChangeTime = currentTime;
    }

    // Apply the current direction
    switch (currentDirection)
    {
    case 0: // Move up
        vy = -fabs(vy);
        dir = 0;
        break;
    case 1: // Move down
        vy = fabs(vy);
        dir = 1;
        break;
    case 2: // Move left
        vx = -fabs(vx);
        dir = 2;
        break;
    case 3: // Move right
        vx = fabs(vx);
        dir = 3;
        break;
    }

    if (dir == 0 || dir == 1)
    {
        y += vy * dt;
    }
    else if (dir == 2 || dir == 3)
    {
        x += vx * dt;
    }

    int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
    int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();
	int width = texture->getWidth();
	int height = texture->getHeight();
    if (x <= 0 + width) {
        x = 0 + width;
    }
    else if (x >= BackBufferWidth - width) {
        x = (float)(BackBufferWidth - width);
    }
    if (y <= 0 + height) {
        y = 0 + height;
    }
    else if (y >= BackBufferHeight - height) {
        y = (float)(BackBufferHeight - height);
    }

    // Set bullet direction according to enemy's direction
    if (!bullet->GetIsActive())
    {
        bullet->SetDir(dir);
    }

    // Shooting mechanism
    if (health > 0 && !bullet->GetIsActive())
    {
        bullet->SetIsActive(true);
        bullet->SetX(x);
        bullet->SetY(y);
    }

    if (bullet->GetIsActive())
    {
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
        health--;
    }
    // DebugOutTitle(L"bullet x, y: %d %d; enemy x, y: %d %d, alive = %d", bullet->GetX(), bullet->GetY(), x, y, alive);
}

void CEnemy::Render()
{
    if (health > 0)
    {
        texture = listTexture[(health - 1) * 4 + dir];

        CGame::GetInstance()->Draw(x, y, texture);
        bullet->Render();
    }
}