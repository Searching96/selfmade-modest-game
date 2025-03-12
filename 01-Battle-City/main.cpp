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

#define TEXTURE_PATH_PLAYER_UP L"Sprite/yellow_tank_up.png"
#define TEXTURE_PATH_PLAYER_DOWN L"Sprite/yellow_tank_down.png"
#define TEXTURE_PATH_PLAYER_LEFT L"Sprite/yellow_tank_left.png"
#define TEXTURE_PATH_PLAYER_RIGHT L"Sprite/yellow_tank_right.png"

#define TEXTURE_PATH_ENEMY_3HP_UP L"Sprite/red_tank_up.png"
#define TEXTURE_PATH_ENEMY_3HP_DOWN L"Sprite/red_tank_down.png"
#define TEXTURE_PATH_ENEMY_3HP_LEFT L"Sprite/red_tank_left.png"
#define TEXTURE_PATH_ENEMY_3HP_RIGHT L"Sprite/red_tank_right.png"
#define TEXTURE_PATH_ENEMY_2HP_UP L"Sprite/green_tank_up.png"
#define TEXTURE_PATH_ENEMY_2HP_DOWN L"Sprite/green_tank_down.png"
#define TEXTURE_PATH_ENEMY_2HP_LEFT L"Sprite/green_tank_left.png"
#define TEXTURE_PATH_ENEMY_2HP_RIGHT L"Sprite/green_tank_right.png"
#define TEXTURE_PATH_ENEMY_1HP_UP L"Sprite/white_tank_up.png"
#define TEXTURE_PATH_ENEMY_1HP_DOWN L"Sprite/white_tank_down.png"
#define TEXTURE_PATH_ENEMY_1HP_LEFT L"Sprite/white_tank_left.png"
#define TEXTURE_PATH_ENEMY_1HP_RIGHT L"Sprite/white_tank_right.png"

#define TEXTURE_PATH_PLAYER_BULLET L"bullet.png"
#define TEXTURE_PATH_ENEMY L"starship.png"
#define TEXTURE_PATH_ENEMY_BULLET L"enemy_bullet.png"
#define TEXTURE_PATH_BULLET L"Sprite/bullet.png"

#define TEXTURE_PATH_MISC L"misc.png"

#define BACKGROUND_COLOR D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

using namespace std;

CPlayer* player;
#define PLAYER_START_X 80.0f
#define PLAYER_START_Y 200.0f
#define PLAYER_START_VX 0.1f
#define PLAYER_START_VY -0.1f

CBullet* bullet;
#define BULLET_V 0.2f

//vector<CEnemy*> enemies;
CEnemy* enemy;
//CEnemy* enemy2;
#define ENEMY1_START_X 140.0f
#define ENEMY1_START_Y 20.0f
#define ENEMY2_START_X 20.0f
#define ENEMY2_START_Y 40.0f
#define ENEMY_START_VX 0.1f
#define ENEMY_START_VY 0.1f

LPTEXTURE texPlayer = NULL;
LPTEXTURE texPlayerUp = NULL;
LPTEXTURE texPlayerDown = NULL;
LPTEXTURE texPlayerLeft = NULL;
LPTEXTURE texPlayerRight = NULL;

LPTEXTURE texEnemy = NULL;
LPTEXTURE texEnemy3HpUp = NULL;
LPTEXTURE texEnemy3HpDown = NULL;
LPTEXTURE texEnemy3HpLeft = NULL;
LPTEXTURE texEnemy3HpRight = NULL;
LPTEXTURE texEnemy2HpUp = NULL;
LPTEXTURE texEnemy2HpDown = NULL;
LPTEXTURE texEnemy2HpLeft = NULL;
LPTEXTURE texEnemy2HpRight = NULL;
LPTEXTURE texEnemy1HpUp = NULL;
LPTEXTURE texEnemy1HpDown = NULL;
LPTEXTURE texEnemy1HpLeft = NULL;
LPTEXTURE texEnemy1HpRight = NULL;

