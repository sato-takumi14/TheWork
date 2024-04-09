
#include"main.h"
#include "Player.h"

#include"ModelManager.h"
#include"Input.h"
#include"Time.h"
#include"MeshField.h"
#include"Scene.h"
#include"SceneManager.h"
#include"DrawAnimModel.h"
#include"CircleCollider.h"
#include"Debug.h"
#include"Enemy.h"
#include"Particle.h"

#include"PlayerState.h"
#include"ActionKey.h"
#include"DashGauge.h"

#include"TargetManager.h"
#include"UIManager.h"
#include"ParticleManager.h"
#include"EnemyManager.h"

#include"audio\Tutorial.h"
#include"audio\Charge.h"
#include"audio\Run.h"
#include"audio\Trampoline.h"
#include"audio\Balloon.h"

void Player::Move()
{

	if (mKnockBack.flag || mTrampoline.flag)return;

	mMove = false;
	ver3 vel{};
	ver3 rot = SceneManager::GetScene()->GetMainCamera()->GetVer3();

	if (ActionKey::Action("forward") && ActionKey::Action("left")) {

		mMove = true;
		rot.y += (PI * 1.75f);

	}
	else if (ActionKey::Action("forward") && ActionKey::Action("right")) {

		mMove = true;
		rot.y += (PI * 0.25f);

	}
	else if (ActionKey::Action("back") && ActionKey::Action("left")) {

		mMove = true;
		rot.y += (PI * 1.25f);

	}
	else if (ActionKey::Action("back") && ActionKey::Action("right")) {

		mMove = true;
		rot.y += (PI * 0.75f);

	}
	else if (ActionKey::Action("forward")) {

		mMove = true;

	}
	else if (ActionKey::Action("back")) {

		mMove = true;
		rot.y += (PI);

	}
	else if (ActionKey::Action("left")) {

		mMove = true;
		rot.y += (PI * 1.5f);

	}
	else if (ActionKey::Action("right")) {

		mMove = true;
		rot.y += (PI * 0.5f);

	}

	auto gamepad = Input::GetGamePad();
	if (gamepad->GetConnect()) {

		ver2 vec = gamepad->GetLeft();
		if (vec != Vector2::zero()) {
			mMove = true;
			ver3 vel = mTransform.pos + ver3(vec.x, 0.0f, vec.y);
			vel = vel - mTransform.pos;
			D3DXVec3Normalize(&vel, &vel);
			rot.y += atan2f(vel.x, vel.z);

		}

	}

	if (mMove) {

		if (!mStateManager->GetState("attack")->GetBool() && !mStop) {
			mMoveVel = Vector3::GetForward(rot) * mSpeed;
			mMoveVel.y = 0.0f;
			mTransform.rot.y = rot.y;
			
			mAudioManager->BeginPlay("run");

		}

		if (!mJump) {
			
			if (!mStateManager->GetState("attack")->GetBool()) {
				mDraw->SetAnimName("Run");
			}
			
		}

	}
	else {

		if (!mJump) {
			if (mMoveVel == Vector3::zero()) {
				if (!mStateManager->GetState("attack")->GetBool()) {
					mDraw->SetAnimName("Idle");
					mAudioManager->Stop("run");
				}
			}
			else {
				mAudioManager->BeginPlay("run");
				mDraw->SetAnimName("RunStop");
				if (mDraw->GetFloat() == 1.0f) {
					mMoveVel = Vector3::zero();
				}
			}
		}
		
	}

	mTransform.pos += mMoveVel;

	ver3 pos = mTransform.pos;
	pos.y = 0.0f;
	if (D3DXVec3Length(&pos) >= 295.0f) {
		mTransform.UndoPosition();
	}

}

void Player::Deceleration()
{

	if (!mMove) {

		if (mMoveVel != Vector3::zero()) {

			ver3 vel = -mMoveVel * Time::GetDeltaTime() * 0.8f;
			mMoveVel += vel;

		}

	}


}

