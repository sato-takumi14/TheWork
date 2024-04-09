#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF12/05
*
********************************************************************************************
* TargetObject
********************************************************************************************/

#include"GameObject.h"

class TargetObject : public GameObject
{
private:

	enum class TargetObjectState {
		none,
		lockon,
		lock,
	};

	class Camera* mCamera{};
	class Player* mPlayer{};

	GameObject* mTarget{};
	DrawInfo mInfo{};

	TargetObjectState mStatus{};
	uint mTexID = -1;

public:

	void Init()override;
	void Start()override;
	void Update()override;
	void Draw()override;

	void SetGameObject(GameObject* set)override { mTarget = set; }
	
	const DrawInfo GetDrawInfo()const override { return mInfo; }

};