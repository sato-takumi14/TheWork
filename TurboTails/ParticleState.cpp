
#include"Main.h"

#include "ParticleState.h"

#include"Random.h"

/*****************************************************************************************************************
* パーティクルインターフェース
******************************************************************************************************************/
void ParticleState::ParticleStateInterface::Update()
{
	mTarget->SetAddLocalPosition(mVel * mSpeed);
	if (mTarget->GetBool()) mTarget->Set();
}

/*****************************************************************************************************************
* 上に上がるパーティクル
******************************************************************************************************************/
ParticleState::Rise::Rise(GameObject* target, crfloat speed, GameObject* parent)
{
	mTarget = target;
	mSpeed = speed;
	mVel = Vector3::up() * mSpeed;
	mStartVel = mVel;
	ver3 size = mTarget->GetVer3();
	size.x = Random::Float(-size.x, size.x);
	size.z = Random::Float(-size.z, size.z);
	mTarget->SetLocalPosition(size);
	mStartTime = Random::Int(1, 100);
}

void ParticleState::Rise::Update()
{

	if (mTime++ >= mStartTime) {
		mVel.y -= mSpeed * 0.1f;
		if (mTarget->GetBool()) {
			mVel = mStartVel;
		}

		ParticleStateInterface::Update();
	}
}

/*****************************************************************************************************************
* 中心に集まるパーティクル
******************************************************************************************************************/
ParticleState::Collect::Collect(GameObject* target, crfloat speed, GameObject* parent)
{
	mTarget = target;
	mSpeed = speed;
	mParent = parent;

	mStartColor = mTarget->GetMaterial().Diffuse;

	ver3 size = mTarget->GetVer3();
	size.x = Random::Float(-size.x, size.x);
	size.y = Random::Float(-size.y, size.y);
	size.z = Random::Float(-size.z, size.z);
	mTarget->SetLocalPosition(size);

	mVel = (parent->GetPosition() - target->GetPosition()) * speed;
	
}

void ParticleState::Collect::Update()
{
	mTarget->SetAddLocalPosition(mVel);
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material = mTarget->GetMaterial();
	material.Diffuse += color(0.0f, 0.0f, 0.1f, 0.0f);
	mTarget->SetMaterial(material);
	if (Vector3::round(mParent->GetPosition()) == Vector3::round(mTarget->GetPosition())) {
		mTarget->Set();
		mVel = (mParent->GetPosition() - mTarget->GetPosition()) * mSpeed;
		material.Diffuse = mStartColor;
		mTarget->SetMaterial(material);
	}
}

cver3 ParticleState::Collect::GetVector() const
{
	return mVel;
}
