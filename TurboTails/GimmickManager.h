#pragma once

#include"Manager.h"

class GameObject;

class GimmickManager : public Manager
{
private:

	list<GameObject*> mTranmpoline{};
	class StageManager* mStageManager{};

public:

	GimmickManager(StageManager* stage) : mStageManager(stage) {}

	void Init()override;
	void Update()override;
	void Uninit()override;

};