#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F02/16
*
********************************************************************************************
* Bloom�N���X�F�u���[���������s�����߂̃N���X
********************************************************************************************/

#include"GameObject.h"

class Bloom : public GameObject
{
private:

	ID3D11Buffer* mVertexBuffer{};

public:

	void Init()override;
	void Start()override;
	void Update()override;
	void Draw()override;
	void Uninit()override;

};