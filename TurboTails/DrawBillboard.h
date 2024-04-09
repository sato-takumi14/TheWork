#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* DrawBillBoard�N���X�FBillboard�����������Ă���|���S����`�悷��N���X
********************************************************************************************/

#include"DrawInterface.h"

class DrawBillBoard : public DrawInterface
{
private:

	ID3D11Buffer* mVertexBuffer{};

	GameObject* mCamera{};
	ver4 mColor{};

public:

	DrawBillBoard(
		GameObject* target,
		crstring vertex,
		crstring pixel,
		const list<int> tex,
		crver4 color,
		crbool set = false
	);

	void Draw()override;
	void Uninit()override;

	void Set(crver4 set) { mColor = set; }

};