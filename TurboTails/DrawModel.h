#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* DrawModel�N���X�F3D���f����`�悷��N���X
********************************************************************************************/

#include"DrawInterface.h"

class DrawModel : public DrawInterface
{
private:

	int mModelID = -1;
	color mColor{};

public:

	DrawModel(
		GameObject* target,
		crint id,
		crstring vertex,
		crstring pixel,
		const list<int> tex,
		crcolor color);

	void Draw()override;

	void Set(crcolor set)override { mColor = set; }

};