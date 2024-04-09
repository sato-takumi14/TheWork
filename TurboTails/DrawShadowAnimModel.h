#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* DrawShadowAnimModelクラス：3Dモデルと影を描画するクラス
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