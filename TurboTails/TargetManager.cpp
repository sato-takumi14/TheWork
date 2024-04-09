
#include"Main.h"

#include "TargetManager.h"

#include"Scene.h"
#include"GameObject.h"
#include"TargetObject.h"

void TargetManager::Init()
{

	mScene->AddGameObject(new TargetObject());
	mNowScene = mScene;
	mTag = "TargetManager";

}

void TargetManager::Update()
{

	mTargets.remove_if([](TargetInfoData data) { return data.target->GetDestroy(); });

}

void TargetManager::Uninit()
{

	mTargets.clear();

}

void TargetManager::AddTarget(GameObject* target)
{
	mTargets.push_back(TargetInfoData(target, mNowScene->GetName()));
}

GameObject* TargetManager::GetTarget(crver3 pos)
{

	GameObject* target{};
	float len = 100.0f;

	for (auto& data : mTargets) {

		if (data.target->GetActivate()) {

			ver3 vel = data.target->GetPosition() - pos;
			float length = D3DXVec3Length(&vel);
			if (len > length) {
				len = length;
				target = data.target;
			}

		}

	}

	return target;

}
