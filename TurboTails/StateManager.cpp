
#include"Main.h"

#include "StateManager.h"

#include"State.h"

StateManager::Info::~Info()
{
	if (mState) {
		mState->Uninit();
		delete mState;
	}
}

void StateManager::Info::Update()
{
	mState->Update();
	if (mState->GetChange()) {
		State* next = mState->NextState();
		next->Init();
		mState->Uninit();
		delete mState;
		mState = next;
	}
}

void StateManager::Info::Uninit()
{
	if (mState) {
		mState->Uninit();
		delete mState;
		mState = nullptr;
	}
}

StateManager::~StateManager()
{
	Uninit();
}

void StateManager::Update()
{

	for (auto& data : mState) {
		data->Update();
	}

	mState.remove_if([&](State* state) {
		if (state->GetChange()) {
			Change(state);
			state->Delete();
			return true;
		}
		return false;
		});

	for (auto& data : mStateInfo) { data.second->Update(); }

}

void StateManager::Uninit()
{

	for (auto& data : mState) {
		data->Uninit();
		delete data;
	}

	mState.clear();

	for (auto& data : mStateInfo) {
		data.second->Uninit();
		delete data.second;
	}

	mStateInfo.clear();

}

void StateManager::AddState(State* state)
{

	state->Init();
	state->SetID(mIndex++);

	mState.push_back(state);

}

void StateManager::AddState(crstring key, State* state)
{
	state->Init();
	mStateInfo[key] = new Info(state);
}

void StateManager::Change(State* state)
{

	State* next = state->NextState();
	next->SetID(state->GetID());

	mState.push_back(next);

}

void StateManager::SetParameter()
{

	for (auto& data : mState) {
		data->SetParameter();
	}

	for (auto& data : mStateInfo) {
		data.second->GetState()->SetParameter();
	}

}

State* StateManager::GetState(crint id)
{
	
	for (auto& data : mState) {
		if (data->GetID() == id) {
			return data;
		}
	}

	return nullptr;

}

State* StateManager::GetState(crstring key)
{
	for (auto& data : mStateInfo) { if (data.first == key)return data.second->GetState(); }
	return nullptr;
}
