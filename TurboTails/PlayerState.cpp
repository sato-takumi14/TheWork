
#include"Main.h"

#include "PlayerState.h"

#include"Input.h"
#include"MeshField.h"
#include"Time.h"
#include"TargetManager.h"
#include"UIManager.h"
#include"Player.h"
#include"ActionKey.h"
#include"ParticleManager.h"
#include"DashGauge.h"
#include"SceneManager.h"
#include"Scene.h"
#include"Camera.h"
#include"Particle.h"

static const function<State* (GameObject*, MeshField*, const PlayerStateMove&)> gNextMoveState =
[](GameObject* target,MeshField* field,const PlayerStateMove& state)->State* {

	switch ((PlayerStateMove)(state)) {

	case PlayerStateMove::ground:
		return new PlayerState::Move::Ground(target, field);
		break;

	case PlayerStateMove::jump:
		return new PlayerState::Move::Jump(target, field);
		break;

	case PlayerStateMove::fall:
		return new PlayerState::Move::Fall(target, field);
		break;

	case PlayerStateMove::ride:
		return new PlayerState::Move::RideScaffold(target, field);
		break;

	case PlayerStateMove::trampoline:
		return new PlayerState::Move::TrampolineState(target, field);
		break;

	case PlayerStateMove::elevator:
		return new PlayerState::Move::ElevatorState(target, field);
		break;

	case PlayerStateMove::flydash:
		return new PlayerState::Move::FlyDash(target, field);
		break;

	case PlayerStateMove::rundash:
		return new PlayerState::Move::RunDash(target, field);
		break;

	case PlayerStateMove::balloon:
		return new PlayerState::Move::BalloonState(target, field);
		break;

	}

	return nullptr;

	};

/************************************************************************************
* 移動系のステート
*************************************************************************************/

PlayerState::Move::Ground::Ground(GameObject* target, MeshField* field)
{
	mTarget = target;
	mField = field;
	mTarget->Set();
}

void PlayerState::Move::Ground::Update()
{

	if (mTarget->GetStateManager()->GetState("attack")->GetName() == "Assault")return;
	ver3 pos = mTarget->GetPosition();
	pos.y = mField->GetHeight(pos);
	mTarget->SetPosition(pos);

	auto particle = mTarget->GetScene()->GetParticleManager()->GetParticle("dust");
	if (dynamic_cast<Player*>(mTarget)->GetMove())particle->Set(true);
	else particle->Set(false);

	if (ActionKey::Action("jump")) {
		mChange = true;
		mNext = (int)(PlayerStateMove::jump);
	}

	if (ActionKey::Action("dash") && 
		dynamic_cast<Player*>(mTarget)->GetMove()) SetChange(static_cast<int>(PlayerStateMove::rundash));

}

State* PlayerState::Move::Ground::NextState() const
{
	mTarget->GetScene()->GetParticleManager()->GetParticle("dust")->Set(false);
	return gNextMoveState(mTarget, mField, (PlayerStateMove)mNext);
}

/************************************************************************************/

PlayerState::Move::Jump::Jump(GameObject* target, MeshField* field)
{
	mTarget = target;
	mField = field;
	mVel = Vector3::up() * 0.3f;
	mTarget->SetAnim("Jump");
	mTarget->Set(true);
	mName = "Jump";
	mTarget->Set(0);
}

void PlayerState::Move::Jump::Update()
{

	ver3 pos = mTarget->GetPosition();

	mVel.y -= 0.015f;
	pos += mVel;
	mTarget->SetPosition(pos);

	if (mDash) {
		dynamic_cast<DashGauge*>(dynamic_cast<UIManager*>(mTarget->GetScene()->GetManagerTag("UIManager"))->GetUI("DashGauge"))->DownGauge(Time::GetDeltaTime());
	}

	float height = mField->GetHeight(pos);

	if (ActionKey::Action("dash")) SetChange(static_cast<int>(PlayerStateMove::flydash));

	if (height > pos.y) {
		mChange = true;
		pos.y = height;
		mTarget->SetPosition(pos);
		if (mTarget->GetFloat() == 0.1f)mNext = (int)(PlayerStateMove::ground);
		else mNext = static_cast<int>(PlayerStateMove::rundash);
	}

}

State* PlayerState::Move::Jump::NextState() const
{
	mTarget->Set(false);
	return gNextMoveState(mTarget, mField, (PlayerStateMove)mNext);
}

