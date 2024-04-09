#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/14
*
********************************************************************************************
* 複数のオブジェクトでの同じ処理をするものを記述するための基底クラス
********************************************************************************************/

#include"Interface.h"

class Component : public Interface
{

};

namespace Function
{

	/*一度だけ実行*/
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

	/*入れた関数の順番に実行*/
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

	/*交互に実行*/
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