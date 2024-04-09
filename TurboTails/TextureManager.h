#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* TextureManagerクラス：テクスチャを管理するクラス
********************************************************************************************/

#include"Main.h"
#include"ManagerData.h"

struct TexData : public Data {
	ID3D11ShaderResourceView* tex;
};

class TextureManager
{
private:

	static unordered_map<uint, TexData> mData;
	static int mIndex;

	static class Scene* mScene;

public:

	static void Uninit();
	static void Uninit(crstring scene);

	static void SetScene(Scene* scene) { mScene = scene; }

	static cuint Load(crstring file, crstring name);
	static ID3D11ShaderResourceView** GetTexture(cruint id);

};