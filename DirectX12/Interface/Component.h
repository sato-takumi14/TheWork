#pragma once

/***********************************************************************************************************
*
*	Component
*
*																				����ҁF������
*																				������F02/20
*
************************************************************************************************************
* GameObject�œ������R���|�[�l���g�̊��N���X
************************************************************************************************************/

#include"Main.h"

class Component
{
private:

	bool mStart{};

protected:

	bool mDestroy{};	

public:

	inline virtual void Init() {}
	inline virtual void Start() {}
	inline virtual void Update() {}
	inline virtual void FixedUpdate() {}
	inline virtual void Draw() {}
	inline virtual void Uninit() {}

	inline void SetDestroy(crbool set = true)	{ mDestroy = set; }
	inline void SetStart(crbool start)			{ mStart = start; }

	inline cbool GetDestroy()const				{ return mDestroy; }
	inline cbool GetStart()const				{ return mStart; }

	inline cbool Destroy() {
		if (mDestroy) {
			Uninit();
			delete this;
			return true;
		}
		else return false;
	}

};