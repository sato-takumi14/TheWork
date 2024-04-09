#pragma once

#include"GameObject.h"

class Box : public GameObject
{
private:

	list<Transform> mInfos{};
	uint mModelID = -1;
	class Time* mTime{};

public:

	void Init()override;
	void Update()override;

	cuint GetModelID()const override { return mModelID; }
	cstring GetShaderName()const { return "multi"; }

};