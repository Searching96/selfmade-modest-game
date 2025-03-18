#include "Door.h"

void CDoor::Render()
{
    LPANIMATION ani = CAnimations::GetInstance()->Get(530);
    if (isOpened)
        ani = CAnimations::GetInstance()->Get(531);
    ani->Render(x, y);
}

void CDoor::Update(DWORD dt)
{
    if (isOpened)
    {
        ULONGLONG now = GetTickCount64();
        if (now - lastOpenTime >= 1200)
        {
            isOpened = false;
        }
    }
}

void CDoor::SetIsOpened(bool isOpened)
{
    this->isOpened = isOpened;
    if (isOpened)
    {
        lastOpenTime = GetTickCount64(); // Set the timestamp when the door is opened
    }
}
