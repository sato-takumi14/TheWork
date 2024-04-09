
#include"Main.h"

#include "ParticleManager.h"

#include"Time.h"
#include"Scene.h"

void ParticleManager::Init()
{
}

void ParticleManager::Update()
{

	erase_if(mParticles, [](auto& data) { return data.second->GetDestroy(); });

}

void ParticleManager::Uninit()
{
	mParticles.clear();
}

Particle* ParticleManager::GetParticle(crstring name)
{
	for (auto& data : mParticles) { if (data.first == name)return dynamic_cast<Particle*>(data.second); }
	return nullptr;
}

Particle* ParticleManager::CreateParticle(
	crstring name, 
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
	
	Particle* particle = new Particle(
		transform, 
		shader, 
		num, 
		model, 
		mode, 
		end,
		speed,
		size,
		diffuse,
		bloom);

	particle->SetTransform(transform);

	mParticles[name] = particle;
	mScene->AddGameObject(particle);

	return particle;

}
