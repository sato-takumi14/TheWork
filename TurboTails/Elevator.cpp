
#include"Main.h"

#include "Elevator.h"

#include"Scene.h"
#include"ModelManager.h"
#include"DrawModel.h"
#include"CircleCollider.h"
#include"MeshField.h"
#include"TargetManager.h"

void Elevator::Init()
{

	mModelID = ModelManager::LoadModel("asset\\model\\box.obj", "Elevator");
	mTransform.pos = Vector3::up() * 15.0f;
	mTransform.pos.x = 5.0f;

	//捕まる部分の初期化
	{
		Bottom* bottom = new Bottom;
		bottom->SetTransform(mTransform);
		bottom->Set(mTransform.pos);
		bottom->Set(50.0f);
		ver3 pos = bottom->GetPosition();
		pos.y = mScene->GetGameObjectTag("MeshField")->GetHeight(pos);
		bottom->SetPosition(pos);
		mScene->AddGameObject(bottom);
	}

	mLayer.push_back("3D");

}

void Elevator::Start()
{
	mDraw = new DrawModel(
		this,
		mModelID,
		"shader\\VertexLightingVS.cso",
		"shader\\VertexLightingPS.cso",
		list<int>(),
		Color::normal()
	);
}

void Elevator::Update()
{
	GameObject::Update();
}

void Elevator::Draw()
{
	GameObject::Draw();
}

void Elevator::Uninit()
{
	GameObject::Uninit();
}

/*****************************************************************************
* エレベーター下部分
******************************************************************************/
void Bottom::Init()
{

	AddComponent(new CircleCollider(this, Vector3::zero(), 1.0f));

	mOriginPos = mTransform.pos;
	mVel = (mTopPos - mTransform.pos) / mTime;

	mModelID = ModelManager::LoadModel("asset\\model\\box.obj", "Elevator");

	mLayer.push_back("3D");

	mTag = "ElevatorBottom";
	mClass = "Bottom";

}

void Bottom::Start()
{
	mDraw = new DrawModel(
		this,
		mModelID,
		"shader\\vertexLightingVS.cso",
		"shader\\vertexLightingPS.cso",
		list<int>(),
		Color::normal()
	);
}

void Bottom::Update()
{

	GameObject::Update();

	if (mPlayer) {

		mTransform.pos += mVel;

		if (Vector3::round(mTransform.pos) == Vector3::round(mTopPos)) {

			mPlayer = nullptr;
			mReturn = true;

		}

	}

	if (mReturn) {

		mTransform.pos -= mVel;

		if (Vector3::round(mTransform.pos) == Vector3::round(mOriginPos)) {
			mReturn = false;
		}

	}

}

void Bottom::OnCollisionEnter(crint id)
{

	if (mReturn)return;

	auto obj = mScene->GetGameObject(id);

	if (obj->GetTag() == "Player") {

		mPlayer = obj;

	}

}

void Bottom::OnCollisionStay(crint id)
{
}

void Bottom::OnCollisionExit(crint id)
{
}

/******************************************************************************/
