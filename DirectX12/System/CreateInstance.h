#pragma once

/***********************************************************************************************************
*
*	CreateInstance
*
*																				制作者：佐藤匠
*																				制作日：02/29
*
************************************************************************************************************
* 指定したkeyでインスタンスを生成するクラス
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

		static std::unordered_map<string, std::function<GameObject::Interface* ()>> mGameObjects;	/*ゲームオブジェクト*/
		static std::unordered_map<string, std::function<Scene* ()>> mScenes;						/*シーン*/

	public:

		/*通常処理*/
		static void MasterInit();
		static void MasterUninit();

		/*Addで追加した各インスタンスを生成する*/
		inline static GameObject::Interface* CreateObject(crstring name) { return mGameObjects[name](); }
		inline static Scene* CreateScene(crstring name) { return mScenes[name](); }

		/*指定した型を引数で設定したkeyで生成するように関数をセット
		* ※引数付きコンストラクタは使えないので注意！*/
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

		/*メモリ開放*/
		inline static void GameObjectClear() { mGameObjects.clear(); }
		inline static void SceneClear() { mScenes.clear(); }

	};

}
