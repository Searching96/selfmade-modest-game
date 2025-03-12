/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102

	SAMPLE 01 - SKELETON CODE

	This sample illustrates how to:

	1/ Re-organize introductory code to an initial skeleton for better scalability
	2/ CGame is a singleton, playing a role of an "engine".
	3/ CGameObject is an abstract class for all game objects
	4/ CTexture is a wrapper class for ID3D10TEXTURE

	NOTE: to create transparent background, download GIMP, then use Color to Alpha feature
================================================================ */

#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <vector>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"

#define WINDOW_CLASS_NAME L"Game Window"
#define MAIN_WINDOW_TITLE L"01 - Skeleton"
#define WINDOW_ICON_PATH L"brick.ico"

#define TEXTURE_PATH_PLAYER L"touhougirl.png"
#define TEXTURE_PATH_PLAYER_BULLET L"bullet.png"
#define TEXTURE_PATH_ENEMY L"starship.png"
#define TEXTURE_PATH_ENEMY_BULLET L"enemy_bullet.png"

#define TEXTURE_PATH_MISC L"misc.png"

#define BACKGROUND_COLOR D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.0f)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

using namespace std;

CPlayer* player;
#define PLAYER_START_X 80.0f
#define PLAYER_START_Y 200.0f
#define PLAYER_START_VX 0.1f
#define PLAYER_START_VY -0.1f

CBullet* bullet;
#define BULLET_START_VY -0.2f

vector<CEnemy*> enemies;
CEnemy* enemy1;
CEnemy* enemy2;
#define ENEMY1_START_X 140.0f
#define ENEMY1_START_Y 20.0f
#define ENEMY2_START_X 20.0f
#define ENEMY2_START_Y 40.0f
#define ENEMY_START_VX 0.1f
#define ENEMY_START_VY 0.1f

LPTEXTURE texPlayer = NULL;
LPTEXTURE texPlayerBullet = NULL;
LPTEXTURE texEnemy = NULL;
LPTEXTURE texEnemyBullet = NULL;

vector<LPGAMEOBJECT> objects;

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
	Load all game resources. In this example, create a brick object and mario object
*/
void LoadResources()
{
	CGame* game = CGame::GetInstance();
	texPlayer = game->LoadTexture(TEXTURE_PATH_PLAYER);
	texPlayerBullet = game->LoadTexture(TEXTURE_PATH_PLAYER_BULLET);
	texEnemy = game->LoadTexture(TEXTURE_PATH_ENEMY);
	texEnemyBullet = game->LoadTexture(TEXTURE_PATH_ENEMY_BULLET);

	// player_bullet = new CBullet(0, 0, 0, BULLET_START_VY, texBullet);
	player = new CPlayer(PLAYER_START_X, PLAYER_START_Y, PLAYER_START_VX, PLAYER_START_VY, texPlayer,
		0, 0, 0, BULLET_START_VY, texPlayerBullet);
	enemy1 = new CEnemy(ENEMY1_START_X, ENEMY1_START_Y, ENEMY_START_VX, ENEMY_START_VY, texEnemy,
		0, 0, 0, -BULLET_START_VY, texEnemyBullet);
	enemy2 = new CEnemy(ENEMY2_START_X, ENEMY2_START_Y, ENEMY_START_VX, ENEMY_START_VY, texEnemy,
		0, 0, 0, -BULLET_START_VY, texEnemyBullet);

	objects.push_back(player);
	objects.push_back(enemy1);
	objects.push_back(enemy2);

	enemies.push_back(enemy1);
	enemies.push_back(enemy2);
	game->AddPlayer(player);
	game->AddEnemy(enemy1);
	game->AddEnemy(enemy2);
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{

	for (int i = 0; i < objects.size(); i++)
		objects[i]->Update(dt);

	DebugOutTitle(L"player x, y: %f %f alive = %d health = %d", player->GetX(), player->GetY(), player->GetIsAlive(), player->getHealth());
	//DebugOutTitle(L"01 - Skeleton %0.1f, %0.1f", mario->GetX(), mario->GetY());
}

/*
	Render a frame
*/
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

		for (int i = 0; i < objects.size(); i++) {
			objects[i]->Render();
		}

		// Uncomment this line to see how to draw a porttion of a texture  
		//g->Draw(10, 10, texMisc, 300, 117, 317, 134);
		//g->Draw(10, 10, texMario, 215, 120, 234, 137);

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
)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	CGame* game = CGame::GetInstance();
	game->Init(hWnd);

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);


	LoadResources();

	Run();

	return 0;
}