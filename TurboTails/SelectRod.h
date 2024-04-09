#pragma once

#include"GameObject.h"

class SelectRod : public GameObject
{
private:

	DrawInfo mDrawInfo{};
	UI mUIInfo{};

public:

	void Init()override;
	void Update()override;

	const DrawInfo GetDrawInfo()const override { return mDrawInfo; }
	const UI GetUIInfo()const override { return mUIInfo; }

};