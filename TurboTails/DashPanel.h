#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF11/30
*
********************************************************************************************
* DashPanel
********************************************************************************************/

#include"GameObject.h"

class DashPanel : public GameObject
{
private:

	DrawInfo mInfo{};

public:

	void Init()override;
	void Start()override;
	void Update()override;

	void OnCollisionEnter(crint id)override;
	void OnCollisionStay(crint id)override;
	void OnCollisionExit(crint id)override;

	const DrawInfo GetDrawInfo()const override { return mInfo; }

};