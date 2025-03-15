#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#define ENEMY_WIDTH 14

class CEnemy : public CGameObject
{
protected:
	float vx;
public:
	CEnemy(float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
};