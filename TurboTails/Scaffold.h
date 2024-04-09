#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF11/10
*
********************************************************************************************
* Scaffold
********************************************************************************************/

#include"GameObject.h"

class Scaffold : public GameObject
{
private:

	uint mModelID = -1;

public:

	void Init()override;
	void Start()override;
	void Update()override;

	void OnCollisionEnter(crint id)override;
	void OnCollisionStay(crint id)override;
	void OnCollisionExit(crint id)override;

	cuint GetModelID()const { return mModelID; }
	cstring GetShaderName()const { return "multi"; }

};