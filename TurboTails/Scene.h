#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/14
*
********************************************************************************************
* ���ݓ������I�u�W�F�N�g�����N���X
********************************************************************************************/

//#include<thread>

#include"GameObject.h"
#include"Manager.h"
#include"Layer.h"
#include"TextureManager.h"
#include"ModelManager.h"
#include"ParticleManager.h"

#include"Collision.h"
#include"Input.h"
#include"Camera.h"
#include"Load.h"
#include"Shader.h"
#include"ActionKey.h"

#if _DEBUG

#define DEBUG_MODE false

#include"ObjectDebug.h"
#include"Debug.h"

#endif // _DEBUG


class Scene
{
private:

	int mGameObjectIndex{};
	int mClassIndex{};
	int mManagerIndex{};

	list<int> mEraseGameObjectIDs{};

	unordered_map<string, int> mClass{};
	unordered_map<int, list<int>> mSceneObjectIDs{};

	GameObject* mDebugCamera{};

	string mStageFilePath{};
	string mName{};

	Collision mCollision{};
	ParticleManager* mParticleManager{};

protected:

	list<GameObject*> mGameObject{};
	list<GameObject*> mStartGameObject{};

	list<Manager*> mManager{};
	list<Manager*> mStartManager{};

	Layer mLayer{};

#if _DEBUG


#endif // _DEBUG

	bool mDebug{};

protected:

	void SetStageFilePath(crstring file) { mStageFilePath = "asset\\stage\\" + file; }
	void SetName(crstring name) { mName = name; }

public:

	/*��{�I�ȏ���*/

	virtual void Init() {

		ModelManager::SetScene(this);
		TextureManager::SetScene(this);
		ActionKey::SetScene(this);

		mParticleManager = new ParticleManager();
		mParticleManager->Init();
		AddManager(mParticleManager);

	}

	virtual void Start() {

		for (auto& data : mStartGameObject) {
			data->Start();
		}
		mStartGameObject.clear();

		for (auto& data : mStartManager) {
			data->Start();
		}
		mStartManager.clear();

	}

	virtual void Update() {

		if (!mDebug) {

			for (auto& data : mGameObject) {
				data->Update();
				if (data->GetDestroy()) {
					mEraseGameObjectIDs.emplace_back(data->GetObjectID());
				}
			}

			for (auto& data : mManager) {
				data->Update();
			}

		}

		mCollision.Update();
		mLayer.Update();

#if _DEBUG

		if (Input::GetKeyTrigger(VK_TAB)) {
			mDebug = !mDebug;
			mDebugCamera->Set(Vector3::zero());
			mDebugCamera->Set(true);
		}

		if (mDebug) {
			mDebugCamera->Update();
		}

		Debug::MasterUpdate();
		ObjectDebug::Update();

#endif // _DEBUG

		mGameObject.remove_if([](GameObject* obj) { return obj->Destroy(); });
		mManager.remove_if([](Manager* manager) { return manager->Destroy(); });


	}

	virtual void Draw() { mLayer.Draw(); }

	virtual void FixedUpdate() {

		if (!mDebug) {

			for (auto& data : mGameObject) {
				data->FixedUpdate();
				if (data->GetDestroy()) {
					mEraseGameObjectIDs.emplace_back(data->GetObjectID());
				}
			}

			for (auto& data : mManager) {
				data->FixedUpdate();
			}

		}

		mLayer.Update();

#if _DEBUG

		ObjectDebug::Update();

#endif // _DEBUG

		mGameObject.remove_if([](GameObject* obj) { return obj->Destroy(); });
		mManager.remove_if([](Manager* manager) { return manager->Destroy(); });

	}

	virtual void Uninit() {

#if _DEBUG

		ObjectDebug::Uninit();
		Debug::MasterUninit();

#endif // _DEBUG


		for (auto& data : mGameObject) {
			data->Uninit();
			delete data;
		}

		mGameObject.clear();

		for (auto& data : mManager) {
			data->Uninit();
			delete data;
		}

		mManager.clear();

		mClass.clear();
		mSceneObjectIDs.clear();
		mCollision.Uninit();

		ModelManager::Uninit(this->GetName());
		TextureManager::Uninit(this->GetName());

	}

/***********************************************************/

