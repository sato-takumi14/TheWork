
#include"Main.h"
#include "Balloon.h"

#include"ModelManager.h"
#include"DrawModel.h"
#include"CircleCollider.h"
#include"TargetManager.h"
#include"Shader.h"
#include"Scene.h"
#include"Time.h"
#include"MultiGameObject.h"

void Balloon::Init()
{

	mLayer.push_back("3D");
	SetTag("Balloon");

	mModelID = ModelManager::LoadModel("asset\\model\\Balloon.obj", "Balloon");

	AddComponent(new CircleCollider(this, Vector3::zero(), 1.0f));

	dynamic_cast<TargetManager*>(mScene->GetManagerTag("TargetManager"))->AddTarget(this);

	mTime = 0;

}

void Balloon::Start()
{
	mDraw = new DrawModel(
		this,
		mModelID,
		"shader\\UnlitTextureVS.cso",
		"shader\\UnlitTexturePS.cso",
		list<int>(),
		Color::normal()
	);
}

void Balloon::Update()
{

	GameObject::Update();

}

void Balloon::OnCollisionEnter(crint id)
{

	auto obj = mScene->GetGameObject(id);
	
	if (obj->GetTag() == "Player") {

		SetDestroy();

		if (mMulti)mMulti->Remove(GameObject::GetObjectID());

	}

}