void Player::KnockBack()
{

	if (mKnockBack.flag) {

		if (!mKnockBack.time) { 
			mKnockBack.time = new Time();
			mKnockBack.time->Set(2.0);
		}

		mDraw->SetAnimName("Backflip");

		if (mKnockBack.time->GetTimeSecond()) {
			mKnockBack.flag = false;
			mDraw->SetAnimName("Idle");
			delete mKnockBack.time;
			mKnockBack.time = nullptr;
		}
		else {
			mTransform.pos -= mKnockBack.back * Time::GetDeltaTime();
			mTransform.pos.y = mMeshField->GetHeight(mTransform.pos);
		}

	}

}

void Player::SetChangeState(crbool flag)
{
	mStop = flag;
	mJump = flag;
}

void Player::Init()
{

	{
		/*モデル*/
		mModelID = ModelManager::LoadAnimModel(
			"asset\\animationmodel\\player\\TurboTails.fbx",
			"player");

		ModelManager::LoadAnimMotion(
			mModelID,
			"asset\\animationmodel\\player\\TurboTailsIdle.fbx",
			"Idle",
			0.2f);

		ModelManager::LoadAnimMotion(
			mModelID,
			"asset\\animationmodel\\player\\TurboTailsFast Run.fbx",
			"Run",
			0.05f);

		ModelManager::LoadAnimMotion(
			mModelID,
			"asset\\animationmodel\\player\\Walking.fbx",
			"Walk",
			0.01f);

		ModelManager::LoadAnimMotion(
			mModelID,
			"asset\\animationmodel\\player\\TurboTailsJump.fbx",
			"Jump",
			1.0f);

		ModelManager::LoadAnimMotion(
			mModelID,
			"asset\\animationmodel\\player\\TurboTailsFlying.fbx",
			"Fly",
			1.0f);

		ModelManager::LoadAnimMotion(
			mModelID,
			"asset\\animationmodel\\player\\Female Action Pose.fbx",
			"Sliding",
			0.01f);

		ModelManager::LoadAnimMotion(
			mModelID,
			"asset\\animationmodel\\player\\TurboTailsBackflip.fbx",
			"Backflip",
			0.01f);

		ModelManager::LoadAnimMotion(
			mModelID,
			"asset\\animationmodel\\player\\TurboTailsRun To Stop.fbx",
			"RunStop",
			0.1f);

	}

	{
		/*サウンド*/
		mAudioManager = new AudioManager(this);
		mAudioManager->Load(
			"charge",
			"Tutorial",
			"Charge",
			"",
			CRI_CHARGE_MAOU_SE_8BIT15,
			AudioState::se,
			true
		);
		mAudioManager->Volume("charge", 0.1f, CRI_CHARGE_MAOU_SE_8BIT15);

		mAudioManager->Load(
			"run",
			"Tutorial",
			"Run",
			"",
			CRI_RUN_土の上を走る,
			AudioState::se,
			true
		);
		mAudioManager->Volume("run", 0.5f, CRI_RUN_土の上を走る);

		mAudioManager->Load(
			"trampoline",
			"Tutorial",
			"Trampoline",
			"",
			CRI_TRAMPOLINE_ボヨン,
			AudioState::se,
			false
		);
		mAudioManager->Volume("trampoline", 0.5f, CRI_TRAMPOLINE_ボヨン);

		mAudioManager->Load(
			"balloon",
			"Tutorial",
			"Balloon",
			"",
			CRI_BALLOON_クローゼットを閉める,
			AudioState::se,
			false
		);
		mAudioManager->Volume("balloon", 5.0f, CRI_BALLOON_クローゼットを閉める);

	}

	/*各変数*/
	mTag = "Player";
	mClass = "Player";
	mLayer.push_back("3D");

	mTransform.scale *= 0.01f;
	mTransform.scale.y *= 0.8f;

	mKnockBack.time = nullptr;

	mMoveVel = Vector3::zero();

	mMeshField = mScene->GetGameObject<MeshField>("MeshField");

	{
		/*パーティクル*/
		Transform transform;
		transform.pos = mTransform.pos - mTransform.GetUp();
		transform.scale *= 0.03f;
		auto manager = mScene->GetParticleManager();
		manager->CreateParticle(
			"dust",
			transform,
			"particle",
			50,
			"asset\\model\\DefaultCube.obj",
			Particle::ParticleMode::rise,
			50,
			0.15f,
			cver3(1.0f, 0.0f, 1.0f) * 0.8f,
			ccolor(0.3f, 0.3f, 0.0f, 1.0f)
		);

		auto particle = manager->CreateParticle(
			"charge",
			transform,
			"particle",
			30,
			"asset\\model\\DefaultCube.obj",
			Particle::ParticleMode::collect,
			10,
			0.01f,
			Vector3::one(),
			ccolor(0.0f, 0.0f, 0.8f, 1.0f),
			true
		);
		particle->Set(false);
	}

	/*ステート*/
	mStateManager = new StateManager();
	mStateManager->AddState("move", new PlayerState::Move::Ground(this, mMeshField));
	mStateManager->AddState("attack", new PlayerState::Attack::PlayerAttackNormal(this));

	/*当たり判定*/
	AddComponent(new CircleCollider(this, Vector3::zero(), 1.0f));

	mTransform.pos = Vector3::back() * 10.0f;

	mStartPos = mTransform.pos;

	Debug::AddResetFunction([&]() { mTransform.pos = mStartPos; });

}

