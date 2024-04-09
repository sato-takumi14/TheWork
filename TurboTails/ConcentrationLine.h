#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF12/05
*
********************************************************************************************
* ConcentrationLine
********************************************************************************************/

#include"GameObject.h"

class ConcentrationLine : public GameObject
{
private:

	DrawInfo mInfo{};

	ID3D11Buffer* mVertexBuffer{};

	uint mTexID = -1;

public:

	ConcentrationLine();

	void Init()override;
	void Start()override;
	void Update()override;
	void Uninit()override;
	
	const DrawInfo GetDrawInfo()const override { return mInfo; }

};