#pragma once

/***********************************************************************************************************
*
*	CreateInstance
*
*																				����ҁF������
*																				������F02/29
*
************************************************************************************************************
* �w�肵��key�ŃC���X�^���X�𐶐�����N���X
************************************************************************************************************/

#include"Main.h"

namespace GameObject
{
	class Interface;
}

class Scene;

namespace Create
{

	class Device
	{
	private:

		static std::unordered_map<string, std::function<GameObject::Interface* ()>> mGameObjects;	/*�Q�[���I�u�W�F�N�g*/
		static std::unordered_map<string, std::function<Scene* ()>> mScenes;						/*�V�[��*/

	public:

		/*�ʏ폈��*/
		static void MasterInit();
		static void MasterUninit();

		/*Add�Œǉ������e�C���X�^���X�𐶐�����*/
		inline static GameObject::Interface* CreateObject(crstring name) { return mGameObjects[name](); }
		inline static Scene* CreateScene(crstring name) { return mScenes[name](); }

		/*�w�肵���^�������Őݒ肵��key�Ő�������悤�Ɋ֐����Z�b�g
		* �������t���R���X�g���N�^�͎g���Ȃ��̂Œ��ӁI*/
		template <class T>
		inline static void AddGameObject(crstring name) {
			for (auto& data : mGameObjects) { if (data.first == name)return; }
			mGameObjects[name] = []() { return new T; };
		}

		template <class T>
		inline static void AddScene(crstring name) {
			for (auto& data : mScenes) { if (data.first == name)return; }
			mScenes[name] = []() { return new T; };
		}

		/*�������J��*/
		inline static void GameObjectClear() { mGameObjects.clear(); }
		inline static void SceneClear() { mScenes.clear(); }

	};

}
