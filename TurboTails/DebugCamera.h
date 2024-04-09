#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：11/05
*
********************************************************************************************
* DebugCamera：デバッグ時使用するカメラ
********************************************************************************************/

#include"GameObject.h"

class DebugCamera : public GameObject
{
private:

	matrix mView{};
	matrix mProjection{};
	ver3 mTargetPos{};
	GameObject* mTarget{};
	bool mFlag{};

public:

	void Init()override;
	void Start()override;
	void Update()override;
	void Draw()override;
	void Uninit()override;

	void Set(crver3 set)override;
	void Set(crbool set)override;

};