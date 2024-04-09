#pragma once

#include"Main.h"

class GameObject;

class ObjectDebug
{
private:

	static list<GameObject*> mObject;

	static unordered_map<string, list<GameObject*>> mObjectDatas;

	static string mFileName;
	static class Scene* mScene;

public:

	static void Update();
	static void Uninit();

	static void Load(crstring file);
	static void SetObject(GameObject* obj);

};