void Player::Start()
{

	
	dynamic_cast<DashGauge*>(dynamic_cast<UIManager*>(mScene->GetManagerTag("UIManager"))->GetUI("DashGauge"))->SetScreenPosition(mTransform.pos);

	/*描画*/
	mDraw = new DrawAnimModel(
		this,
		mModelID,
		"shader\\VertexLightingVS.cso",
		"shader\\LightPS.cso",
		list<int>(),
		Color::normal(),
		0.05f,
		"Idle"
	);

}

void Player::Update()
{
	
	GameObject::Update();

	Move();
	Deceleration();
	KnockBack();

	if (mStateManager->GetState("move")->GetNowState() != static_cast<uint>(PlayerStateMove::ride))mScaffold.clear();

	auto manager = mScene->GetParticleManager();
	manager->GetParticle("dust")->SetPosition(mTransform.pos - mTransform.GetForward());
	manager->GetParticle("charge")->SetPosition(mTransform.pos + mTransform.GetUp());

}

void Player::Draw()
{

	auto tex = Renderer::GetDepthShadowTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &tex);

	mStateManager->SetParameter();

	GameObject::Draw();

}

void Player::Uninit()
{

	GameObject::Uninit();
	
	if (mKnockBack.time) delete mKnockBack.time;

}

