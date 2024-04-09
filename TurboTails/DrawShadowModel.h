#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* DrawShadowModelクラス：3Dモデルと影をを描画するクラス
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