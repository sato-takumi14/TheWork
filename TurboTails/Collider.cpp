
#include"Main.h"

#include "Collider.h"
#include"GameObject.h"
#include"Scene.h"
#include"SceneManager.h"

void Collider::Update()
{

	auto numbers = mTarget->GetScene()->GetEraseGameObjectIDs();

	for (auto& data : numbers) {
		mObjIDs.remove_if([data](int id) { return id == data; });
	}

	for (auto& id : mObjIDs) {
		mTarget->OnCollisionStay(id);
	}

}

void Collider::Uninit()
{
	cint id = mTarget->GetObjectID();
	mTarget->GetScene()->AddEraseGameObjectID(id);
	mObjIDs.clear();
}

void Collider::OnCollisionEnter(crint id)
{

	bool flag = true;

	for (auto& data : mObjIDs) {
		if (data == id) {
			flag = false;
			break;
		}
	}

	if (flag) {
		mTarget->OnCollisionEnter(id);
		mObjIDs.emplace_back(id);
	}

}

void Collider::OnCollisionExit(crint id)
{

	int erase = -1;
	bool flag = true;
	for (auto& data : mObjIDs) {
		if (data == id) {
			mTarget->OnCollisionExit(id);
			erase = id;
			flag = false;
			break;
		}
	}

	mObjIDs.remove_if([erase](int objID) {return objID == erase; });

}

cint Collider::GetInt() const
{
	return mTarget->GetObjectID();
}

cver3 Collider::GetVer3() const
{
	return mTarget->GetPosition() + mOffset;
}

cver2 Collider::GetVer2() const
{
	ver3 pos = mTarget->GetPosition();
	return cver2(pos.x + mOffset.x, pos.y + mOffset.x);
}

cstring Collider::GetString() const
{
	return mTarget->GetTag();
}

cbool Collider::GetBool() const
{
	return mTarget->GetDestroy();
}

cbool Collider::GetActivate() const
{
	return mTarget->GetActivate();
}