/************************************************************************************/

PlayerState::Move::Fall::Fall(GameObject* target, MeshField* field)
{
	mTarget = target;
	mField = field;
	mVel = Vector3::zero();
	mTarget->Set(0);
}

void PlayerState::Move::Fall::Update()
{

	ver3 pos = mTarget->GetPosition();
	pos += mVel;
	mVel.y -= 0.01f;
	mTarget->SetPosition(pos);

	float height = mField->GetHeight(pos);

	if (pos.y < height) {
		pos.y = height;
		mTarget->SetPosition(pos);
		mChange = true;
		mNext = (int)(PlayerStateMove::ground);
	}

}

State* PlayerState::Move::Fall::NextState() const
{
	return gNextMoveState(mTarget, mField, (PlayerStateMove)mNext);
}

/************************************************************************************/

PlayerState::Move::RideScaffold::RideScaffold(GameObject* target, MeshField* field)
{
	mTarget = target;
	mField = field;
	mTarget->GetScene()->GetParticleManager()->GetParticle("dust")->Set(false);
}

void PlayerState::Move::RideScaffold::Update()
{

	auto manager = mTarget->GetScene()->GetParticleManager();
	if (dynamic_cast<Player*>(mTarget)->GetMove())manager->GetParticle("dust")->Set(true);
	else manager->GetParticle("dust")->Set(false);

	if (ActionKey::Action("jump")) {
		mChange = true;
		mNext = (int)(PlayerStateMove::jump);
	}

}

State* PlayerState::Move::RideScaffold::NextState() const
{
	mTarget->GetScene()->GetParticleManager()->GetParticle("dust")->Set(false);
	return gNextMoveState(mTarget, mField, (PlayerStateMove)mNext);
}

/************************************************************************************/

PlayerState::Move::TrampolineState::TrampolineState(GameObject* target, MeshField* field)
{
	mTarget = target;
	mField = field;
	mVel = mTarget->GetVer3() * Time::GetDeltaTime();
	mTarget->SetAnim("Fly");
	mSpeed = 5.0f;
}

void PlayerState::Move::TrampolineState::Update()
{

	State::Update();

	if (mStopCount++ >= 3) {
		ver3 pos = mTarget->GetPosition();
		cfloat height = mField->GetHeight(pos);

		if (mTimeCount++ >= 15) {
			mNext = (int)(PlayerStateMove::fall);
			mChange = true;
			mTarget->SetAnim("Jump");
		}

		if (height > pos.y) {
			mNext = (int)(PlayerStateMove::ground);
			mChange = true;
			pos.y = height;
		}


		mVel = mTarget->GetVer3() * Time::GetDeltaTime() * mSpeed;
		pos += mVel;
		mTarget->SetPosition(pos);

		if (ActionKey::Action("jump")) {
			mNext = (int)(PlayerStateMove::jump);
			mChange = true;
		}
	}

}

State* PlayerState::Move::TrampolineState::NextState() const
{
	mTarget->Set();
	ver3 rot = mTarget->GetRotation();
	rot.x = 0.0f;
	rot.z = 0.0f;
	mTarget->SetRotation(rot);
	return gNextMoveState(mTarget, mField, (PlayerStateMove)mNext);
}

/************************************************************************************/

PlayerState::Move::ElevatorState::ElevatorState(GameObject* target, MeshField* field)
{

	mTarget = target;
	mField = field;

	auto info = dynamic_cast<Player*>(mTarget)->GetElevatorInfo();
	mElevator = info->elevator;
	mVel = info->vel;

}

void PlayerState::Move::ElevatorState::Update()
{

	if (!mElevator->GetBool()) {
		mTarget->SetAddPosition(mVel);
	}
	else {
 		mChange = true;
		mNext = static_cast<int>(PlayerStateMove::jump);
		dynamic_cast<Player*>(mTarget)->SetStop(false);
	}

}

State* PlayerState::Move::ElevatorState::NextState() const
{
	return gNextMoveState(mTarget, mField, (PlayerStateMove)mNext);
}

/************************************************************************************/

