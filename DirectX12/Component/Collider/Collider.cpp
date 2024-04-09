
#include "Main.h"
#include "Collider.h"

#include"Interface\Scene.h"

void Collider::Update()
{

	auto numbers = mTarget->GetScene()->GetEraseObjectIDs();
	for (auto& data : numbers) {
		mObjIDs.remove_if([data](uint id) { return id == data; });
	}

	for (auto& data : mObjIDs) {
		if (not mTrigger)mTarget->GetTransform()->UndoPosition();
		mTarget->OnCollisionEnter(data);
	}

}

void Collider::Uninit()
{
	mTarget->GetScene()->AddEraseObjectID(mTarget->GetObjectID());
	mObjIDs.clear();
}

void Collider::OnCollisionEnter(cruint id)
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

void Collider::OnCollisionExit(cruint id)
{

	for (auto& data : mObjIDs) {
		if (data = id) {
			mTarget->OnCollisionExit(id);
			mObjIDs.remove_if([id](uint data) { return data == id; });
			break;
		}
	}

	

}
