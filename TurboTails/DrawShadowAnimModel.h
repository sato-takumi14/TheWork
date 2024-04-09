#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* DrawShadowAnimModel�N���X�F3D���f���Ɖe��`�悷��N���X
********************************************************************************************/

#include"DrawInterface.h"

class DrawShadowAnimModel : public DrawInterface
{
private:

	int mModelID = -1;
	color mColor{};
	string mAnimName{};
	string mNextAnimName{};
	int mTime{};
	float mBlend{};
	float mAddBlend{};

public:

	DrawShadowAnimModel(
		GameObject* target,
		crint id,
		crstring vertex,
		crstring pixel,
		const list<int> tex,
		crcolor color,
		crfloat addblend,
		crstring anim
	);

	void Draw()override;

	void Set(crfloat set)override { mAddBlend = set; }
	void Set(crstring set)override;

};