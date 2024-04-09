#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* DrawShadowModel�N���X�F3D���f���Ɖe����`�悷��N���X
********************************************************************************************/

#include"DrawInterface.h"

class DrawShadowModel : public DrawInterface
{
private:

	int mModelID = -1;
	color mColor{};

public:

	DrawShadowModel(
		GameObject* target,
		crint id,
		crstring vertex,
		crstring pixel,
		const list<int> tex,
		crcolor color);

	void Draw()override;

	void Set(crcolor set)override { mColor = set; }

};