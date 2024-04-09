#pragma once

#include"State.h"

namespace CameraState
{

	constexpr float MouseCameraSpeed = 0.005f;

	//カメラステートインターフェース
	class CameraStateInterface : public State
	{
	private:

		bool mShake{};
		float mOffset{};
		int mTime{};
		float mShakeAmplitude{};
		ver3 mOldPos{};

	protected:

		GameObject* mPlayer{};
		ver3 mTargetPos{};
		ver3 mAddRot{};
		float mLength{};
		class MeshField* mField{};

	protected:

		void Move(crver3 data, crfloat size);
		void EasingMove(crver3 data, crfloat size);
		void SetOldPostion(crver3 data) { mOldPos = data; }
		cver3 GetOldPosition()const { return mOldPos; }

	private:

		void Shake();

	public:

		void Update()override;

		void Set(crfloat set)override;
		cfloat GetFloat()const override { return mOffset; }
		cver3 GetVer3()const override { return mTargetPos; }
		cver3 GetRotation()const { return mAddRot; }

	};

	class NoneCamera : public CameraStateInterface
	{
	private:



	public:

		NoneCamera(GameObject* target, GameObject* player, crver3 rot, crfloat length);
		void Update()override;

	};

	//ノーマル状態ステート
	class CameraNormal : public CameraStateInterface
	{
	private:

		bool mDush{};
		ver3 mOldPos{};
		int mTime{};

	private:

		

	public:

		CameraNormal(GameObject* target, GameObject* player, crver3 rot = Vector3::zero());
		CameraNormal(GameObject* target, GameObject* player, crver3 rot, crfloat length);
		void Update()override;
		State* NextState()const override;

	};	

	class CameraDash : public CameraStateInterface
	{
	private:

		bool mStop{};

	public:

		CameraDash(GameObject* target, GameObject* player, crver3 rot = Vector3::zero());
		CameraDash(GameObject* target, GameObject* player, crver3 rot, crfloat length);
		void Update()override;
		State* NextState()const override;

	};

	class HitCamera : public CameraStateInterface
	{
	private:

		

	public:

		HitCamera(GameObject* target, GameObject* player, crver3 rot = Vector3::zero());
		HitCamera(GameObject* target, GameObject* player, crver3 rot, crfloat length);
		void Update()override;
		State* NextState()const override;

	};

	class CameraTracking : public CameraStateInterface
	{
	private:

		ver3 mOldPos{};
		int mTime{};

	private:

		

	public:

		CameraTracking(GameObject* target, GameObject* player, crver3 rot);
		CameraTracking(GameObject* target, GameObject* player, crver3 rot, crfloat length);
		void Update()override;
		State* NextState()const override;

	};

}