#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F12/20
*
********************************************************************************************
* Balloon�N���X
********************************************************************************************/

#include"GameObject.h"

class Balloon : public GameObject
{
private:

	uint mModelID = -1;
	class MultiGameObject* mMulti{};
	int mTime{};
	int mRespawnTime{};

public:

	Balloon() {}
	Balloon(crint time) : mRespawnTime(time) { mActivate = false; }
	Balloon(MultiGameObject* multi) : mMulti(multi) {}
	void Init()override;
	void Start()override;
	void Update()override;

	cuint GetModelID()const override { return mModelID; }
	cstring GetShaderName()const override { return "multi"; }

	void OnCollisionEnter(crint id)override;

	void SetMultiGameObject(MultiGameObject* multi)override { mMulti = multi; }

};