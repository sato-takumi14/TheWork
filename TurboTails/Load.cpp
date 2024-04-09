
#include<thread>

#include"main.h"
#include "Load.h"

#include"Scene.h"
#include"CreateInstance.h"
#include"LoadObject.h"
#include"ActionKey.h"
#include"SceneManager.h"

bool Load::mFinish = false;
mutex Load::mLoadedMutex{};
int Load::mTime = 0;

void Load::Loading(Scene* load)
{

	lock_guard<mutex> lock(mLoadedMutex);
	load->Init();

	mFinish = true;

}

void Load::Unload()
{

	mFinish = false;
	mTime = 0;

}

void Load::Init()
{
	mLoadObject = new LoadObject();
	mLoadObject->Init();
	mLoading = false;
	mLayer.push_back("2D");
	mTag = "Load";
}

void Load::Update()
{
	if (not mActivate)return;
	GameObject::Update();
	if (ActionKey::Action("NextScene")) {
		Scene* scene = CreateInstance::CreateScene(mLoadScene);
		assert(scene);
		SceneManager::SetLoadingNextScene(scene);
		mLoading = true;
	}
	if (mFinish)mTime++;
	if (mLoading)mLoadObject->Update();
}

void Load::Draw()
{
	GameObject::Draw();
	if (mLoading) mLoadObject->Draw();
}

void Load::Uninit()
{
	GameObject::Uninit();
	if (mLoadObject) {
		mLoadObject->Uninit();
		delete mLoadObject;
	}
}
