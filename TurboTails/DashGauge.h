#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF01/05
*
********************************************************************************************
* DashGauge
********************************************************************************************/

#include"GameObject.h"

class DashGauge : public GameObject
{
private:

	class Camera* mMainCamera{};

	DrawInfo mInfo{};
	ver4 mGauge{};
	ver3 mOffset{};

	bool mReset{};
	int mTime{};

public:

	void Init()override;
	void Start()override;
	void Update()override;
	void Draw()override;
	void Uninit()override;

	const DrawInfo GetDrawInfo()const override { return mInfo; }
	cfloat GetFloat()const override { return mGauge.x; }

	void Set(crfloat set)override { mGauge.x = set; }
	void Set(crbool set)override { mReset = set; }

	void SetScreenPosition(crver3 data);

	void DownGauge(crfloat down) { mGauge.x -= down; }

};