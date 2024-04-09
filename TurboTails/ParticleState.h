#pragma once

#include"State.h"

namespace ParticleState
{
	/*�p�[�e�B�N���C���^�[�t�F�[�X*/
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

	/*�㏸����p�[�e�B�N��*/
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

	/*���S�ɏW�܂�p�[�e�B�N��*/
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