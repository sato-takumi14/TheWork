#pragma once

/***********************************************************************************************************
*
*	Parent
*
*																				����ҁF������
*																				������F03/16
*
************************************************************************************************************
* GameObject�̐e�q�֌W���Ǘ�����R���|�[�l���g
************************************************************************************************************/

#include"Interface\Component.h"

namespace GameObject
{
	class Interface;
}

namespace Parent
{

	class Data
	{
	private:

		GameObject::Interface* mChild{};
		uint mId{};

	public:

		inline Data() : mChild(nullptr), mId(-1) {}
		inline Data(GameObject::Interface* child, cruint id) : mChild(child), mId(id) {}

		GameObject::Interface* GetChild() { return mChild; }
		inline cuint GetID()const { return mId; }

		inline void SetChild(GameObject::Interface* child) { mChild = child; }
		inline void SetID(cruint id) { mId = id; }

		inline void Decrement() { mId--; }

	};

	class Device : public Component
	{
	private:

		GameObject::Interface* mParent{};
		GameObject::Interface* mHolder{};
		std::list<Data> mChild{};
		uint mIndex{};

	public:

		inline Device(GameObject::Interface* holder) : mHolder(holder), mParent(nullptr) {}
		~Device();

		void Update()override;
		void FixedUpdate()override;

		inline void SetParent(GameObject::Interface* parent) { mParent = parent; }
		void SetChild(GameObject::Interface* child);
		void SetWorldPosition();

		GameObject::Interface* GetChild(cruint id);
		inline GameObject::Interface* GetParent() { return mParent; }
		inline const std::list<Parent::Data>& GetChild()const { return mChild; }

		void Remove(cruint id);

	};

}
