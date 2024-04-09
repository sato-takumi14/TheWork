#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF12/23
*
********************************************************************************************
* Goal
********************************************************************************************/

#include"GameObject.h"

class Goal : public GameObject
{
private:

	enum class GoalState {
		none,
		goal,
		next,
	};

	GoalState mGoalState = GoalState::none;
	DrawInfo mInfo{};

	uint mModelID = -1;

public:

	void Init()override;
	void Start()override;
	void Update()override;
	void Draw()override;

	void OnCollisionEnter(crint id)override;
	void OnCollisionExit(crint id)override;
	void OnCollisionStay(crint id)override;

	cbool GetBool()const override { return mGoalState != GoalState::none; }

	const DrawInfo GetDrawInfo()const override { return mInfo; }

};