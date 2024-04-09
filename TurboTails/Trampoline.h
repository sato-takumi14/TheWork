#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF11/17
*
********************************************************************************************
* Trampoline
********************************************************************************************/

#include"GameObject.h"

class Trampoline : public GameObject
{
private:

	GameObject* mPlayer{};
	class MeshField* mField{};

	static ver3 mVel;

	uint mModelID = -1;

public:

	void Init()override;
	void Start()override;
	void Update()override;
	
	void OnCollisionEnter(crint id)override;
	void OnCollisionStay(crint id)override;
	void OnCollisionExit(crint id)override;

	cver3 GetOffset()const override { return mTransform.GetUp(); }
	cuint GetModelID()const override { return mModelID; }
	cstring GetShaderName()const override { return "multi"; }

};