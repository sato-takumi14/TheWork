#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF12/10
*
********************************************************************************************
* Ring
********************************************************************************************/

#include"GameObject.h"

class Ring : public GameObject
{
private:

	GameObject* mPlayer{};
	float mAddForce{};
	bool mHit{};
	uint mModelID = -1;
	int mTime{};

public:

	void Init()override;
	void Start()override;
	void Update()override;

	void OnCollisionEnter(crint id)override;
	void OnCollisionExit(crint id)override;

	cstring GetShaderName()const override { return "multi"; }
	cuint GetModelID()const override { return mModelID; }

	void SetMultiGameObject(MultiGameObject* obj)override { mMultiGameObject = obj; }

};