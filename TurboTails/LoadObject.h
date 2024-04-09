#pragma once

#include"GameObject.h"

class LoadObject : public GameObject
{
private:

	DrawInfo mInfo{};
	int mTime{};

public:

	void Init()override;
	void Update()override;

	const DrawInfo GetDrawInfo()const override { return mInfo; }

};