#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT "a_m_b_i_r"
#define ID_ANI_MARIO_IDLE_LEFT "a_m_b_i_l"

#define ID_ANI_MARIO_WALKING_RIGHT "a_m_b_w_r"
#define ID_ANI_MARIO_WALKING_LEFT "a_m_b_w_l"

#define ID_ANI_MARIO_RUNNING_RIGHT "a_m_b_r_r"
#define ID_ANI_MARIO_RUNNING_LEFT "a_m_b_r_l"

#define ID_ANI_MARIO_JUMP_WALK_RIGHT "a_m_b_j_w_r"
#define ID_ANI_MARIO_JUMP_WALK_LEFT "a_m_b_j_w_l"

#define ID_ANI_MARIO_JUMP_RUN_RIGHT "a_m_b_j_r_r"
#define ID_ANI_MARIO_JUMP_RUN_LEFT "a_m_b_j_r_l"

#define ID_ANI_MARIO_SIT_RIGHT "a_m_b_s_r"
#define ID_ANI_MARIO_SIT_LEFT "a_m_b_s_l"

#define ID_ANI_MARIO_BRACE_RIGHT "a_m_b_b_r"
#define ID_ANI_MARIO_BRACE_LEFT "a_m_b_b_l"

#define ID_ANI_MARIO_DIE "a_m_d"

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT "a_m_s_i_r"
#define ID_ANI_MARIO_SMALL_IDLE_LEFT "a_m_s_i_l"

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT "a_m_s_w_r"
#define ID_ANI_MARIO_SMALL_WALKING_LEFT "a_m_s_w_l"

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT "a_m_s_r_r"
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT "a_m_s_r_l"

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT "a_m_s_b_r"
#define ID_ANI_MARIO_SMALL_BRACE_LEFT "a_m_s_b_l"

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT "a_m_s_j_w_r"
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT "a_m_s_j_w_l"

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT "a_m_s_j_r_r"
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT "a_m_s_j_r_l"

#pragma endregion

#define GROUND_Y 160.0f




#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12


#define MARIO_UNTOUCHABLE_TIME 2500

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level; 
	int untouchable; 
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin; 

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);

	string GetAniIdBig();
	string GetAniIdSmall();

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 

		level = MARIO_LEVEL_BIG;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};