#pragma once

#include"GameObject.h"

class Skydome : public GameObject
{
private:

	GameObject* mPlayer{};
	uint mModelID = -1;

public:

	void Init()override;
	void Start()override;
	void Update()override;

};