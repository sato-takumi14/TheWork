
#include"Main.h"

#include "CreateInstance.h"

#include"GameObject.h"
#include"Scene.h"
#include"Component.h"

unordered_map<string, function<GameObject* ()>> CreateInstance::mGameObject{};
unordered_map<string, function<Scene* ()>> CreateInstance::mScene{};
unordered_map<string, function<Component* ()>> CreateInstance::mComponent{};

void CreateInstance::Init()
{
	
}

void CreateInstance::Uninit()
{
	mGameObject.clear();
	mScene.clear();
	mComponent.clear();
}

GameObject* CreateInstance::CreateObject(crstring name)
{
	return mGameObject[name]();
}

Scene* CreateInstance::CreateScene(crstring name)
{
	return mScene[name]();
}

Component* CreateInstance::CreateComponet(crstring name)
{
	return mComponent[name]();
}

void CreateInstance::GameObjectClear()
{
	mGameObject.clear();
}

void CreateInstance::SceneClear()
{
	mScene.clear();
}

void CreateInstance::ComponentClear()
{
	mComponent.clear();
}
