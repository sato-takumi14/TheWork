#pragma once

/***********************************************************************************************************
*
*	Scene
*
*																				����ҁF������
*																				������F02/20
*
************************************************************************************************************
* �S�ẴI�u�W�F�N�g�E�}�l�[�W���[���Ǘ����N���X
************************************************************************************************************/

#include"System\Layer.h"
#include"System\InputKey.h"
#include"Interface\Manager.h"
#include"Component\Time.h"

class Scene
{
private:

	std::list<GameObject::Interface*> mObjects{};
	std::list<GameObject::Interface*> mStartObjects{};
	std::list<Manager*> mManagers{};
	std::list<Manager*> mStartManagers{};
	std::list<uint> mEraseObjectIDs{};

	uint mObjectID = 0;

	string mName{};

protected:

	Layer mLayer{};

public:

	/*�ʏ폈��*/
	inline virtual void Init() {}

	inline virtual void Start() {
		for (auto& data : mStartObjects) {
			data->Start();
			data->SetStart(true);
		}
		mStartObjects.remove_if([](GameObject::Interface* obj) { return obj->GetStart(); });
		for (auto& data : mStartManagers) {
			data->Start();
			data->SetStart(true);
		}
		mStartManagers.remove_if([](Manager* data) { return data->GetStart(); });
	}

	inline virtual void Update() {

		for (auto& data : mObjects) {
			data->Update();
			if (data->GetDestory())mEraseObjectIDs.push_back(data->GetObjectID());
		}

		for (auto& data : mManagers) { data->Update(); }

		mLayer.Update();

		mObjects.remove_if([](GameObject::Interface* obj) { return obj->Destroy(); });
		mManagers.remove_if([](Manager* data) { return data->Destroy(); });

	}

	inline virtual void FixedUpdate() {

		if (not Time::GetFixedUpdate())return;

		for (auto& data : mObjects) {
			data->FixedUpdate();
			if (data->GetDestory())mEraseObjectIDs.push_back(data->GetObjectID());
		}

		for (auto& data : mManagers) { data->FixedUpdate(); }

		mLayer.Update();

		mObjects.remove_if([](GameObject::Interface* obj) { return obj->Destroy(); });

	}

	inline virtual void Draw() {
		mLayer.Draw();
	}

	inline virtual void Uninit() {

		for (auto& data : mObjects) {
			data->Uninit();
			delete data;
		}
		mObjects.clear();
		mStartObjects.clear();
		mEraseObjectIDs.clear();

		for (auto& data : mManagers) {
			data->Uninit();
			delete data;
		}
		mManagers.clear();
		mStartManagers.clear();

		mLayer.Uninit();

		InputKey::Uninit();

	}

	/*Set*/
	inline void SetName(crstring name) { mName = name; }

	/*Get*/
	inline cstring GetName()const { return mName; }
	inline const std::list<uint> GetEraseObjectIDs()const { return mEraseObjectIDs; }

	/*etc*/
	inline void AddEraseObjectID(cruint id) { mEraseObjectIDs.emplace_back(id); }

	/*�V�[���ɃQ�[���I�u�W�F�N�g��ǉ�*/
	inline GameObject::Interface* AddGameobject(GameObject::Interface* object) {

		object->SetScene(this);
		object->SetObjectID(mObjectID++);
		object->Init();
		mObjects.push_back(object);
		mStartObjects.push_back(object);
		mLayer.AddGameObject(object);

		return object;

	}

	/*�V�[���Ƀ}�l�[�W���[��ǉ�*/
	inline Manager* AddManager(Manager* manager) {

		manager->SetScene(this);
		manager->Init();
		mManagers.push_back(manager);
		mStartManagers.push_back(manager);
		return manager;

	}

	/*�V�[���ɑ��݂���Q�[���I�u�W�F�N�g��ID�Ŏ擾*/
	inline GameObject::Interface* GetGameObject(cruint id) {
		for (auto& data : mObjects) { if (data->GetObjectID() == id)return data; }
		return nullptr;
	}

	/*�V�[���ɑ��݂���Q�[���I�u�W�F�N�g���^�O�Ŏ擾*/
	inline GameObject::Interface* GetGameObjectTag(crstring tag) {
		for (auto& data : mObjects) { if (data->GetTag() == tag)return data; }
		return nullptr;
	}

};