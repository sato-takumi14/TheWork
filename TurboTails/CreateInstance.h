#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F12/09
*
********************************************************************************************
* �C���X�^���X�𐶐�����N���X
********************************************************************************************/

#include"Main.h"

class GameObject;
class Scene;
class Component;

class CreateInstance
{
private:

	static unordered_map<string, function<GameObject* ()>>		mGameObject;	//�Q�[���I�u�W�F�N�g
	static unordered_map<string, function<Scene* ()>>			mScene;			//�V�[��
	static unordered_map<string, function<Component* ()>>		mComponent;		//�R���|�[�l���g

public:

	static void Init();
	static void Uninit();

/*********************************************************************************************************/

	/*Add�Œǉ������N���X���ŃC���X�^���X�𐶐�����*/
	static GameObject* CreateObject(crstring name);
	static Scene* CreateScene(crstring name);
	static Component* CreateComponet(crstring name);

/*********************************************************************************************************/

	/*�w�肵���^�������Őݒ肵��key�Ő�������悤�Ɋ֐����Z�b�g*/
	template <typename T>
	static void AddGameObject(crstring name) { 

		for (auto& data : mGameObject) { if (data.first == name)return; }
		mGameObject[name] = []() { return new T; };
	}

	template <typename S>
	static void AddScene(crstring name) {

		for (auto& data : mScene) { if (data.first == name)return; }
		mScene[name] = []() { return new S; };

	}

	template <typename C>
	static void AddComponent(crstring name) {

		for (auto& data : mComponent) { if (data.first == name) return; }
		mComponent[name] = []() { return new C; };

	}

/*********************************************************************************************************/

	/*�������J��*/
	static void GameObjectClear();
	static void SceneClear();
	static void ComponentClear();

/*********************************************************************************************************/

};