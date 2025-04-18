#pragma once
#include <unordered_map>
#include <string>
#include <d3dx10.h>

#include "Texture.h"

using namespace std;

/*
	Manage texture database
*/
class CTextures
{
	static CTextures* __instance;

	unordered_map<string, LPTEXTURE> textures;

public:
	CTextures();
	void Add(string id, LPCWSTR filePath);
	LPTEXTURE Get(string id);
	void Clear();

	static CTextures* GetInstance();
};