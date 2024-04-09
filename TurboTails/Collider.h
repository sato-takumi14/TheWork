#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* Collider�N���X�F�����蔻��������Ƃ��̏������L�q����Ă���
********************************************************************************************/

#include"Component.h"

#include<list>

class Collider : public Component
{
private:



protected:

	class GameObject* mTarget{};
	ver3 mOffset{};
	list<int> mObjIDs{};
	float mRadius{};
	int mID = -1;

public:

	void Update()override;
	void Uninit()override;

	void OnCollisionEnter(crint id);
	void OnCollisionExit(crint id);

	void Set(crfloat set)override { mRadius = set; }
	void Set(crver3 set)override { mOffset = set; }

	cint GetInt()const override;
	cfloat GetFloat()const override { return mRadius; }
	cver3 GetVer3()const override;
	cver2 GetVer2()const override;
	cstring GetString()const override;
	cbool GetBool()const override;

	cbool GetActivate()const;
	crint GetID()const { return mID; }

	GameObject* GetGameObject()const { return mTarget; }

};