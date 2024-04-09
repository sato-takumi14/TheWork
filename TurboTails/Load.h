#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：01/10
*
********************************************************************************************
* Loadクラス：ロードとロード時のテクスチャを描画する
********************************************************************************************/

#include"GameObject.h"

#include<mutex>

class Load : public GameObject
{
private:

	class Scene* mLoad{};
	static mutex mLoadedMutex;

	static bool mFinish;
	static int mTime;

	GameObject* mLoadObject{};
	bool mLoading{};
	string mLoadScene{};

public:

	static void Loading(Scene* load);
	static void Unload();

	static cbool GetFinish() { return mFinish && mTime >= 30; }
	static void SetFinish(crbool set) { mFinish = set; }

	Load() { mActivate = false; }
	Load(crstring next) : mLoadScene(next) {}

	void Init()override;
	void Update()override;
	void Draw()override;
	void Uninit()override;

	void Set(crstring set)override {
		mLoadScene = set;
		SetActivate();
	}

};