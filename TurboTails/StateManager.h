#pragma once

#include"Manager.h"

class State;

class StateManager : public Manager
{
private:

	class Info
	{
	private:

		State* mState{};

	public:

		Info() : mState(nullptr) {}
		Info(State* state) : mState(state) {}
		~Info();
		void Update();
		void Uninit();
		State* GetState()const { return mState; }

	};

private:

	list<State*> mState{};
	int mIndex{};

	unordered_map<string, Info*> mStateInfo{};

public:

	~StateManager();

	void Update()override;
	void Uninit()override;

	void AddState(State* state);
	void AddState(crstring key, State* state);
	void Change(State* state);

	void SetParameter();


	State* GetState(crint id);
	State* GetState(crstring key);

};