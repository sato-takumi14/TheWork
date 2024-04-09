#pragma once

#include"Manager.h"

class GameObject;

typedef struct Info {

	int id;
	ver3 pos;
	bool flag;

}TargetInfo;

class TargetManager : public Manager
{
private:

	typedef struct TargetInfo {

		GameObject* target;
		string scene;

		TargetInfo() {
			this->target = nullptr;
			this->scene = "";
		}

		TargetInfo(GameObject* target, crstring scene) {
			this->target = target;
			this->scene = scene;
		}

	}TargetInfoData;

private:

	list<TargetInfoData> mTargets;
	class Scene* mNowScene;

public:

	void Init()override;
	void Update()override;
	void Uninit()override;

	void AddTarget(GameObject* target);
	GameObject* GetTarget(crver3 pos);

};