void Player::OnCollisionEnter(crint id)
{

	auto obj = mScene->GetGameObject(id);
	cstring tag = obj->GetTag();

	if (tag == "Enemy") {

		obj->SetDestroy();
		mKnockBack.back = obj->GetPosition() - mTransform.pos;
		D3DXVec3Normalize(&mKnockBack.back, &mKnockBack.back);
		mKnockBack.back *= 0.05f;
		mStateManager->GetState("attack")->SetChange();
		mKnockBack.flag = true;

	}
	else if (tag == "Ring") {
		State* state = mStateManager->GetState("attack");
		if (state->GetName() == "Assault")state->SetChange();
		
		state = mStateManager->GetState("move");
		state->SetChange(static_cast<int>(PlayerStateMove::flydash));
		mTransform.rot = obj->GetRotation();

	}
	else if (tag == "Scaffold") {

		State* state = mStateManager->GetState("move");

		if (state->GetNowState() == (uint)(PlayerStateMove::jump) ||
			state->GetNowState() == (uint)(PlayerStateMove::trampoline) ||
			state->GetNowState() == (uint)(PlayerStateMove::fall) ||
			state->GetNowState() == (uint)(PlayerStateMove::ride)) {

			ver3 pos = obj->GetPosition();
			ver3 scale = obj->GetScale();

			if (pos.y + scale.y * 2.0 <= mTransform.pos.y) {

				mTransform.pos.y = pos.y + scale.y * 2.2f;
				state->SetChange(static_cast<int>(PlayerStateMove::ride));

			}
			else if (pos.y >= mTransform.pos.y) {
				mTransform.UndoPosition();
			}

			bool flag = true;
			for (auto& data : mScaffold) {
				if (data == obj->GetObjectID()) {
					flag = false;
					break;
				}
			}

			if (flag) mScaffold.push_back(obj->GetObjectID());

		}
		else {

			if (static_cast<int>(mScaffold.size()) != 0) {
				mTransform.UndoPosition();
			}
			else {

				ver3 pos = obj->GetPosition();
				ver3 scale = obj->GetScale();

				if (pos.y + scale.y * 2.0f <= mTransform.pos.y) {

					mTransform.pos.y = pos.y + scale.y * 2.2f;
					state->SetChange(static_cast<int>(PlayerStateMove::ride));
					
					bool flag = true;
					for (auto& data : mScaffold) {
						if (data == obj->GetObjectID()) {
							flag = false;
							break;
						}
					}

					if (flag) mScaffold.push_back(obj->GetObjectID());

				}
				else {
					mTransform.UndoPosition();
				}
			}
		}

		Player::SetChangeState(false);

	}
	else if (tag == "Trampoline") {

		mStateManager->GetState("move")->SetChange(static_cast<int>(PlayerStateMove::trampoline));
		Transform transform = obj->GetReferenceTransform();
		mTrampoline.vel = transform.GetUp();
		mTrampoline.flag = true;

		Player::SetChangeState();
		
		State* state = mStateManager->GetState("attack");

		if (state->GetName() == "Assault")state->SetChange();

		mAudioManager->Play("trampoline");

	}
	else if (tag == "ElevatorBottom") {

		State* state = mStateManager->GetState("move");
		state->SetChange(static_cast<int>(PlayerStateMove::elevator));

		mElevatorInfo.elevator = obj;
		mElevatorInfo.vel = obj->GetVer3();

		Player::SetChangeState();

	}
	else if (tag == "Balloon") {

		State* state = mStateManager->GetState("move");
		state->SetChange(static_cast<int>(PlayerStateMove::balloon));
		mAudioManager->Play("Balloon");
		state = mStateManager->GetState("attack");
		if (state->GetName() == "Assault") state->SetChange();
		
	}
	else if (tag == "Goal") {
		mGoal = mKey;
	}

}

void Player::OnCollisionExit(crint id)
{

	auto obj = mScene->GetGameObject(id);
	cstring tag = obj->GetTag();

	if (tag == "Scaffold") {

		State* state = mStateManager->GetState("move");

		if (state->GetNowState() == (uint)(PlayerStateMove::ride)) {
			
			mScaffold.remove_if([&](int id) { return id == obj->GetObjectID(); });

			if (static_cast<int>(mScaffold.size()) == 0) {
				state->SetChange(static_cast<int>(PlayerStateMove::fall));
			}
		}

	}
	else if (tag == "ElevatorBottom") {

		State* state = mStateManager->GetState("move");

		if (state->GetName() == "PlayerGround") {

			state->SetChange();

		}

	}
	else if (tag == "Goal") {
		mGoal = false;
	}

}

void Player::OnCollisionStay(crint id)
{

}

void Player::SetAnim(crstring set)
{
	mDraw->SetAnimName(set);
}

void Player::Set(crstring set)
{
	mDraw->SetAnimName(set);
}

void Player::Set()
{
	mTrampoline.flag = false;
	mJump = false;
	mStop = false;
}
