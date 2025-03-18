#pragma once

#include "Sprite.h"

#include <vector>

using namespace std;

/*
	Sprite animation
*/
class CAnimationFrame
{
	vector<LPSPRITE> sprites;
	DWORD time;

public:
	CAnimationFrame(vector<LPSPRITE> sprites, int time) { this->sprites = sprites; this->time = time; }
	DWORD GetTime() { return time; }
	vector<LPSPRITE> GetSprites() { return sprites; }
};

typedef CAnimationFrame* LPANIMATION_FRAME;

