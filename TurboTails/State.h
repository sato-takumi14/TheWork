#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/14
*
********************************************************************************************
* ステートパターンで動かすための基底クラス
********************************************************************************************/

#include"Interface.h"
#include"GameObject.h"

class MeshField;

enum class PlayerRotationState {
	none,
	front,
	right,
	left,
	back,
	frontright,
	frontleft,
	backright,
	backleft,
};

enum class PlayerStateMove
{
	none,
	ground,
	jump,
	fall,
	ride,
	trampoline,
	elevator,
	flydash,
	rundash,
	balloon,
};

enum class CameraStatus {
	normal,
	dash,
	hit,
	move,
};

class State : public Interface
{
protected:

	class GameObject* mTarget{};
	bool mChange{};
	int mID{};
	string mName{};
	int mNextState = -1;
	int mNext = (int)(PlayerStateMove::none);
	list<Component*> mComponent{};

public:

	void Update()override {
		for (auto& data : mComponent) { data->Update(); }
		mComponent.remove_if([](Component* com) { return com->Destroy(); });
	}
	void Uninit()override {
		for (auto& data : mComponent) {
			data->Uninit();
			delete data;
		}
		mComponent.clear();
	}

	void Set(crint set)override { mNextState = set; }
	virtual State* NextState()const { return nullptr; }
	virtual const State GetNextState()const { return State(); }

	void ChangeState() { if (mChange)mTarget->ChangeState(this, mID); }

	void Delete() {
		Uninit();
		delete this;
	}

	virtual void SetParameter() {}
	void SetID(crint id) { mID = id; }
	cint GetID()const { return mID; }
	cbool GetChange()const { return mChange; }
	void SetChange(crbool change = true) { mChange = change; }
	virtual cint GetNowState()const { return -1; }

	crstring GetName()const { return mName; }

	virtual void Set(GameObject* target, MeshField* field) {}
	void SetChange(crint state, crbool change = true) { 
		mNext = state;
		mChange = change;
	}

	void AddComponent(Component* com) {
		com->Init();
		mComponent.push_back(com);
	}

};