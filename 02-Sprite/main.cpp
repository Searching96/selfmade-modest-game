/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 02 - SPRITE AND ANIMATION

	This sample illustrates how to:

		1/ Render a sprite (within a sprite sheet)
		2/ How to manage sprites/animations in a game
		3/ Enhance CGameObject with sprite animation
================================================================ */

#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>

#include "debug.h"
#include "Game.h"
#include "Textures.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Animation.h"
#include "Animations.h"


#include "Mario.h"
#include "Enemy.h"
#include "Door.h"


#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"02 - Sprite animation"
#define WINDOW_ICON_PATH L"mario.ico"

#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255,0.0f)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_ENEMY_REVERSED 30

#define TEXTURES_DIR L"textures"
#define TEXTURE_PATH_MARIO TEXTURES_DIR "\\mario_transparent.png"
#define TEXTURE_PATH_MISC TEXTURES_DIR "\\misc_transparent.png"
#define TEXTURE_PATH_ENEMIES TEXTURES_DIR "\\enemies_transparent.png"
#define TEXTURE_PATH_ENEMIES_REVERSED TEXTURES_DIR "\\enemies_transparent_reversed.png"

CMario *mario;
#define MARIO_START_X 10.0f
#define MARIO_START_Y 130.0f
#define MARIO_BASE_SPEED 0.1f

CEnemy* enemy;
#define ENEMY_START_X 10.0f
#define ENEMY_START_Y 150.0f
#define ENEMY_BASE_SPEED 0.1f

CBrick *brick;

// this external var is not a good practice and will be change later
extern CDoor* door = NULL; // we must assign door to a value, �f not, there will be error

vector<CGameObject*> objects;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources 
	In this example: load textures, sprites, animations and mario object
*/
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_MARIO, TEXTURE_PATH_MARIO);
	//textures->Add(ID_ENEMY_TEXTURE, TEXTURE_PATH_ENEMIES, D3DCOLOR_XRGB(156, 219, 239));
	textures->Add(ID_TEX_MISC, TEXTURE_PATH_MISC);
	textures->Add(ID_TEX_ENEMY, TEXTURE_PATH_ENEMIES);
	textures->Add(ID_TEX_ENEMY_REVERSED, TEXTURE_PATH_ENEMIES_REVERSED);


	CSprites * sprites = CSprites::GetInstance();
	
	LPTEXTURE texMario = textures->Get(ID_TEX_MARIO);

	// readline => id, left, top, right 

	sprites->Add(10001, 246, 154, 259, 181, texMario, 8, 8);
	sprites->Add(10002, 275, 154, 290, 181, texMario, 8, 8);
	sprites->Add(10003, 304, 154, 321, 181, texMario, 8, 8);

	sprites->Add(10011, 186, 154, 200, 181, texMario);
	sprites->Add(10012, 155, 154, 171, 181, texMario);
	sprites->Add(10013, 125, 154, 141, 181, texMario);

	sprites->Add(10021, 245, 153, 260, 181, texMario);

	sprites->Add(10031, 185, 153, 200, 181, texMario);

	CAnimations * animations = CAnimations::GetInstance();
	LPANIMATION ani;

	// mario moving left
	ani = new CAnimation(100);
	ani->Add({ 10001 });
	ani->Add({ 10002 });
	ani->Add({ 10003 });
	animations->Add(500, ani);


	// mario moving right
	ani = new CAnimation(100);
	ani->Add({ 10011 });
	ani->Add({ 10012 });
	ani->Add({ 10013 });
	animations->Add(501, ani);

	// mario idling left
	ani = new CAnimation(100);
	ani->Add({ 10021 });
	animations->Add(502, ani);

	// mario idling right
	ani = new CAnimation(100);
	ani->Add({ 10031 });
	animations->Add(503, ani);


	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(20001, 300, 135, 317, 150, texMisc);
	sprites->Add(20002, 318, 135, 335, 150, texMisc);
	sprites->Add(20003, 336, 135, 353, 150, texMisc);
	sprites->Add(20004, 354, 135, 371, 150, texMisc);

	ani = new CAnimation(100);
	ani->Add({ 20001 }, 1000);
	ani->Add({ 20002 });
	ani->Add({ 20003 });
	ani->Add({ 20004 });
	animations->Add(510, ani);
	
	
	LPTEXTURE texEnemy = textures->Get(ID_TEX_ENEMY);
	sprites->Add(30001, 6, 131, 23, 157, texEnemy);
	sprites->Add(30002, 28, 129, 45, 157, texEnemy);
	
	ani = new CAnimation(100);
	ani->Add({ 30001 });
	ani->Add({ 30002 });
	animations->Add(520, ani);
	
	LPTEXTURE texEnemyReversed = textures->Get(ID_TEX_ENEMY_REVERSED);
	sprites->Add(30003, 456, 131, 473, 157, texEnemyReversed);
	sprites->Add(30004, 434, 129, 451, 157, texEnemyReversed);

	ani = new CAnimation(100);
	ani->Add({ 30003 });
	ani->Add({ 30004 });
	animations->Add(521, ani);

	// add door sprites from texMisc
	// closed door
	sprites->Add(40001, 282, 99, 297, 114, texMisc, 0, -16); // upper part
	sprites->Add(40002, 282, 117, 297, 132, texMisc);

	// one third opened door
	sprites->Add(40003, 285, 135, 297, 150, texMisc, 0, -16); // upper part
	sprites->Add(40004, 285, 153, 297, 168, texMisc);

	// two third opened door
	sprites->Add(40005, 285, 171, 297, 186, texMisc, 0, -16); // upper part
	sprites->Add(40006, 285, 189, 297, 204, texMisc);

	// fully opened door
	sprites->Add(40007, 285, 207, 297, 222, texMisc, 0, -16); // upper part
	sprites->Add(40008, 285, 225, 297, 240, texMisc);

	// closed door ani
	ani = new CAnimation(100);
	ani->Add({ 40001, 40002 });
	animations->Add(530, ani);
	
	// opening door ani
	ani = new CAnimation(100);
	ani->Add({ 40003, 40004 });
	ani->Add({ 40005, 40006 });
	ani->Add({ 40007, 40008 }, 1000);
	animations->Add(531, ani);

	mario = new CMario(MARIO_START_X, MARIO_START_Y, MARIO_BASE_SPEED);
	enemy = new CEnemy(ENEMY_START_X, ENEMY_START_Y, ENEMY_BASE_SPEED);
	brick = new CBrick(100.0f, 100.0f);
	door = new CDoor(120.0f, 136.0f);

	objects.push_back(mario);
	objects.push_back(enemy);
	objects.push_back(brick);
	objects.push_back(door);
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt);
	}
}

void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	if (pD3DDevice != NULL)
	{
		// clear the background 
		pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

		// Use Alpha blending for transparent sprites
		FLOAT NewBlendFactor[4] = { 0,0,0,0 };
		pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Render();
		}

		// Uncomment this line to see how to draw a porttion of a texture  
		//g->Draw(10, 10, texMisc, 300, 117, 316, 133);


		spriteHandler->End();
		pSwapChain->Present(0, 0);
	}
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		DWORD ErrCode = GetLastError();
		DebugOut(L"[ERROR] CreateWindow failed! ErrCode: %d\nAt: %s %d \n", ErrCode, _W(__FILE__), __LINE__);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetDebugWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		ULONGLONG dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update((DWORD)dt);
			Render();
		}
		else
			Sleep((DWORD)(tickPerFrame - dt));
	}

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	CGame *game = CGame::GetInstance();
	game->Init(hWnd);

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}