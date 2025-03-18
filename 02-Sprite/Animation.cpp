#include "Animation.h"

void CAnimation::Add(vector<int> spriteIds, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	vector<LPSPRITE> sprites;
	for (int i = 0; i < spriteIds.size(); i++)
	{
		sprites.push_back(CSprites::GetInstance()->Get(spriteIds[i]));
	}
	LPANIMATION_FRAME frame = new CAnimationFrame(sprites, t);
	frames.push_back(frame);
}

void CAnimation::Render(float x, float y)
{
	ULONGLONG now = GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
			//DebugOut(L"now: %d, lastFrameTime: %d, t: %d\n", now, lastFrameTime, t);
		}

	}

	vector<LPSPRITE> sprites = frames[currentFrame]->GetSprites();
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i]->Draw(x, y);
	}
}

