
#include"Main.h"

#include "Ring.h"

#include"Scene.h"
#include"DrawModel.h"
#include"ModelManager.h"
#include"CircleCollider.h"
#include"Time.h"
#include"TargetManager.h"
#include"UIManager.h"
#include"MultiGameObject.h"

void Ring::Init()
{

	AddComponent(new CircleCollider(this, Vector3::zero(), 1.0f));
	mModelID = ModelManager::LoadModel("asset\\model\\Ring.obj", "Ring");
	mTag = "Ring";
	mAddForce = 2.0f;

}

void Ring::Start()
{

	

}

void Ring::Update()
{

	if (not GameObject::GetActivate()) {

		if (mTime++ >= 100) {

			GameObject::SetActivate(true);
			mTime = 0;

		}

	}

	GameObject::Update();

}

void Ring::OnCollisionEnter(crint id)
{

	auto obj = mScene->GetGameObject(id);

	if (obj->GetTag() == "Player") {

		dynamic_cast<UIManager*>(mScene->GetManagerTag("UIManager"))->GetUI("Score")->Set(5);
		GameObject::SetActivate(false);

	}

}

void Ring::OnCollisionExit(crint id)
{

}
