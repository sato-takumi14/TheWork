#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F01/01
*
********************************************************************************************
* ParticleManager�N���X�F�p�[�e�B�N�����Ǘ�����N���X
********************************************************************************************/

#include"Manager.h"
#include"Particle.h"

class ParticleManager : public Manager
{
private:

	unordered_map<string, GameObject*> mParticles;

public:

	void Init();
	void Update();
	void Uninit();

	Particle* GetParticle(crstring name);
	Particle* CreateParticle(
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
		crbool bloom = false
	);

};