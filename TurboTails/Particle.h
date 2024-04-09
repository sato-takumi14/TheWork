#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF12/10
*
********************************************************************************************
* Particle
********************************************************************************************/

#include"GameObject.h"

class Particle : public GameObject
{
public:

	enum class ParticleMode {
		none,
		rise,
		collect,
	};

private:

	class ParticleObject : public GameObject
	{
	private:

		GameObject* mParticle{};
		ver3 mVel{};
		ver3 mOriginPos{};
		uint mTime{};
		uint mEndTime{};
		MATERIAL mMaterial{};
		ver3 mSize{};

	public:

		ParticleObject(
			const Transform& transform,
			const Particle::ParticleMode& mode,
			cruint end,
			crfloat speed,
			GameObject* obj,
			const function<State* (const ParticleMode&, GameObject*, crfloat, GameObject*)>& func,
			crver3 size,
			crcolor diffuse);

		void Update()override;
		void Uninit()override;

		const MATERIAL GetMaterial()const override { return mMaterial; }
		GameObject* GetGameObject()const override { return mParticle; }
		cbool GetBool()const override { return mTime >= mEndTime; }
		cver3 GetVer3()const override { return mSize; }

		void Set()override;
		void SetMaterial(const MATERIAL& material)override { mMaterial = material; }
		void Set(crver3 set)override { mVel = set; }
		void SetGameObject(GameObject* obj)override { mParticle = obj; }

	};

private:

	uint mModelID = -1;

	list<GameObject*> mParticles{};
	class Geometry* mGeometry{};
	string mShader{};
	bool mActive = true;
	color mDiffuse{};

	function<State* (const ParticleMode&, GameObject*, crfloat, GameObject*)> mParticleState{};

public:

	Particle() {}
	Particle(const Transform& transform, cruint num) { mTransform = transform; }
	Particle(
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

	void Init()override;
	void Start()override;
	void Update()override;
	void Draw()override;
	void Uninit()override;

	cuint GetModelID()const override { return mModelID; }
	cstring GetShaderName()const override { return "particle"; }
	const list<GameObject*> GetGameObjects()const override { return mParticles; }

	void SetModelID(cruint id)override { mModelID = id; }
	void Set(crbool set)override { mActive = set; }

};