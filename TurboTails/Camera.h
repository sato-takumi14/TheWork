#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF09/05
*
********************************************************************************************
* Camera
********************************************************************************************/

#include"GameObject.h"

class Camera : public GameObject
{
private:

	GameObject* mTarget{};
	ver3 mTargetPos{};
	float mSensitivity{};

	D3DXMATRIX mViewMatrix{};
	D3DXMATRIX mProjectionMatrix{};

	long mMouseX{};
	ver3 mAddRot{}, mOldRot{};

	float mShakeAmplitude{};
	int mShakeTime{};
	float mShakeOffset{};

	bool mTitle{};

public:

	Camera(crbool main = false);

	void Init()override;
	void Start()override;
	void Draw()override;

	void OnCollisionEnter(crint id)override;
	void OnCollisionStay(crint id)override;
	void OnCollisionExit(crint id)override;

	cver3 GetVer3()const override;

	const D3DXMATRIX GetMatrix()const override { return mViewMatrix; }
	cmatrix GetProjectionMatrix()const { return mProjectionMatrix; }
	cbool CheckView(crver3 pos, crfloat len = 0.0f);

	void Set(crfloat set) { mShakeAmplitude = set; }

};