PlayerState::Move::FlyDash::FlyDash(GameObject* target, MeshField* field)
{
	mTarget = target;
	mField = field;
	Player* player = dynamic_cast<Player*>(mTarget);
	player->Set(true);
	player->SetStop(true);

	ver3 addforce = mTarget->GetAddForce();
	if (addforce == Vector3::zero()) mAddForce = mTarget->GetReferenceTransform().GetForward();
	else mAddForce = addforce;

	mTarget->SetAnim("Fly");

}

PlayerState::Move::FlyDash::~FlyDash()
{
	
}

void PlayerState::Move::FlyDash::Update()
{

	if (mTime++ <= 8) { mTarget->SetAddPosition(mAddForce); }
	else SetChange(static_cast<int>(PlayerStateMove::fall));

}

State* PlayerState::Move::FlyDash::NextState() const
{
	Player* player = dynamic_cast<Player*>(mTarget);
	player->Set(false);
	player->SetStop(false);
	player->AddForce(Vector3::zero());
	return gNextMoveState(mTarget, mField, (PlayerStateMove)mNext);
}

/************************************************************************************/

PlayerState::Move::BalloonState::BalloonState(GameObject* target, MeshField* field)
{
	mTarget = target;
	mField = field;
}

void PlayerState::Move::BalloonState::Update()
{

	SetChange(static_cast<int>(PlayerStateMove::jump));

}

State* PlayerState::Move::BalloonState::NextState() const
{
	return gNextMoveState(mTarget, mField, (PlayerStateMove)mNext);
}

/************************************************************************************/

PlayerState::Move::RunDash::RunDash(GameObject* target, MeshField* field)
{
	mTarget = target;
	mField = field;
	mSpeed = mTarget->GetFloat();
	AddComponent(new Math::Lerp::Float(&mSpeed, mSpeed, mSpeed + 0.2f, 0.1f, 3.0f));

	auto gauge = dynamic_cast<DashGauge*>(dynamic_cast<UIManager*>(mTarget->GetScene()->GetManagerTag("UIManager"))->GetUI("DashGauge"));
	gauge->SetActivate(true);
	gauge->Set(false);
	mTarget->GetScene()->GetParticleManager()->GetParticle("dust")->Set(true);
	mName = "Dash";

}

void PlayerState::Move::RunDash::Update()
{

	State::Update();
	ver3 pos = mTarget->GetPosition();
	if (not mRide)pos.y = mField->GetHeight(pos);
	mTarget->SetPosition(pos);
	mTarget->Set(mSpeed);	

	DashGauge* gauge = dynamic_cast<DashGauge*>(dynamic_cast<UIManager*>(mTarget->GetScene()->GetManagerTag("UIManager"))->GetUI("DashGauge"));
	gauge->DownGauge(Time::GetDeltaTime());

	if (ActionKey::Action("jump")) {
		mChange = true;
		mNext = (int)(PlayerStateMove::jump);
	}

	if (!dynamic_cast<Player*>(mTarget)->GetMove() ||
		dynamic_cast<DashGauge*>(dynamic_cast<UIManager*>(mTarget->GetScene()->GetManagerTag("UIManager"))->GetUI("DashGauge"))->GetFloat() <= 0.0f) {
		SetChange(static_cast<int>(PlayerStateMove::ground));
	}
}

State* PlayerState::Move::RunDash::NextState() const
{
	mTarget->GetScene()->GetParticleManager()->GetParticle("dust")->Set(false);
	State* next = gNextMoveState(mTarget, mField, (PlayerStateMove)mNext);
	if ((PlayerStateMove)(mNext) != PlayerStateMove::jump) {
		dynamic_cast<DashGauge*>(dynamic_cast<UIManager*>(mTarget->GetScene()->GetManagerTag("UIManager"))->GetUI("DashGauge"))->Set(true);
		mTarget->AddComponent(new Math::Lerp::Float(mTarget->GetPFloat(), mTarget->GetFloat(), 0.1f, 0.1f, 3.0f));
	}
	else {
		dynamic_cast<PlayerState::Move::Jump*>(next)->Set(true);
	}
	return next;
}

/************************************************************************************/

/***********************************************************************************
* 攻撃系ステート
************************************************************************************/

void PlayerState::Attack::Assault::SetRotation(crver3 vel)
{

	ver3 rot = mTarget->GetRotation();

	ver3 vec{};
	D3DXVec3Normalize(&vec, &vel);
	rot.y = atan2f(vec.x, vec.z);
	mTarget->SetRotation(rot);

}

