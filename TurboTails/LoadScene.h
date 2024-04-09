#pragma once

#include"Scene.h"

#include<mutex>
#include<thread>

class LoadScene : public Scene
{
private:

	string mNextScene{};
	Scene* mLoadScene{};
	bool mLoad{};

	int mTime{};

	mutex mMutex{};

private:

	void Load();

public:

	LoadScene() {}
	LoadScene(crstring next) : mNextScene(next) {}

	void Init()override;
	void Update()override;
	void Uninit()override;

	cbool GetLoad()const override { return mLoad && mTime >= 20; }

};