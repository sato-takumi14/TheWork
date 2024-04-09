#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* DrawShadowObject�N���X�F3D���f���Ɖe����`�悷��N���X
********************************************************************************************/

#include"DrawInterface.h"

class DrawShadowObject : public DrawInterface
{
private:

	int mModelID = -1;
	int mTexID = -1;
	color mColor{};

public:

	DrawShadowObject(
		GameObject* target,
		crint model,
		crstring vertex,
		crstring pixel,
		crint tex,
		crcolor color
	);

	void Draw()override;

};