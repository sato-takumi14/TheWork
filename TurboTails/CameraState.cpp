
#include"Main.h"

#include "CameraState.h"
#include"Input.h"
#include"Debug.h"
#include"Time.h"
#include"ActionKey.h"
#include"Player.h"
#include"UIManager.h"
#include"DashGauge.h"
#include"Scene.h"
#include"MeshField.h"

function<State* (GameObject*, GameObject*, crver3, const CameraStatus&, crfloat)> gNextState =
[](GameObject* target, GameObject* player, crver3 rot, const CameraStatus& status, crfloat length)->State* {

	switch (status) {

	case CameraStatus::normal:
		Debug::Log("CameraState : Normal");
		return new CameraState::CameraNormal(target, player, rot, length);
		break;

	case CameraStatus::dash:
		Debug::Log("CameraState : Dash");
		return new CameraState::CameraDash(target, player, rot, length);
		break;

	case CameraStatus::hit:
		Debug::Log("CameraState : Hit");
		return new CameraState::HitCamera(target, player, rot, length);
		break;

	case CameraStatus::move:
		Debug::Log("CameraState : Move");
		return new CameraState::CameraTracking(target, player, rot, length);
		break;

	}

	return nullptr;

	};

/**********************************************************************************************
* カメラステートのインターフェース
***********************************************************************************************/
void CameraState::CameraStateInterface::Move(crver3 data, crfloat size)
{

	ver3 pos;
	ver3 playerPos = mPlayer->GetPosition();

	mAddRot.x += data.y * size;
	mAddRot.y += data.x * size;

	mTargetPos = playerPos + Vector3::up() * 2.0f;
	pos = mTargetPos - (Vector3::GetForward(mAddRot) * mLength) + Vector3::up();

	if (playerPos.y + 0.5f > pos.y) {

		pos.y = playerPos.y + 0.5f;

		mAddRot.x -= data.y * size;
		pos = mTargetPos - (Vector3::GetForward(mAddRot) * mLength) + Vector3::up();

	}

	ver3 vec = pos - playerPos;
	if (D3DXVec3Length(&vec) > 7.8f) {

		mAddRot.x -= data.y * size;
		pos = mTargetPos - (Vector3::GetForward(mAddRot) * mLength) + Vector3::up();

	}

	mOldPos = pos;
	mTarget->SetPosition(pos);

}

void CameraState::CameraStateInterface::EasingMove(crver3 data, crfloat size)
{

	ver3 pos;
	ver3 target;
	ver3 playerPos = mPlayer->GetPosition();

	mAddRot.x += data.y * size;
	mAddRot.y += data.x * size;

	mTargetPos = playerPos + Vector3::up() * 2.0f;
	target = mTargetPos - (Vector3::GetForward(mAddRot) * mLength) + Vector3::up();

	pos = mOldPos + ((target - mOldPos) * 0.1f);

	if (playerPos.y + 0.5f > target.y) {

		target.y = playerPos.y + 0.5f;

		mAddRot.x -= data.y * size;
		target = mTargetPos - (Vector3::GetForward(mAddRot) * mLength) + Vector3::up();

	}

	ver3 vec = target - playerPos;
	float len = D3DXVec3Length(&vec);
	if (len > (7.8f + (mLength - 5.0f))) {

		mAddRot.x -= data.y * size;
		target = mTargetPos - (Vector3::GetForward(mAddRot) * mLength) + Vector3::up();

	}

	mOldPos = pos;
	mTarget->SetPosition(pos);

}


void CameraState::CameraStateInterface::Shake()
{
	mOffset = sinf(static_cast<float>(mTime) * 1.5f) * mShakeAmplitude;
	mTime++;
	mShakeAmplitude *= 0.8f;
}

void CameraState::CameraStateInterface::Update()
{
	State::Update();
	if (mShake) { CameraStateInterface::Shake(); }
}

void CameraState::CameraStateInterface::Set(crfloat set)
{
	mShake = true;
	mShakeAmplitude = set;
}

/**********************************************************************************************
* 何もしていないときのカメラ
***********************************************************************************************/
CameraState::CameraNormal::CameraNormal(GameObject* target, GameObject* player, crver3 rot)
{
	mTarget = target;
	mPlayer = player;
	mAddRot = rot;
	mTargetPos = player->GetPosition() + Vector3::up() * 2.0f;
	CameraStateInterface::SetOldPostion(mTargetPos - (Vector3::GetForward(rot) * 5.0f) + Vector3::up());
	mLength = 5.0f;
}

