#pragma once

#include"Manager.h"
#include"UI.h"

class GameObject;

class UIManager : public Manager
{
private:

	unordered_map<string, GameObject*> mUI;
	list<UI> mData{};

public:

	void Init()override;
	void Update()override;
	void Uninit()override;

	GameObject* GetUI(crstring name) { return mUI[name]; }

};