	const list<int> GetEraseGameObjectIDs()const { return mEraseGameObjectIDs; }
	void AddEraseGameObjectID(crint id) { mEraseGameObjectIDs.push_back(id); }
	cstring GetName()const { return mName; }
	virtual cbool GetLoad()const { return false; }
	Collision* GetCollision() { return &mCollision; }
	Layer* GetLayer() { return&mLayer; }
	ParticleManager* GetParticleManager() { return mParticleManager; }

/***********************************************************/
	/*Add*/

	GameObject* AddGameObject(GameObject* obj, crbool set = true) {

		obj->SetScene(this);
		obj->SetObjectID(mGameObjectIndex++);
		obj->Init();
		mGameObject.push_back(obj);
		mStartGameObject.push_back(obj);

#if _DEBUG

		if (set) {
			ObjectDebug::SetObject(obj);
		}

#endif // _DEBUG



		mLayer.AddGameObject(obj);

		bool flag = true;

		for (auto& data : mClass) {
			if (data.first == obj->GetClass()) {
				flag = false;
				obj->SetClassID(data.second);
				mSceneObjectIDs[data.second].emplace_back(obj->GetObjectID());
				break;
			}
		}

		if (flag) {
			obj->SetClassID(mClassIndex);
			mClass[obj->GetClass()] = mClassIndex;
			mSceneObjectIDs[mClassIndex++].emplace_back(obj->GetObjectID());
		}

		return obj;

	}

	Manager* AddManager(Manager* manager) {
		manager->SetScene(this);
		manager->SetFilePath(mStageFilePath);
		manager->SetManagerID(mManagerIndex++);
		manager->Init();
		mManager.push_back(manager);
		mStartManager.push_back(manager);
		return manager;
	}

/***********************************************************/
	/*GameObject�擾�֐��Q*/

	/*�I�u�W�F�N�gID*/
	GameObject* GetGameObject(crint id) {
		for (auto& data : mGameObject) {
			if (data->GetObjectID() == id) {
				return data;
			}
		}
		return nullptr;
	}

	/*�^�O*/
	GameObject* GetGameObjectTag(crstring tag) {
		for (auto& data : mGameObject) {
			if (data->GetTag() == tag) {
				return data;
			}
		}
		return nullptr;
	}

	/*�N���XID*/
	list<GameObject*> GetGameObjectsID(crint classid) {
		list<GameObject*> objects{};
		for (auto& data : mGameObject) {
			if (data->GetClassID() == classid) {
				objects.push_back(data);
			}
		}
		return objects;
	}

	/*�V�[����ɑ��݂�������̃^�O*/
	list<GameObject*> GetGameObjectsTag(crstring tag) {
		list<GameObject*> objects{};
		for (auto& data : mGameObject) {
			if (data->GetTag() == tag) {
				objects.push_back(data);
			}
		}
		return objects;
	}

	/*�I�u�W�F�N�gID*/
	template <typename T>
	T* GetGameObject(crint id) {
		for (auto& data : mGameObject) {
			if (data->GetObjectID() == id) {
				return dynamic_cast<T*>(data);
			}
		}
		return nullptr;
	}

	/*�^�O*/
	template <typename T>
	T* GetGameObject(crstring tag) {
		for (auto& data : mGameObject) {
			if (data->GetTag() == tag) {
				return dynamic_cast<T*>(data);
			}
		}
		return nullptr;
	}

/***********************************************************/

	Camera* GetMainCamera()const {

		for (auto& data : mGameObject) {
			if (data->GetTag() == "MainCamera") {
				return dynamic_cast<Camera*>(data);
			}
		}

		return nullptr;

	}	

/***********************************************************/

	Manager* GetManager(cruint id)const {
		for (auto& data : mManager) {
			if (data->GetManagerID() == id) {
				return data;
			}
		}
		return nullptr;
	}

	Manager* GetManagerTag(crstring tag) {

		for (auto& manager : mManager) {
			if (manager->GetTag() == tag) {
				return manager;
			}
		}

		return nullptr;

	}

/***********************************************************/

	void SetDebugCamera(GameObject* set) { mDebugCamera = set; }

};