#include "Door.h"

void CDoor::Render()
{
	LPANIMATION ani = CAnimations::GetInstance()->Get(530); // to change id here
	ani->Render(x, y);
}
