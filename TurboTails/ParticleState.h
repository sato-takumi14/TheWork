#pragma once

#include"State.h"

namespace ParticleState
{
	/*パーティクルインターフェース*/
	class ParticleStateInterface : public State
	{
	protected:

		GameObject* mParent{};
		float mSpeed{};
		bool mEnd{};
		ver3 mVel{};
		ver3 mStartVel{};

	public:
		void Update()override;
		virtual cver3 GetVector()const = 0;
		cbool GetBool()const override { return mEnd; }
	};

	/*上昇するパーティクル*/
	class Rise : public ParticleStateInterface
	{
	private:

		int mStartTime{};
		int mTime{};

	public:

		Rise(GameObject* target, crfloat speed, GameObject* parent);

		void Update()override;
		cver3 GetVector()const override { return Vector3::up() * mSpeed; }
		cbool GetBool()const override { return mTime >= mStartTime; }
		void Set(crint set)override { mTime = set; }

	};

	/*中心に集まるパーティクル*/
	class Collect : public ParticleStateInterface
	{
	private:

		color mStartColor{};

	public:

		Collect(GameObject* target, crfloat speed, GameObject* parent);

		void Update()override;
		cver3 GetVector()const override;

	};

}