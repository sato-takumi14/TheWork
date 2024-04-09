#pragma once

/***********************************************************************************************************
*
*	State
*
*																				����ҁF������
*																				������F03/16
*
************************************************************************************************************
* �X�e�[�g�p�^�[���œ��������߂̊��N���X
************************************************************************************************************/

#include"Interface\GameObject.h"
#include"Interface\Component.h"

class State
{
protected:

	GameObject::Interface* mTarget{};
	bool mChange{};
	string mName{};
	uint mNextState{};
	std::list<Component*> mComponent{};

public:

	inline State(GameObject::Interface* target) : mTarget(target) {}

	/*�ʏ폈��*/
	inline virtual void Init() {}
	inline virtual void Update() {
		for (auto& com : mComponent) { com->Update(); }
		std::erase_if(mComponent, [](Component* com) { return com->Destroy(); });
	}
	inline virtual void FixedUpdate() {
		for (auto& com : mComponent) { com->FixedUpdate(); }
		std::erase_if(mComponent, [](Component* com) { return com->Destroy(); });
	}
	inline virtual void Uninit() {
		for (auto& com : mComponent) {
			com->Uninit();
			delete com;
		}
		mComponent.clear();
	}

	/*Set*/
	inline void SetChange(crbool set = true) { mChange = set; }
	inline void SetChange(cruint state, crbool set) {
		mNextState = state;
		mChange = set;
	}

	/*Get*/
	inline crstring GetName()const { return mName; }
	inline cbool GetChange()const { return mChange; }

	/*etc*/
	inline virtual State* NextState()const { return nullptr; }
	inline void AddComponent(Component* com) {
		com->Init();
		mComponent.push_back(com);
	}

};