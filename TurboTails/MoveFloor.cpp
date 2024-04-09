
#include"Main.h"

#include "MoveFloor.h"

#include"Scene.h"
#include"ModelManager.h"
#include"DrawModel.h"
#include"Time.h"
#include"CircleCollider.h"
#include"MeshField.h"

void MoveFloor::Front()
{

	mTransform.pos += mMove;

	if (mTime->GetTimeSecond()) {
		mFloorState = MoveFloorState::stop;
		mOldFloorState = MoveFloorState::front;
		mTime->Set(mStopTime);
	}

}

void MoveFloor::Back()
{

	mTransform.pos -= mMove;

	if (mTime->GetTimeSecond()) {
		mFloorState = MoveFloorState::stop;
		mOldFloorState = MoveFloorState::back;
		mTime->Set(mStopTime);
	}

}

void MoveFloor::Stop()
{

	if (mTime->GetTimeSecond()) {

		if (mOldFloorState == MoveFloorState::front) {

			mFloorState = MoveFloorState::back;

		}
		else {

			mFloorState = MoveFloorState::front;

		}

		mTime->Set(mMoveSpeed);

	}

}

MoveFloor::MoveFloor(crdouble movetime, crdouble stop, crver3 move)
{

	mMoveSpeed = movetime;
	mStopTime = stop;
	mMove = move;

}

void MoveFloor::Init()
{

	list<int> tex{};

	mModelID = ModelManager::LoadModel("asset\\model\\Jewel.obj", "MoveFloor");

	mTime = new Time();
	AddComponent(mTime);

	AddComponent(new CircleCollider(this, Vector3::zero(), 1.0f));

	mLayer.push_back("3D");

	mTransform.pos.y = mScene->GetGameObject<MeshField>("MeshField")->GetHeight(mTransform.pos);

}

void MoveFloor::Start()
{

	mDraw = new DrawModel(
		this,
		mModelID,
		"shader\\ToonVS.cso",
		"shader\\ToonPS.cso",
		list<int>(),
		Color::normal()
	);

}

void MoveFloor::Update()
{

	if (mStop)return;

	GameObject::Update();

	switch (mFloorState) {

	case MoveFloorState::none:
		mFloorState = MoveFloorState::front;
		mTime->Set(mMoveSpeed);
		break;

	case MoveFloorState::front:
		Front();
		break;

	case MoveFloorState::back:
		Back();
		break;

	case MoveFloorState::stop:
		Stop();
		break;

	}

}

void MoveFloor::OnCollisionEnter(crint id)
{



}

void MoveFloor::OnCollisionExit(crint id)
{



}

void MoveFloor::OnCollisionStay(crint id)
{

	auto obj = mScene->GetGameObject(id);

	if (obj->GetTag() == "Player") {

		if (mFloorState != MoveFloorState::stop) {

			if (mFloorState == MoveFloorState::front) {
				obj->SetAddPosition(mMove);
			}
			else if (mFloorState == MoveFloorState::back) {
				obj->SetAddPosition(mMove * (-1.0f));
			}

		}

	}

}