PlayerState::Attack::Assault::Assault(GameObject* target, const PARAMETER& param)
{

	mTarget = target;
	mParameter = param;

	mAssaultTarget = dynamic_cast<TargetManager*>(mTarget->GetScene()->GetManagerTag("TargetManager"))->GetTarget(mTarget->GetPosition());

	if (mAssaultTarget) {
		mTarget->SetAnim("Fly");
		mOffset = mAssaultTarget->GetOffset();
		dynamic_cast<UIManager*>(mTarget->GetScene()->GetManagerTag("UIManager"))->GetUI("line")->SetActivate(true);
		mVel = (mAssaultTarget->GetPosition() + mOffset - mTarget->GetPosition());
		mVel *= (mParameter.Color.b * Time::GetDeltaTime());
		SetRotation(mVel);
	}
	else mChange = true;

	mName = "Assault";

}

PlayerState::Attack::Assault::~Assault()
{
	
}

void PlayerState::Attack::Assault::Update()
{

	if (mChange)return;

	mStop = true;

	if (mAssaultTarget) {

		if (mStop) {

			mVel = (mAssaultTarget->GetPosition() + mOffset - mTarget->GetPosition());
			mVel *= (mParameter.Color.b * Time::GetDeltaTime());
			SetRotation(mVel);

			mTarget->SetAddPosition(mVel);

		}

	}
	else mChange = true;

}

State* PlayerState::Attack::Assault::NextState() const
{
	dynamic_cast<UIManager*>(mTarget->GetScene()->GetManagerTag("UIManager"))->GetUI("line")->SetActivate(false);
	return new Deceleration(mTarget, mParameter);
}

void PlayerState::Attack::Assault::SetParameter()
{
	Renderer::SetParameter(mParameter);
}

/************************************************************************************/

PlayerState::Attack::Charge::Charge(GameObject* target, const PARAMETER& param)
{
	mTarget = target;
	mParam = param;
	mTarget->PlayAudio("charge");
	auto particle = mTarget->GetScene()->GetParticleManager()->GetParticle("charge");
	particle->Set(true);
}

void PlayerState::Attack::Charge::Update()
{

	if (ActionKey::Action("charge")) {
		mParam.Color.b += 0.05f;
		if (mParam.Color.b > 1.0f)mParam.Color.b = 1.0f;
	}
	else mChange = true;

}

State* PlayerState::Attack::Charge::NextState() const
{
	auto particle = mTarget->GetScene()->GetParticleManager()->GetParticle("charge");
	particle->Set(false);
	mTarget->StopAudio("charge");
	return new Assault(mTarget, mParam);
}

void PlayerState::Attack::Charge::SetParameter()
{
	Renderer::SetParameter(mParam);
}

/************************************************************************************/

PlayerState::Attack::Deceleration::Deceleration(GameObject* target, const PARAMETER& param)
{
	mTarget = target;
	mParam = param;
}

void PlayerState::Attack::Deceleration::Update()
{

	if (ActionKey::Action("attack")) {

		mChange = true;
		mReCharge = true;

	}

	mParam.Color.b -= 0.1f;

	if (mParam.Color.b <= .0f) {
		mParam.Color.b = 0.0f;
		mChange = true;
	}

}

State* PlayerState::Attack::Deceleration::NextState() const
{
	if (mReCharge)return new Charge(mTarget, mParam);
	return new PlayerAttackNormal(mTarget);
}

void PlayerState::Attack::Deceleration::SetParameter()
{
	Renderer::SetParameter(mParam);
}

/************************************************************************************/

PlayerState::Attack::PlayerAttackNormal::PlayerAttackNormal(GameObject* target)
{
	mTarget = target;
}

void PlayerState::Attack::PlayerAttackNormal::Update()
{
	if (ActionKey::Action("attack") &&
		not dynamic_cast<Player*>(mTarget)->GetGoal()) mChange = true;
}

State* PlayerState::Attack::PlayerAttackNormal::NextState() const
{
	return new Charge(mTarget, PARAMETER{ Vector4::zero(),Color::black(),Color::black(), Color::black() });
}

void PlayerState::Attack::PlayerAttackNormal::SetParameter()
{
	Renderer::SetParameter(PARAMETER{ Vector4::zero(),Color::normal(),Color::normal(),Color::transparent() });
}

/************************************************************************************/
