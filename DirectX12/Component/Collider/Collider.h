#pragma once

#include"Interface\Component.h"
#include"Interface\Interface.h"
#include"Interface\GameObject.h"
#include"System\Vector.h"

class Collider : public Component, public Interface
{
protected:

	GameObject::Interface* mTarget{};
	Vector3 mOffset{};
	std::list<int> mObjIDs{};
	float mRadius{};
	uint mID = -1;
	bool mTrigger{};

public:

	/*’Êíˆ—*/
	void Update()override;
	void Uninit()override;

	/*“–‚½‚è”»’è*/
	void OnCollisionEnter(cruint id);
	void OnCollisionExit(cruint id);

	/*Set*/
	void Set(crfloat set)override { mRadius = set; }
	void Set(crbool set)override { mTrigger = set; }

	/*Get*/
	inline cuint GetUInt()const override { return mTarget->GetObjectID(); }
	inline cVector3 GetVector3()const override { return mTarget->GetWorldPosition() + mOffset; }
	inline cstring GetString()const { return mTarget->GetTag(); }
	inline cbool GetBool()const { return mTarget->GetDestory(); }
	inline cbool GetActivate()const { return mTarget->GetActive(); }

};