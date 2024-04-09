#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF11/25
*
********************************************************************************************
* MoveFloor
********************************************************************************************/

#include"GameObject.h"

class MoveFloor : public GameObject
{
private:

	enum class MoveFloorState {
		none,
		front,
		back,
		stop,
	};

	MoveFloorState mFloorState = MoveFloorState::none;
	MoveFloorState mOldFloorState = MoveFloorState::none;

	class Time* mTime{};

	ver3 mMove{};
	
	double mMoveSpeed{};
	double mStopTime{};

	bool mStop{};

	uint mModelID = -1;

private:

	void Front();
	void Back();
	void Stop();

public:

	MoveFloor() {}
	MoveFloor(crdouble movetime, crdouble stop, crver3 move);

	void Init()override;
	void Start()override;
	void Update()override;

	void OnCollisionEnter(crint id)override;
	void OnCollisionExit(crint id)override;
	void OnCollisionStay(crint id)override;

	void Set(crbool set)override { mStop = set; }

};