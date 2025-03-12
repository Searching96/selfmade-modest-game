#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

#include "Texture.h"
#include "helper.h"

using namespace std;

class CGameObject
{
protected:
	float x;
	float y;

	// This should be a pointer to an object containing all graphic/sound/audio assets for rendering this object. 
	// For now, just a pointer to a single texture
	LPTEXTURE texture;
public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	float GetX() { return x; }
	float GetY() { return y; }

	CGameObject(float x = 0.0f, float y = 0.0f, LPTEXTURE texture = NULL);

	virtual void Update(DWORD dt) = 0;
	virtual void Render();

	~CGameObject();
};
typedef CGameObject* LPGAMEOBJECT;

class CMoveableObject : public CGameObject
{
protected:
	float vx;
	float vy;
public:
	CMoveableObject(float x, float y, float vx, float vy, LPTEXTURE texture) : CGameObject(x, y, texture)
	{
		this->vx = vx;
		this->vy = vy;
	}
};

class CBullet : public CMoveableObject
{
protected:
	bool isActive = false;
	int dir;
public:
	CBullet(float x, float y, float vx, float vy, LPTEXTURE texture, int dir) : CMoveableObject(x, y, vx, vy, texture) {}
	bool GetIsActive() { return isActive; }
	void SetIsActive(bool isActive) { this->isActive = isActive; }
	void SetX(float x) { this->x = x; }
	void SetY(float y) { this->y = y; }
	void SetVX(float vx) { this->vx = vx; }
	void SetVY(float vy) { this->vy = vy; }
	void SetDir(int dir) { this->dir = dir; }
	void Update(DWORD dt);
	void Render();
	LPTEXTURE GetTexture() { return texture; }
};

class CPlayer : public CMoveableObject
{
protected:
	vector<LPTEXTURE> listTexture;
	CBullet* bullet;
	bool isAlive = true;
	int health = 10;
	int dir;
public:
	CPlayer(float x, float y, float vx, float vy, LPTEXTURE texture,
		vector<LPTEXTURE> listTexture,
		CBullet* bullet, int dir) : CMoveableObject(x, y, vx, vy, texture)
	{
		this->texture = texture;
		this->listTexture = listTexture;
		this->bullet = new CBullet(*bullet);
		this->dir = dir;
	}
	CBullet* GetBullet() { return bullet; }
	void Update(DWORD dt);
	void Render();
	void CheckCollision(CBullet* bullet);
	void CheckBulletCollisionWithEnemyBullet(CBullet* bullet);
	bool GetIsAlive() { return isAlive; }
	int GetDir() { return dir; }
	int GetHealth() { return health; }
};

class CEnemy : public CMoveableObject
{
protected:
	vector<LPTEXTURE> listTexture;
	CBullet* bullet;
	int dir;
	int health;
	DWORD lastChangeTime;
	int currentDirection;
	DWORD duration;
public:
	CEnemy::CEnemy(float x, float y, float vx, float vy, LPTEXTURE texture,
		vector<LPTEXTURE> listTexture,
		CBullet* bullet, int health, int dir) : CMoveableObject(x, y, vx, vy, texture)
	{
		this->texture = texture;
		this->listTexture = listTexture;
		this->bullet = new CBullet(*bullet);
		this->dir = dir;
		this->health = health;
		this->lastChangeTime = GetTickCount();
		this->duration = getRandomInt(1000, 3000);
	}
	CBullet* GetBullet() { return bullet; }
	void Update(DWORD dt);
	void Render();
	void CheckCollision(CBullet* bullet);
};