#pragma once

#include"GameObject.h"

class SceneChangeObject : public GameObject
{
private:

	class Scene* mNextScene{};
	int mBottonInt{};
	char mBottonKey{};
	bool mKey{};
	bool mUpdate{};
	string mScene{};
	string mActionKey{};
	function<void()> mNextSceneFunc{};
	function<cbool()> mConditionsFunc{};

	DrawInfo mInfo{};

private:

	void SetNextScene();
	void SetNextLoadingScene();
	void Next();

public:

	SceneChangeObject(
		crstring next,
		crint botton,
		crbool update,
		crbool thread)
		: mScene(next),
		mBottonInt(botton),
		mUpdate(update) {
		if (thread)mNextSceneFunc = [this]() { SetNextLoadingScene(); };
		else mNextSceneFunc = [this]() { SetNextScene(); };
	}

	SceneChangeObject(
		crstring next,
		const char& botton,
		crbool update,
		crbool thread)
		: mScene(next),
		mBottonKey(botton),
		mKey(true),
		mUpdate(update) {
		if (thread)mNextSceneFunc = [this]() { SetNextLoadingScene(); };
		else mNextSceneFunc = [this]() { SetNextScene(); };
	}

	SceneChangeObject(
		crstring next,
		crstring action,
		const function<cbool()> func,
		crbool thread
	) : mScene(next),
		mActionKey(action) 
	{
		if (thread)mNextSceneFunc = [this]() { SetNextLoadingScene(); };
		else mNextSceneFunc = [this]() { SetNextScene(); };
		mConditionsFunc = func;
	}

	void Update()override;

	void SetDrawInfo(const DrawInfo& info) { mInfo = info; }
	void Set(crbool set)override { mUpdate = set; }

};