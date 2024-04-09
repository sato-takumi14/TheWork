
#include "Main.h"
#include "SceneManager.h"
#include"System\Renderer.h"

#include"Scene\TestScene.h"
#include"System\InputKey.h"
#include"Component\Time.h"
#include"System\File.h"
#include"System\Debug.h"
#include"System\Input.h"

Scene* SceneManager::mScene{};
Scene* SceneManager::mNextScene{};

void SceneManager::MasterInit()
{
	Renderer::MasterInit();
	InputKey::MasterInit();
	Time::MasterInit();
	File::ExcelDevice::MasterInit();
	if (Input::Device::Connect())Debug::Device::Log("connect");
	else Debug::Device::Log("not connect");
	mScene = new TestScene();
	mScene->Init();
}

void SceneManager::MasterUpdate()
{

	Time::MasterUpdate();
	Input::Device::MasterUpdate();

	mScene->Update();
	mScene->FixedUpdate();

	if (Input::Device::GamepadTrigger(GamepadButton::L)) {
		Debug::Device::Log("ƒvƒŒƒX");
	}

}

void SceneManager::MasterDraw()
{
	mScene->Draw();
}

void SceneManager::MasterUninit()
{
	mScene->Uninit();
	delete mScene;
	File::ExcelDevice::MasterUninit();
	Time::MasterUninit();
	InputKey::MasterUninit();
	Renderer::MasterUninit();
	Input::Device::MasterUninit();
}

void SceneManager::SceneChange(Scene* scene)
{
	
}
