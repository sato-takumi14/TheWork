#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF11/17
*
********************************************************************************************
* Jewel
********************************************************************************************/

#include"GameObject.h"

class Jewel : public GameObject
{
private:

	GameObject* mPlayer{};
	float mAngle{};
	uint mModelID = -1;
	MATERIAL mMaterial{};

public:

	void Init()override;
	void Start()override;
	void Update()override;

	void OnCollisionEnter(crint id)override;
	void OnCollisionExit(crint id)override;
	void OnCollisionStay(crint id)override;

	cuint GetModelID()const override { return mModelID; }
	cstring GetShaderName()const override { return "material"; }

	const MATERIAL GetMaterial()const override { return mMaterial; }

};