LPTEXTURE texBullet = NULL;

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
	texPlayerUp = game->LoadTexture(TEXTURE_PATH_PLAYER_UP);
	texPlayerDown = game->LoadTexture(TEXTURE_PATH_PLAYER_DOWN);
	texPlayerLeft = game->LoadTexture(TEXTURE_PATH_PLAYER_LEFT);
	texPlayerRight = game->LoadTexture(TEXTURE_PATH_PLAYER_RIGHT);
	texPlayer = texPlayerUp;

	texEnemy3HpUp = game->LoadTexture(TEXTURE_PATH_ENEMY_3HP_UP);
	texEnemy3HpDown = game->LoadTexture(TEXTURE_PATH_ENEMY_3HP_DOWN);
	texEnemy3HpLeft = game->LoadTexture(TEXTURE_PATH_ENEMY_3HP_LEFT);
	texEnemy3HpRight = game->LoadTexture(TEXTURE_PATH_ENEMY_3HP_RIGHT);
	texEnemy2HpUp = game->LoadTexture(TEXTURE_PATH_ENEMY_2HP_UP);
	texEnemy2HpDown = game->LoadTexture(TEXTURE_PATH_ENEMY_2HP_DOWN);
	texEnemy2HpLeft = game->LoadTexture(TEXTURE_PATH_ENEMY_2HP_LEFT);
	texEnemy2HpRight = game->LoadTexture(TEXTURE_PATH_ENEMY_2HP_RIGHT);
	texEnemy1HpUp = game->LoadTexture(TEXTURE_PATH_ENEMY_1HP_UP);
	texEnemy1HpDown = game->LoadTexture(TEXTURE_PATH_ENEMY_1HP_DOWN);
	texEnemy1HpLeft = game->LoadTexture(TEXTURE_PATH_ENEMY_1HP_LEFT);
	texEnemy1HpRight = game->LoadTexture(TEXTURE_PATH_ENEMY_1HP_RIGHT);
	texEnemy = texEnemy3HpDown;

	texBullet = game->LoadTexture(TEXTURE_PATH_BULLET);

	bullet = new CBullet(0, 0, 0, BULLET_V, texBullet, 0);

	vector<LPTEXTURE> listPlayerTexture;
	listPlayerTexture.push_back(texPlayerUp);
	listPlayerTexture.push_back(texPlayerDown);
	listPlayerTexture.push_back(texPlayerLeft);
	listPlayerTexture.push_back(texPlayerRight);

	vector<LPTEXTURE> listEnemyTexture;
	listEnemyTexture.push_back(texEnemy1HpUp);
	listEnemyTexture.push_back(texEnemy1HpDown);
	listEnemyTexture.push_back(texEnemy1HpLeft);
	listEnemyTexture.push_back(texEnemy1HpRight);
	listEnemyTexture.push_back(texEnemy2HpUp);
	listEnemyTexture.push_back(texEnemy2HpDown);
	listEnemyTexture.push_back(texEnemy2HpLeft);
	listEnemyTexture.push_back(texEnemy2HpRight);
	listEnemyTexture.push_back(texEnemy3HpUp);
	listEnemyTexture.push_back(texEnemy3HpDown);
	listEnemyTexture.push_back(texEnemy3HpLeft);
	listEnemyTexture.push_back(texEnemy3HpRight);

	player = new CPlayer(PLAYER_START_X, PLAYER_START_Y, PLAYER_START_VX, PLAYER_START_VY, texPlayer,
		listPlayerTexture,
		bullet, 0);

	game->AddObject(player);

	//enemy = new CEnemy(ENEMY1_START_X, ENEMY1_START_Y, 0, 0, listEnemyTexture[2],
	//	listEnemyTexture,
	//	bullet, 1, 3);

	//game->AddObject(enemy);


	srand(time(NULL));
	for (int i = 0; i < 7; i++)
	{
		int width = CGame::GetInstance()->GetBackBufferWidth();
		int height = CGame::GetInstance()->GetBackBufferHeight();
		float x = rand() % width;
		float y = rand() % height;
		int dir = rand() % 4;
		int health = rand() % 3 + 1;
		LPTEXTURE randomizedTex = listEnemyTexture[(health - 1) * 4 + dir];

		game->AddObject(new CEnemy(x, y, ENEMY_START_VX, ENEMY_START_VY, randomizedTex,
			listEnemyTexture,
			bullet, health, dir));
	}
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	vector<CGameObject*> objects = CGame::GetInstance()->GetObjects();
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Update(dt);

	DebugOutTitle(L"player x, y: %f %f alive = %d health = %d", player->GetX(), player->GetY(), player->GetIsAlive(), player->GetHealth());
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

		vector<CGameObject*> objects = g->GetObjects();
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