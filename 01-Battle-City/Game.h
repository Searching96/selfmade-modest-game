#pragma once
#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <vector>

#include "Texture.h"
#include "GameObject.h"

#define MAX_FRAME_RATE 60

using namespace std;

/*
	Our simple game framework 
*/
class CGame
{
	static CGame * __instance;
	HWND hWnd;									// Window handle

	int backBufferWidth = 0;					// Backbuffer width & height, will be set during Direct3D initialization
	int backBufferHeight = 0;

	ID3D10Device* pD3DDevice = NULL;
	IDXGISwapChain* pSwapChain = NULL;
	ID3D10RenderTargetView* pRenderTargetView = NULL;
	ID3D10BlendState* pBlendStateAlpha = NULL;			// To store alpha blending state

	ID3DX10Sprite* spriteObject = NULL;				// Sprite handling object 

	vector<CGameObject*> objects;

public:
	// Init DirectX, Sprite Handler
	void Init(HWND hWnd);

	//
	// Draw a portion or ALL the texture at position (x,y) on the screen
	// rect : if NULL, the whole texture will be drawn
	//        if NOT NULL, only draw that portion of the texture 
	void Draw(float x, float y, LPTEXTURE tex, RECT *rect = NULL);

	void Draw(float x, float y, LPTEXTURE tex, int l, int t, int r, int b)
	{
		RECT rect; 
		rect.left = l;
		rect.top = t;
		rect.right = r;
		rect.bottom = b;
		this->Draw(x, y, tex, &rect);
	}

	LPTEXTURE LoadTexture(LPCWSTR texturePath);

	ID3D10Device * GetDirect3DDevice() { return this->pD3DDevice; }
	IDXGISwapChain* GetSwapChain() { return this->pSwapChain; }
	ID3D10RenderTargetView* GetRenderTargetView() { return this->pRenderTargetView;  }
	ID3DX10Sprite* GetSpriteHandler() { return this->spriteObject; }
	ID3D10BlendState* GetAlphaBlending() { return pBlendStateAlpha; };

	int GetBackBufferWidth() { return backBufferWidth; }
	int GetBackBufferHeight() { return backBufferHeight; }

	static CGame * GetInstance();

	void AddObject(CGameObject* object) { this->objects.push_back(object); }
	CPlayer* GetPlayer() 
	{
		for (CGameObject* obj : objects)
		{
			CPlayer* player = dynamic_cast<CPlayer*>(obj);
			if (player != NULL)
			{
				return player;
			}
		}
	}
	vector<CEnemy*> GetEnemies() 
	{
		vector<CEnemy*> enemies;

		for (CGameObject* obj : objects)
		{
			CEnemy* enemy = dynamic_cast<CEnemy*>(obj);
			if (enemy != NULL)
			{
				enemies.push_back(enemy);
			}
		}

		return enemies;
	}
	vector<CGameObject*> GetObjects() { return objects; }
	~CGame();
};