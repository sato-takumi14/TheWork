#pragma once

/*******************************************************************************************
*
*																§ìŽÒF²“¡ 
*																§ì“úF02/05
*
********************************************************************************************
* TitleLogo
********************************************************************************************/

#include"GameObject.h"

class TitleLogo : public GameObject
{
private:

	DrawInfo mInfo{};
	ver4 mColor{};
	float mSubtraction{};
	bool mStart{};
	int mTime{};
	uint mTexID = -1;

public:

	void Init()override;
	void Start()override;
	void Update()override;

	const DrawInfo GetDrawInfo()const override { return mInfo; }

	cbool GetBool()const override { return mTime >= 100 && mStart; }

};