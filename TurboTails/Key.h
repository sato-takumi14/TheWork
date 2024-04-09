#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF11/28
*
********************************************************************************************
* Key
********************************************************************************************/

#include"GameObject.h"

class Key : public GameObject
{
private:

	GameObject* mPlayer{};
	float mAngle{};
	bool mGet{};
	uint mModelID = -1;

public:

	void Init()override;
	void Start()override;
	void Update()override;

	void OnCollisionEnter(crint id)override;
	void OnCollisionExit(crint id)override;
	void OnCollisionStay(crint id)override;

	void SetGameObject(GameObject* set)override;

};