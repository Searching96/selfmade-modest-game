#pragma once

#include "GameObject.h"
#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

class CDoor : public CGameObject
{
protected:
    bool isOpened = false;
    ULONGLONG lastOpenTime; // Use ULONGLONG instead of std::chrono

public:
    CDoor(float x, float y) : CGameObject(x, y) {}
    void Render();
    void Update(DWORD dt);
    void SetIsOpened(bool isOpened);
	bool GetIsOpened() { return isOpened; }
	ULONGLONG GetLastOpenTime() { return lastOpenTime; }
};