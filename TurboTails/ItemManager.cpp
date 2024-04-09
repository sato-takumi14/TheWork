
#include"Main.h"

#include "ItemManager.h"

#include"StageManager.h"
#include"Scene.h"
#include"GameObject.h"

#include"Jewel.h"
#include"CreateInstance.h"
#include"Key.h"

void ItemManager::Init()
{

	{
		CreateInstance::AddGameObject<Jewel>("Jewel");
		CreateInstance::AddGameObject<Key>("Key");
	}

	{

		Manager::CreateGameObject("Key", mStageManager->GetTransforminfos("Key"));
		Manager::CreateGameObjects("Jewel", mStageManager->GetTransforminfos("Jewel"));

	}

}

void ItemManager::Start()
{
	mPlayer = mScene->GetGameObjectTag("Player");
}

void ItemManager::Update()
{
	mItems.remove_if([](GameObject* item) { return item->GetDestroy(); });
}

void ItemManager::Uninit()
{
	mItems.clear();
}
