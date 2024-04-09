#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* Parent�N���X�F�e�q�֌W���Ǘ�����N���X
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