
#include"main.h"

#include<math.h>

#include "Jewel.h"

#include"Scene.h"
#include"DrawModel.h"
#include"ModelManager.h"
#include"UIManager.h"
#include"MeshField.h"
#include"CircleCollider.h"
#include"Random.h"

void Jewel::Init()
{

	mModelID = ModelManager::LoadModel("asset\\model\\Jewel_001.obj", "Jewel");

	mLayer.push_back("3D");

	AddComponent(new CircleCollider(this, Vector3::zero(), 1.0f));

	mTag = "Jewel";
}

void Jewel::Start()
{

	mTransform.pos = Random::Vector3(-15.0f, 15.0f);
	mTransform.pos.y = mScene->GetGameObject<MeshField>("MeshField")->GetHeight(mTransform.pos);
	mDraw = new DrawModel(
		this,
		mModelID,
		"shader\\MaterialColorVS.cso",
		"shader\\MaterialColorPS.cso",
		list<int>(),
		Color::normal()
	);

}

void Jewel::Update()
{

	if (mPlayer) {

		mAngle += 0.1f;

		ver3 pos = Vector3::zero();
		pos.x = cosf(mAngle);
		pos.z = sinf(mAngle);

		mTransform.pos = mPlayer->GetPosition() + pos;
		mTransform.scale -= Vector3::one() * 0.01f;

		if (mTransform.scale.x <= 0.0f) {
			mDestroy = true;
		}

	}

}

void Jewel::OnCollisionEnter(crint id)
{

	auto obj = mScene->GetGameObject(id);

	if (obj->GetTag() == "Player") {

		mPlayer = obj;

		ver3 vel = mPlayer->GetPosition() - mTransform.pos;
		D3DXVec3Normalize(&vel, &vel);
		mAngle = atan2f(vel.x, vel.z);

		mScene->GetGameObjectTag("Score")->Set(1);

	}

}

void Jewel::OnCollisionExit(crint id)
{
}

void Jewel::OnCollisionStay(crint id)
{
}
