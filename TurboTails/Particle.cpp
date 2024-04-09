
#include"main.h"

#include "Particle.h"

#include"Draw2D.h"
#include"TextureManager.h"
#include"Random.h"
#include"Time.h"
#include"Shader.h"
#include"Geometry.h"
#include"ModelManager.h"
#include"ParticleState.h"
#include"Debug.h"

Particle::Particle(
	const Transform& transform, 
	crstring shader, 
	cruint num, 
	crstring model, 
	const Particle::ParticleMode& mode,
	cruint end,
	crfloat speed,
	crver3 size,
	crcolor diffuse,
	crbool bloom)
{

	mTransform = transform;
	mShader = shader;
	mDiffuse = diffuse;

	mParticleState = [](
		const Particle::ParticleMode& mode, 
		GameObject* target, 
		crfloat speed, 
		GameObject* parent)->State* {
		switch (mode) {

		case Particle::ParticleMode::rise:
			return new ParticleState::Rise(target, speed, parent);
			break;

		case Particle::ParticleMode::collect:
			return new ParticleState::Collect(target, speed, parent);
			break;

		}

		return nullptr;

		};

	for (uint i = 0; i < num; i++) {
		mParticles.push_back(new ParticleObject(transform, mode, end, speed, this, mParticleState, size, diffuse));
	}

	Debug::AddResetFunction([&]() {
		for (auto& data : mParticles) {
			data->GetStateManager()->GetState("move")->Set(0);
		}
		});

	mGeometry = new Geometry();
	mGeometry->Create(this);
	
	mModelID = ModelManager::LoadModel(model, "Particle");

	if (bloom) mLayer.push_back("bloom");

}

void Particle::Init()
{
	
	mLayer.push_back("3D");

}

void Particle::Start()
{
	Shader::AddShader("particle", "MultiMaterialColorVS.cso", "MultiMaterialColorPS.cso");
}

void Particle::Update()
{

	if (!mActivate || !mActive)return;

	GameObject::Update();

	for (auto& data : mParticles) {
		data->SetPosition(mTransform.pos);
		data->Update();
	}

}

void Particle::Draw()
{

	if (!mActivate || !mActive)return;

	Shader::SetShader("particle");

	mGeometry->SetGeometry(this);

	MATERIAL param;
	ZeroMemory(&param, sizeof(MATERIAL));
	param.Diffuse = mDiffuse;
	Renderer::SetMaterial(param);

	ModelManager::GetModel(mModelID)->Draw(static_cast<uint>(mParticles.size()));

}

void Particle::Uninit()
{

	GameObject::Uninit();

	for (auto& data : mParticles) {
		data->Uninit();
		delete data;
	}
	mParticles.clear();

	mGeometry->Uninit();
	delete mGeometry;

	//mInfo.clear();

}

Particle::ParticleObject::ParticleObject(
	const Transform& transform, 
	const Particle::ParticleMode& mode, 
	cruint end,
	crfloat speed,
	GameObject* parent,
	const function<State* (const ParticleMode&, GameObject*, crfloat, GameObject*)>& func,
	crver3 size,
	crcolor diffuse)
{
	mTransform = transform;
	mSize = size;

	mParticle = parent;

	mMaterial.Diffuse = diffuse;

	mStateManager = new StateManager();
	mStateManager->AddState("move", func(mode, this, speed, parent));

	mEndTime = end;
	mOriginPos = mTransform.local;

}

void Particle::ParticleObject::Update()
{
	if (!mActivate)return;
	GameObject::Update();
	mTime++;
}

void Particle::ParticleObject::Uninit()
{
	GameObject::Uninit();
}

void Particle::ParticleObject::Set()
{
	mTime = 0;
	mTransform.local = ver3(
		Random::Float(-mSize.x, mSize.x),
		Random::Float(-mSize.y, mSize.y),
		Random::Float(-mSize.z, mSize.z)
	);
}
