#pragma once

#include "Texture.h"
#include "Game.h"

class CSprite
{
	int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;

	float x0 = 0;
	float y0 = 0;

	LPTEXTURE texture;
	D3DX10_SPRITE sprite;
	D3DXMATRIX matScaling;
public:
	CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex, float x0 = 0.0f, float y0 = 0.0f);

	void Draw(float x, float y);
};

typedef CSprite* LPSPRITE;