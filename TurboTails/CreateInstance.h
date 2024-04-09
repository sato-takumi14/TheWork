#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：12/09
*
********************************************************************************************
* インスタンスを生成するクラス
********************************************************************************************/

#include"Main.h"

class GameObject;
class Scene;
class Component;

class CreateInstance
{
private:

	static unordered_map<string, function<GameObject* ()>>		mGameObject;	//ゲームオブジェクト
	static unordered_map<string, function<Scene* ()>>			mScene;			//シーン
	static unordered_map<string, function<Component* ()>>		mComponent;		//コンポーネント

public:

	static void Init();
	static void Uninit();

/*********************************************************************************************************/

	/*Addで追加したクラス名でインスタンスを生成する*/
	static GameObject* CreateObject(crstring name);
	static Scene* CreateScene(crstring name);
	static Component* CreateComponet(crstring name);

/*********************************************************************************************************/

	/*指定した型を引数で設定したkeyで生成するように関数をセット*/
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

	/*メモリ開放*/
	static void GameObjectClear();
	static void SceneClear();
	static void ComponentClear();

/*********************************************************************************************************/

};