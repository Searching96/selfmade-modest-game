#include "Door.h"

void CDoor::Render()
{
	LPANIMATION ani = CAnimations::GetInstance()->Get(530);
	if (isOpened)
		ani = CAnimations::GetInstance()->Get(531);
	//if (!isOpened)
	//	CAnimations::GetInstance()->Get(530);
	//else
	//	CAnimations::GetInstance()->Get(531);
	ani->Render(x, y);
}