CameraState::CameraNormal::CameraNormal(GameObject* target, GameObject* player, crver3 rot, crfloat length)
{
	mTarget = target;
	mPlayer = player;
	mAddRot = rot;
	mTargetPos = player->GetPosition() + Vector3::up() * 2.0f;
	CameraStateInterface::SetOldPostion(mTargetPos - (Vector3::GetForward(rot) * length) + Vector3::up());
	mLength = length;
	mField = dynamic_cast<MeshField*>(mTarget->GetScene()->GetGameObjectTag("MeshField"));
}

void CameraState::CameraNormal::Update()
{

	CameraStateInterface::Update();

	auto gamepad = Input::GetGamePad();

	if (gamepad->GetConnect()) CameraStateInterface::EasingMove(Vector3::Create(gamepad->GetRight()), 0.05f);
	else CameraStateInterface::EasingMove(Input::GetMouseVector3(), MouseCameraSpeed);

	if (ActionKey::Action("dash") &&
		dynamic_cast<Player*>(mPlayer)->GetMove()) {
		SetChange(static_cast<int>(CameraStatus::dash));
	}

	ver3 pos = mTarget->GetPosition();
	float height = mField->GetHeight(pos);
	if (pos.y <= height + 0.3f) {
		pos.y = height + 0.3f;
		mTarget->SetPosition(pos);
	}

}

State* CameraState::CameraNormal::NextState() const
{
	return gNextState(mTarget, mPlayer, mAddRot, (CameraStatus)(mNext), mLength);
}

/***************************************************************************************************************/

/**********************************************************************************************
* ダッシュしているときのカメラ
***********************************************************************************************/
CameraState::CameraDash::CameraDash(GameObject* target, GameObject* player, crver3 rot)
{

	mTarget = target;
	mPlayer = player;
	mAddRot = rot;

	mTargetPos = mPlayer->GetPosition() + Vector3::up() * 2.0f;
	CameraStateInterface::SetOldPostion(mTargetPos - (Vector3::GetForward(rot) * 5.0f) + Vector3::up());
	mLength = 5.0f;
	AddComponent(new Math::Lerp::Float(&mLength, mLength, 10.0f, 0.50000000f, 5.0f));

}

CameraState::CameraDash::CameraDash(GameObject* target, GameObject* player, crver3 rot, crfloat length)
{
	mTarget = target;
	mPlayer = player;
	mAddRot = rot;

	mTargetPos = mPlayer->GetPosition() + Vector3::up() * 2.0f;
	CameraStateInterface::SetOldPostion(mTargetPos - (Vector3::GetForward(rot) * 5.0f) + Vector3::up());
	mLength = length;
	AddComponent(new Math::Lerp::Float(&mLength, mLength, mLength + 5.0f, 0.50000000f, 5.0f));

	mField = dynamic_cast<MeshField*>(mTarget->GetScene()->GetGameObjectTag("MeshField"));

}

void CameraState::CameraDash::Update()
{

	CameraStateInterface::Update();

	auto gamepad = Input::GetGamePad();

	if (!mStop) {
		if (!dynamic_cast<Player*>(mPlayer)->GetMove()) {
			mStop = true;
			mLength = 10.0f;
			AddComponent(new Math::Lerp::Float(&mLength, mLength, mLength - 5.0f, 0.50000000f, 5.0f));
		}
	}
	else {
		if (mLength <= 5.00001000f) {
			SetChange(static_cast<int>(CameraStatus::normal));
			mStop = false;
		}
	}

	if (gamepad->GetConnect()) CameraStateInterface::EasingMove(Vector3::Create(gamepad->GetRight()), 0.05f);
	else CameraStateInterface::EasingMove(Input::GetMouseVector3(), MouseCameraSpeed);

	ver3 pos = mTarget->GetPosition();
	float height = mField->GetHeight(pos);
	if (pos.y <= height + 1.0f) {
		pos.y = height + 1.0f;
		mTarget->SetPosition(pos);
	}

}

State* CameraState::CameraDash::NextState() const
{
	return gNextState(mTarget, mPlayer, mAddRot, (CameraStatus)(mNext), mLength);
}

/***************************************************************************************************************/

