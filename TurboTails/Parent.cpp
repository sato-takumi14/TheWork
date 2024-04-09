
#include"Main.h"

#include "Parent.h"
#include"GameObject.h"

Parent::~Parent()
{
	mChild.clear();
}

void Parent::Update()
{

	for (auto& data : mChild) {

		ver3 vel = data.child->GetPosition() - mParent->GetPosition();
		data.child->SetAddPosition(vel - data.vel);

	}

}

void Parent::SetChild(GameObject* child)
{
	ver3 vel = child->GetPosition() - mParent->GetPosition();
	mChild.push_back(Info{ mIndex++, child, vel });
}

GameObject* Parent::GetChild(crint id)
{
	for (auto& data : mChild) {
		if (data.id == id) {
			return data.child;
		}
	}
	return nullptr;
}
