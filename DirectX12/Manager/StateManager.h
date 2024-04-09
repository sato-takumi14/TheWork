#pragma once

/***********************************************************************************************************
*
*	StateManager
*
*																				制作者：佐藤匠
*																				制作日：03/16
*
************************************************************************************************************
* Stateを管理するクラス
************************************************************************************************************/

#include"Interface\Interface.h"
#include"Interface\Manager.h"

class State;

namespace StateManager
{

	/*ステートを管理するためのデータクラス*/
	class Data
	{
	private:

		State* mState{};

	public:

		inline Data() : mState(nullptr) {}
		inline Data(State* state) : mState(state) {}
		void Update();
		void FixedUpdate();
		void Uninit();
		inline State* GetState()const { return mState; }

	};

	/*ステートを管理するためのクラス*/
	class Device : public Interface, public Manager
	{
	private:

		std::unordered_map<uint, Data> mData{};

	public:

		~Device();
		void Update()override;
		void FixedUpdate()override;
		void Uninit()override;

		void AddState(cruint key, State* state);

		State* GetState(cruint key);

	};

}