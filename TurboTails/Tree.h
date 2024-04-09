#pragma once

#include"GameObject.h"

class Tree : public GameObject
{
private:

	DrawInfo mInfo{};

	class MeshField* mMeshField{};

public:

	void Init()override;
	void Start()override;
	void Update()override;

	const DrawInfo GetDrawInfo()const override { return mInfo; }

};