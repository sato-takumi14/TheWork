#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/14
*
********************************************************************************************
* �����̃I�u�W�F�N�g�ł̓���������������̂��L�q���邽�߂̊��N���X
********************************************************************************************/

#include"Interface.h"

class Component : public Interface
{

};

namespace Function
{

	/*��x�������s*/
	class DoOnce : public Component
	{
	private:

		function<void()> mFunction{};
		bool mExecution{};

	public:

		DoOnce(const function<void()>& func) : mFunction(func), mExecution(false) {}

		inline void Update()override {

			if (mExecution)return;
			mFunction();
			SetDestroy();
			mExecution = true;

		}

	};

	/*���ꂽ�֐��̏��ԂɎ��s*/
	class Sequence : public Component
	{
	private:

		unordered_map<int, function<void()>> mFunction{};
		int mIndex = 0;

	public:

		Sequence(const unordered_map<int, function<void()>>& func) : mFunction(func) {}

		inline void Update()override {

			mFunction[mIndex++]();
			if (mIndex == mFunction.size())mIndex = 0;

		}

	};

	/*���݂Ɏ��s*/
	class FlipFlop : public Component
	{
	private:

		unordered_map<bool, function<void()>> mFunction{};
		bool mSwitch = false;

	public:

		FlipFlop(const unordered_map<bool, function<void()>>& func) : mFunction(func) {}

		inline void Update()override {

			mFunction[mSwitch]();
			mSwitch = not mSwitch;

		}

	};

}