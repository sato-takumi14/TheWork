#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F11/05
*
********************************************************************************************
* Score
********************************************************************************************/

#include"GameObject.h"

class Score : public GameObject
{
private:

	list<DrawInfo> mInfo;
	int mScore{};
	uint mTexID = -1;

public:

	void Init()override;
	void Start()override;
	void Update()override;
	void Uninit()override;

	const list<DrawInfo> GetDrawInfos()const override { return mInfo; }

	void Set(crint set)override { mScore += set; }
	cint GetInt()const override { return mScore; }

};