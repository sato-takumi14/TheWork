
#include"Main.h"

#include "MultiGameObject.h"

#include"Scene.h"
#include"CreateInstance.h"
#include"MultiDrawModel.h"

MultiGameObject::MultiGameObject(
	Scene* scene,
	crstring instance,
	const list<Transform>& transform, 
	const MultiDrawGameObjectState& state)
{

	GameObject* obj;
	for (auto& data : transform) {
		obj = CreateInstance::CreateObject(instance);
		obj->SetMultiGameObject(this);
		obj->SetTransform(data);
		scene->AddGameObject(obj);
		mModelID = obj->GetModelID();
		mObjects.push_back(obj);
	}

	mShader = obj->GetShaderName();

	mTransforms = transform;

	mState = state;

}

void MultiGameObject::Init()
{
	mLayer.push_back("3D");
	mClass = "MultiGameObject";
}

void MultiGameObject::Start()
{
	switch (mState) {

	case MultiDrawGameObjectState::model:
		mDraw = new MultiDrawModel(this, mModelID, mShader, list<int>());
		break;

	case MultiDrawGameObjectState::material:

		break;
	}
}

void MultiGameObject::Update()
{
	mObjects.remove_if([](GameObject* obj) { return obj->GetDestroy(); });
}

void MultiGameObject::Draw()
{
	mDraw->Set(static_cast<uint>(mObjects.size()));
	mDraw->Draw();
}

void MultiGameObject::Uninit()
{
	GameObject::Uninit();
	mObjects.clear();
}

void MultiGameObject::AddGameObject(GameObject* obj)
{
	mObjects.push_back(obj);
}

const list<Transform> MultiGameObject::GetTransforms() const
{

	list<Transform> transforms;

	for (auto& data : mObjects) {
		transforms.push_back(data->GetReferenceTransform());
	}

	return transforms;
}

void MultiGameObject::Remove(crint id)
{
	mObjects.remove_if([&](GameObject* obj) { return obj->GetObjectID() == id; });
}
