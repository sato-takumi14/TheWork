#pragma once

#include"GameObject.h"

class TitlePlayer : public GameObject
{
private:

	class MeshField* mField{};
	uint mModelID = -1;

public:

	void Init()override;
	void Start()override;
	void Update()override;

	cuint GetModelID()const override { return mModelID; }

};