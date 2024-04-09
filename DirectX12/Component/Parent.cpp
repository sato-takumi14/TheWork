
#include"Main.h"
#include"Parent.h"

#include"Interface\GameObject.h"

namespace Parent
{

	Device::~Device()
	{
		mChild.clear();
	}

	void Device::Update()
	{
		if (mParent) {

			Vector3 pos;

			for (auto& data : mChild) {

				pos = data.GetChild()->GetWorldPosition() - mParent->GetWorldPosition();
				data.GetChild()->SetLocalPosition(pos);

			}

		}
	}

	void Device::FixedUpdate()
	{
		if (mParent) {

			Vector3 pos;

			for (auto& data : mChild) {

				pos = data.GetChild()->GetWorldPosition() - mParent->GetWorldPosition();
				data.GetChild()->SetLocalPosition(pos);

			}

		}
	}

	void Device::SetChild(GameObject::Interface* child)
	{

		if (mParent) {

			Vector3 pos = child->GetWorldPosition() - mParent->GetWorldPosition();
			child->SetLocalPosition(pos);

		}

		mChild.push_back(Data(child, mIndex++));

		child->GetParent()->SetParent(mHolder);

	}

	void Device::SetWorldPosition()
	{
		if (mParent) mHolder->SetWorldPosition(mParent->GetWorldPosition() + mHolder->GetLocalPosition());
	}

	GameObject::Interface* Device::GetChild(cruint id)
	{
		for (auto& data : mChild) { if (data.GetID() == id)return data.GetChild(); }
		return nullptr;
	}

	void Device::Remove(cruint id)
	{
		std::erase_if(mChild, [id](Data data) { return data.GetID() == id; });
		for (auto& data : mChild) { if (data.GetID() > id)data.Decrement(); }
	}

}