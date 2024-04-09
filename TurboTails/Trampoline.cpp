
#include"Main.h"

#include "Trampoline.h"

#include"Scene.h"
#include"DrawModel.h"
#include"Player.h"
#include"TargetManager.h"
#include"CircleCollider.h"
#include"MeshField.h"

ver3 Trampoline::mVel{};

void Trampoline::Init()
{

	mTag = "Trampoline";
	mClass = "Trampoline";

	mModelID = ModelManager::LoadModel("asset\\model\\Spring.obj", "Trampoline");

	dynamic_cast<TargetManager*>(mScene->GetManagerTag("TargetManager"))->AddTarget(this);

	AddComponent(new CircleCollider(this, Vector3::zero(), 1.0f));

}

void Trampoline::Start()
{
	Shader::AddShader("multi", "MultiVertexLightingVS.cso", "MultiVertexLightingPS.cso");
	mField = mScene->GetGameObject<MeshField>("MeshField");
	float height = mField->GetHeight(mTransform.pos);
	if (height > mTransform.pos.y)mTransform.pos.y = height;
}

void Trampoline::Update()
{

	if (mPlayer) {

		mVel.y -= 0.05f;
		mPlayer->AddForce(mVel);
		ver3 pos = mPlayer->GetPosition();
		float height = mField->GetHeight(pos);
		if (height >= pos.y || mTransform.pos.y >= pos.y) {
			dynamic_cast<Player*>(mPlayer)->SetStop(false);
			pos.y = height;
			mPlayer->SetPosition(pos);
			mPlayer->Set(false);
			mPlayer->AddForce(Vector3::zero());
			mPlayer = nullptr;
		}

	}

}

void Trampoline::OnCollisionEnter(crint id)
{

	auto obj = mScene->GetGameObject(id);

	if (obj->GetTag() == "Player") {
		
	}


}

void Trampoline::OnCollisionStay(crint id)
{
}

void Trampoline::OnCollisionExit(crint id)
{
}
