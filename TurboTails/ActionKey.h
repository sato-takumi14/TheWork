#pragma once

#include"Main.h"

class ActionKey
{
private:

	typedef struct ActionInfo
	{
	public:

		function<cbool(const BYTE&)> func;
		BYTE key;
		string scene;

	public:

		ActionInfo(function<cbool(const BYTE&)> function, const BYTE& key, crstring scene) 
			: func(function), key(key), scene(scene) {}

	}ActionInfo;

	typedef struct ActionInfoGamePad {

	public:

		function<cbool(const DWORD&)> func;
		DWORD key;
		string scene;

	public:

		ActionInfoGamePad(function<cbool(const DWORD&)> func, const DWORD& key, crstring scene) 
			: func(func), key(key), scene(scene) {}

	}ActionInfoGamePad;

private:

	static unordered_map<string, list<ActionInfo>> mAction;
	static unordered_map<string, list<ActionInfoGamePad>> mGamePadAction;
	static class Scene* mScene;

public:

	static void Uninit();
	static void Uninit(crstring scene);

	static void SetScene(Scene* scene);

	static void AddAction(crstring name, function<cbool(const BYTE&)> func, const BYTE& action);
	static void AddGamePadAction(crstring name, function<cbool(const DWORD&)> func, const DWORD& key);

	static cbool Action(crstring name);

};