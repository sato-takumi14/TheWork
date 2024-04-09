#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF12/25
*
********************************************************************************************
* Elevator
********************************************************************************************/

#include"GameObject.h"

class Bottom : public GameObject
{
private:

	GameObject* mPlayer{};
	ver3 mTopPos{};
	ver3 mOriginPos{};
	ver3 mVel{};
	float mTime{};
	bool mReturn{};
	uint mModelID = -1;

public:

	void Init()override;
	void Start()override;
	void Update()override;

	void OnCollisionEnter(crint id)override;
	void OnCollisionStay(crint id)override;
	void OnCollisionExit(crint id)override;

	void Set(crfloat set) { mTime = set; }
	void Set(crver3 set) { mTopPos = set; }

	cbool GetBool()const override { return mReturn; }
	cver3 GetVer3()const override { return mVel; }

};

class Elevator : public GameObject
{
private:

	uint mModelID = -1;

public:

	void Init()override;
	void Start()override;
	void Update()override;
	void Draw()override;
	void Uninit()override;

};