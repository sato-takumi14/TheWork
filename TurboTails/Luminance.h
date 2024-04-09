#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F02/16
*
********************************************************************************************
* Luminance�N���X�FBloom�̏������s�����߂̃N���X
********************************************************************************************/

#include"GameObject.h"

class Luminance : public GameObject
{
private:

	ID3D11Buffer* mVertexBuffer{};
	int mIndex{};

public:

	void Init()override;
	void Start()override;
	void Update()override;
	void Draw()override;
	void Uninit()override;

};