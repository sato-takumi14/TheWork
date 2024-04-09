
#include"Main.h"

#include<thread>

#include "SceneManager.h"
#include"Scene.h"
#include"Renderer.h"
#include"Input.h"
#include"Time.h"
#include"TestScene.h"
#include"Title.h"
#include"LoadScene.h"
#include"CreateInstance.h"
#include"Load.h"
#include"Tutorial.h"
#include"Result.h"
#include"ActionKey.h"
#include"CRIAudio.h"
#include"StageSelect.h"

Scene* SceneManager::mScene{};
Scene* SceneManager::mNextScene{};
Scene* SceneManager::mNextSceneLoad{};

void SceneManager::Init()
{

	Renderer::Init();
	Time::MasterInit();
	CreateInstance::Init();
	CRIAudio::InitMaster();

	{
		CreateInstance::AddScene<LoadScene>("LoadScene");
		CreateInstance::AddScene<Title>("Title");
		CreateInstance::AddScene<TestScene>("TestScene");
		CreateInstance::AddScene<Tutorial>("Tutorial");
		CreateInstance::AddScene<Result>("Result");
		CreateInstance::AddScene<StageSelect>("StageSelect");
	}

	SceneChange(CreateInstance::CreateScene("Title"));

}

void SceneManager::Update()
{

	Input::Update();
	Time::MasterUpdate();

	if (mNextScene) {
		if (mScene) {
			mScene->Uninit();
			delete mScene;
		}
		mScene = mNextScene;
		mScene->Init();
		mNextScene = nullptr;
	}

	if (mNextSceneLoad) {

		if (mScene && Load::GetFinish()) {

			mScene->Uninit();
			delete mScene;

			mScene = mNextSceneLoad;
			Load::Unload();
			mNextSceneLoad = nullptr;

		}

	}

	mScene->Start();
	mScene->Update();

}

void SceneManager::Draw()
{
	mScene->Draw();
}

void SceneManager::Uninit()
{
	mScene->Uninit();
	delete mScene;

	CreateInstance::Uninit();
	Input::Uninit();
	Renderer::Uninit();
	ActionKey::Uninit();
	CRIAudio::UninitMaster();
	ModelManager::Uninit();
	TextureManager::Uninit();
	Shader::Uninit();

}

void SceneManager::SceneChange(Scene* next)
{
	if (mScene) {
		mScene->Uninit();
		delete mScene;
	}
	mScene = next;
	mScene->Init();
}

void SceneManager::SetSceneLoad(Scene* next)
{
	mNextSceneLoad = next;
}

void SceneManager::SetNextSceneLoad(Scene* next)
{
	mNextScene = next;
}

void SceneManager::SetLoadingNextScene(Scene* next)
{

	mNextSceneLoad = next;

	thread th(Load::Loading, mNextSceneLoad);
	th.detach();

}
