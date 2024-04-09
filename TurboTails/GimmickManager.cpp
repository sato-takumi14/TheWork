
#include"Main.h"

#include "GimmickManager.h"
#include"StageManager.h"

#include"Scene.h"
#include"CreateInstance.h"

/*ギミック*/
#include"MoveFloor.h"
#include"Trampoline.h"
//#include"Ring.h"
#include"Scaffold.h"
#include"DashPanel.h"
#include"Random.h"
#include"Elevator.h"
#include"Balloon.h"
#include"Goal.h"
#include"Ring.h"

#include"StageSelectObject.h"

void GimmickManager::Init()
{
	
	/*ギミックのインスタンスを追加*/
	{
		CreateInstance::AddGameObject<MoveFloor>("MoveFloor");
		CreateInstance::AddGameObject<Trampoline>("Trampoline");
		CreateInstance::AddGameObject<Scaffold>("Scaffold");
		CreateInstance::AddGameObject<DashPanel>("DashPanel");
		CreateInstance::AddGameObject<Elevator>("Elevator");
		CreateInstance::AddGameObject<Balloon>("Balloon");
		CreateInstance::AddGameObject<Goal>("Goal");
		CreateInstance::AddGameObject<StageSelectObject>("StageSelectObject");
		CreateInstance::AddGameObject<Ring>("Ring");
	}

	{

		Manager::CreateMultiGameObject("Trampoline", 1, mStageManager->GetTransforminfos("Trampoline"));
		Manager::CreateMultiGameObject("Scaffold", 1, mStageManager->GetTransforminfos("Scaffold"));
		Manager::CreateGameObjects("Balloon", mStageManager->GetTransforminfos("Balloon"));
		Manager::CreateGameObjects("StageSelectObject", mStageManager->GetTransforminfos("StageSelectObject"));
		Manager::CreateGameObject("Goal", mStageManager->GetTransforminfos("Goal"));
		Manager::CreateMultiGameObject("Ring", 1, mStageManager->GetTransforminfos("Ring"));

	}

}

void GimmickManager::Update()
{

	

}

void GimmickManager::Uninit()
{
}
