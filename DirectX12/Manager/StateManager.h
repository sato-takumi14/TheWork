#pragma once

/***********************************************************************************************************
*
*	StateManager
*
*																				����ҁF������
*																				������F03/16
*
************************************************************************************************************
* State���Ǘ�����N���X
************************************************************************************************************/

#include"Interface\Interface.h"
#include"Interface\Manager.h"

class State;

namespace StateManager
{

	/*�X�e�[�g���Ǘ����邽�߂̃f�[�^�N���X*/
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

	/*�X�e�[�g���Ǘ����邽�߂̃N���X*/
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