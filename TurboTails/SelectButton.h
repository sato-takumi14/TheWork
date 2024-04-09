#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF02/05
*
********************************************************************************************
* SelectButton
********************************************************************************************/

#include"GameObject.h"

class SelectButton : public GameObject
{
private:

	DrawInfo mInfo{};
	ver4 mColor{};
	uint mTexID = -1;

	bool mFlag = false;

public:

	void Init()override;
	void Start()override;
	void Update()override;
	void Draw()override;

	const DrawInfo GetDrawInfo()const override { return mInfo; }

	void Set(crbool set)override { mFlag = set; }

};