/**********************************************************************************************
*何かに当たった時のカメラ
***********************************************************************************************/
CameraState::HitCamera::HitCamera(GameObject* target, GameObject* player, crver3 rot)
{

	mTarget = target;
	mPlayer = player;
	mAddRot = rot;
	mLength = 5.0f;
	mTargetPos = player->GetPosition() + Vector3::up() * 2.0f;
	CameraStateInterface::SetOldPostion(mTargetPos - (Vector3::GetForward(rot) * 5.0f) + Vector3::up());

	AddComponent(new Math::Lerp::Float(&mLength, mLength, mLength + 5.0f, 0.5f, 5.0f));

}

CameraState::HitCamera::HitCamera(GameObject* target, GameObject* player, crver3 rot, crfloat length)
{
	mTarget = target;
	mPlayer = player;
	mAddRot = rot;
	mLength = length;
	mTargetPos = player->GetPosition() + Vector3::up() * 2.0f;
	CameraStateInterface::SetOldPostion(mTargetPos - (Vector3::GetForward(rot) * 5.0f) + Vector3::up());

	AddComponent(new Math::Lerp::Float(&mLength, mLength, mLength + 5.0f, 0.5f, 5.0f));
}

void CameraState::HitCamera::Update()
{

	CameraStateInterface::Update();

	auto gamepad = Input::GetGamePad();

	if (gamepad->GetConnect()) CameraStateInterface::Move(Vector3::Create(gamepad->GetRight()), 0.05f);
	else CameraStateInterface::Move(Input::GetMouseVector3(), 0.01f);

}

State* CameraState::HitCamera::NextState() const
{
	return gNextState(mTarget, mPlayer, mAddRot, (CameraStatus)(mNext), mLength);
}

/***************************************************************************************************************/

/**********************************************************************************************
* 移動しているときのカメラ
***********************************************************************************************/
CameraState::CameraTracking::CameraTracking(GameObject* target, GameObject* player, crver3 rot)
{
	mTarget = target;
	mPlayer = player;
	mAddRot = rot;
	mTargetPos = player->GetPosition() + Vector3::up() * 2.0f;
	CameraStateInterface::SetOldPostion(mTargetPos - (Vector3::GetForward(rot) * 5.0f) + Vector3::up());
}

CameraState::CameraTracking::CameraTracking(GameObject* target, GameObject* player, crver3 rot, crfloat length)
{
	mTarget = target;
	mPlayer = player;
	mAddRot = rot;
	mLength = length;
	mTargetPos = player->GetPosition() + Vector3::up() * 2.0f;
	CameraStateInterface::SetOldPostion(mTargetPos - (Vector3::GetForward(rot) * mLength) + Vector3::up());
}

void CameraState::CameraTracking::Update()
{

	CameraStateInterface::Update();

	auto gamepad = Input::GetGamePad();

	if (gamepad->GetConnect()) CameraStateInterface::EasingMove(Vector3::Create(gamepad->GetRight()), 0.05f);
	else CameraStateInterface::EasingMove(Input::GetMouseVector3(), MouseCameraSpeed);

	if (!dynamic_cast<Player*>(mPlayer)->GetMove())SetChange(static_cast<int>(CameraStatus::normal));

}

State* CameraState::CameraTracking::NextState() const
{
	return gNextState(mTarget, mPlayer, mAddRot, (CameraStatus)(mNext), mLength);
}

/***************************************************************************************************************/

/**********************************************************************************************
* 何もないのカメラ
***********************************************************************************************/

CameraState::NoneCamera::NoneCamera(GameObject* target, GameObject* player, crver3 rot, crfloat length)
{

	mTargetPos = Vector3::up() * 2.0f;
	ver3 pos = mTargetPos - (Vector3::GetForward(rot) * length) + Vector3::up();

	target->SetPosition(pos);
	mTarget = target;
	mPlayer = player;
	mLength = length;
	mAddRot = rot;

	mField = dynamic_cast<MeshField*>(mTarget->GetScene()->GetGameObjectTag("MeshField"));

}

void CameraState::NoneCamera::Update()
{

	ver3 playerPos = mPlayer->GetPosition();
	mTargetPos = playerPos + Vector3::up() * 2.0f;
	ver3 pos = mTargetPos - (Vector3::GetForward(mAddRot) * mLength) + Vector3::up();
	mTarget->SetPosition(pos);

	float height = mField->GetHeight(pos);
	if (pos.y <= height + 0.3f) {
		pos.y = height + 0.3f;
		mTarget->SetPosition(pos);
	}

}
