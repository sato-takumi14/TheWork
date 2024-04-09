#pragma once

#include"Manager.h"

class ItemManager : public Manager
{
private:

	class GameObject* mPlayer{};
	list<GameObject*> mItems{};
	class StageManager* mStageManager{};

public:

	ItemManager() {}
	ItemManager(StageManager* stage) : mStageManager(stage) {}
	void Init()override;
	void Start()override;
	void Update()override;
	void Uninit()override;

};