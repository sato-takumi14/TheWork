
#include "Main.h"
#include "StateManager.h"

#include"Interface\State.h"

namespace StateManager
{

	void Data::Update()
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

	void Data::FixedUpdate()
	{
	}

	void Data::Uninit()
	{
		mState->Uninit();
	}

/*********************************************************************************/

	Device::~Device()
	{
		Uninit();
	}

	void Device::Update()
	{

		for (auto& data : mData) { data.second.Update(); }

	}

	void Device::FixedUpdate()
	{

		for (auto& data : mData) { data.second.FixedUpdate(); }

	}

	void Device::Uninit()
	{
		for (auto& data : mData) { data.second.Uninit(); }
		mData.clear();
	}

	void Device::AddState(cruint key, State* state)
	{
		state->Init();
		mData[key] = Data(state);
	}

	State* Device::GetState(cruint key)
	{
		for (auto& data : mData) { if (data.first == key)return data.second.GetState(); }
		return nullptr;
	}

}