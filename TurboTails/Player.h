#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：11/14
*
********************************************************************************************
* プレイヤーオブジェクト
********************************************************************************************/

#include"GameObject.h"

class Player : public GameObject
{
private:

	/*プレイヤー内で使用する構造体*/
	typedef struct KnockBack {

		bool flag;
		ver3 back;
		class Time* time;

	public:

		KnockBack() : flag(false), back(Vector3::zero()), time(nullptr) {}

	}KnockBack;

	typedef struct TrampolineInfo {

		bool flag;
		ver3 vel;
		GameObject* trampoline;

	public:

		TrampolineInfo() : flag(false), vel(Vector3::zero()), trampoline(nullptr) {}

	}TrampolineInfo;

	typedef struct Info {

	public:

		GameObject* elevator;
		ver3 vel;

	public:

		Info() : elevator(nullptr), vel(Vector3::zero()) {}
		Info(GameObject* obj) : elevator(obj), vel(Vector3::zero()) {}

	}ElevatorInfo;

private:

	/*メンバ変数*/

	class MeshField* mMeshField{};

	TrampolineInfo mTrampoline{};
	KnockBack mKnockBack{};
	ElevatorInfo mElevatorInfo{};

	ver3 mMoveVel{};
	ver3 mStartPos{};

	bool mMove{};
	uint mModelID = -1;
	bool mJump{};
	float mSpeed = 0.1f;
	float mHeight{};
	bool mStop{};
	bool mKey{};
	bool mGoal{};

	list<int> mScaffold{};

private:

/****************************************************************************************************/
	/*関数*/
	void Move();
	void Deceleration();
	void KnockBack();

	void SetChangeState(crbool flag = true);

/****************************************************************************************************/

public:

	void Init()override;
	void Start()override;
	void Update()override;
	void Draw()override;
	void Uninit()override;

	void OnCollisionEnter(crint id)override;
	void OnCollisionExit(crint id)override;
	void OnCollisionStay(crint id)override;

	void Set(crbool set)override { mJump = set; }
	void SetStop(crbool set = true) { mStop = set; }
	void SetAnim(crstring set)override;
	void Set(crstring set)override;
	void Set(crfloat set)override { mSpeed = set; }

	void Set()override;
	void SetKey(crbool key = true) { mKey = key; }

	cbool GetBool()const override { return mKnockBack.flag; }
	cver3 GetVer3()const override { return mTrampoline.vel; }
	cstring GetParticleName()const override { return "charge"; }
	cbool GetMove()const { return mMove; }
	cfloat GetFloat()const override { return mSpeed; }
	float* GetPFloat()override { return &mSpeed; }

	cbool GetKey()const { return mKey; }
	cbool GetGoal()const { return mGoal; }

	ElevatorInfo* GetElevatorInfo() { return &mElevatorInfo; }

	GameObject* GetTranmpoline()const { return mTrampoline.trampoline; }

};