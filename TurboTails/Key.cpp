
#include"main.h"

#include "Key.h"

#include"Scene.h"
#include"ModelManager.h"
#include"CircleCollider.h"
#include"DrawModel.h"
#include"UIManager.h"
#include"Player.h"
#include"ParticleManager.h"

void Key::Init()
{

	mModelID = ModelManager::LoadModel("asset\\model\\GoldKey.obj", "Key");

	mLayer.push_back("3D");
	mLayer.push_back("bloom");

	AddComponent(new CircleCollider(this, Vector3::zero(), 1.0f));

	mPlayer = nullptr;
	mAngle = 0.0f;
	mGet = false;

	mTag = "key";

}

void Key::Start()
{
	mDraw = new DrawModel(
		this,
		mModelID,
		"shader\\VertexLightingVS.cso",
		"shader\\VertexLightingPS.cso",
		list<int>(),
		ccolor(0.0f, 0.0f, 0.0f, 1.0f)
	);
	mDraw->Set(false);
}

void Key::Update()
{

	GameObject::Update();

	if (!mDraw->GetBool()) {

		if (dynamic_cast<UIManager*>(mScene->GetManagerTag("UIManager"))->GetUI("Score")->GetInt() >= 100) {
			mDraw->Set(true);
		}

	}

	if (mGet) {

		mAngle += 0.1f;

		ver3 pos = Vector3::zero();
		pos.x = cosf(mAngle);
		pos.z = sinf(mAngle);

		mTransform.pos = mPlayer->GetPosition() + pos;
		mTransform.scale -= Vector3::one() * 0.01f;

		if (mTransform.scale.x <= 0.0f) {
			mDestroy = true;
			dynamic_cast<Player*>(mPlayer)->SetKey();
		}

	}
	
}

void Key::OnCollisionEnter(crint id)
{

	if (!mDraw->GetBool())return;

	auto obj = mScene->GetGameObject(id);
	cstring tag = obj->GetTag();

	if (tag == "Player") {

		if (mDraw->GetBool()) {
			mPlayer = obj;
			mGet = true;
		}

	}

}

void Key::OnCollisionExit(crint id)
{
}

void Key::OnCollisionStay(crint id)
{
}

void Key::SetGameObject(GameObject* set)
{
	if (mDraw->GetBool()) {
		mPlayer = set;
		mGet = true;
	}
}
