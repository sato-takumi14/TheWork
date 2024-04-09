#pragma once

#include"Manager.h"

class StageManager : public Manager
{
private:

	unordered_map<string, list<Transform>> mStageObjectData{};
	string mStatgeName{};
	class CRIAudio* mAudio{};

public:

	StageManager(crstring name) : mStatgeName(name) {}
	void Init()override;
	void Start()override;
	void Update()override;
	void Uninit()override;

	const list<Transform> GetTransforminfos(crstring name)const;

};