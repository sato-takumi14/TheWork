#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* Parentクラス：親子関係を管理するクラス
********************************************************************************************/

#include"Component.h"

class GameObject;

class Parent : public Component
{
private:

	typedef struct {

		int id;
		GameObject* child;
		ver3 vel;

	}Info;

	GameObject* mParent{};
	list<Info> mChild{};
	int mIndex{};

public:

	Parent(GameObject* parent = nullptr) : mParent(parent) {}
	~Parent();

	void Update()override;

	void SetChild(GameObject* child);
	GameObject* GetChild(crint id);
	GameObject* GetParent()const { return mParent; }

};