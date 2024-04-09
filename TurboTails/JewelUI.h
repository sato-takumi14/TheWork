#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF12/05
*
********************************************************************************************
* JewelUI
********************************************************************************************/

#include"GameObject.h"

class JewelUI : public GameObject
{
private:

	DrawInfo mInfo{};
	uint mTexID = -1;

public:

	void Init()override;
	void Start()override;
	void Update()override;
	void Uninit()override;

	const DrawInfo GetDrawInfo()const override { return mInfo; }

};