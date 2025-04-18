#include <Windows.h>

#include "debug.h"
#include "Game.h"
#include "textures.h"

CTextures* CTextures::__instance = NULL;

CTextures::CTextures()
{

}

CTextures* CTextures::GetInstance()
{
	if (__instance == NULL) __instance = new CTextures();
	return __instance;
}

void CTextures::Add(string id, LPCWSTR filePath)
{
	textures[id] = CGame::GetInstance()->LoadTexture(filePath);
}

LPTEXTURE CTextures::Get(string id)
{
	LPTEXTURE t = textures[id];
	if (t == NULL)
		DebugOut(L"[ERROR] Texture Id %s not found !\n", id);
	
	return t;
}

/*
	Clear all loaded textures
*/
void CTextures::Clear()
{
	for (auto x : textures)
	{
		LPTEXTURE tex = x.second;
		if (tex != NULL) delete tex;
	}

	textures.clear();
}



