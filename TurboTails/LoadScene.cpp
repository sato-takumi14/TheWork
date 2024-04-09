
#include"Main.h"

#include "LoadScene.h"

#include"LoadObject.h"
#include"Post.h"
#include"SceneChangeObject.h"
#include"TestScene.h"
#include"SceneManager.h"
#include"CreateInstance.h"

void LoadScene::Load()
{
	lock_guard<mutex> lock(this->mMutex);
	this->mLoadScene->Init();
	this->mLoad = true;
}

void LoadScene::Init()
{

	SetStageFilePath("load");
	SetName("load");

	{
		int index = 0;
		mLayer.AddLayer(Order{ RendererPath::begindepth,index++,"depth" });
		mLayer.AddLayer(Order{ RendererPath::beginbg,index++,"bg" });
		mLayer.AddLayer(Order{ RendererPath::beginpp,index++,"3D" });
		mLayer.AddLayer(Order{ RendererPath::beginpp,index++,"2D" });
		mLayer.AddLayer(Order{ RendererPath::begin,index++,"post" });
	}

	{

		AddGameObject(new LoadObject());
		AddGameObject(new Post());

		mLoadScene = CreateInstance::CreateScene("Tutorial");

		mLoad = false;

		thread th(&LoadScene::Load, this);
		th.detach();

		mTime = 0;

	}

}

void LoadScene::Update()
{
	Scene::Update();

	if (mLoad && mTime++ >= 20) {

		SceneManager::SetSceneLoad(mLoadScene);

	}

}

void LoadScene::Uninit()
{
	Scene::Uninit();
}
