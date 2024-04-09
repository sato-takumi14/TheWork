#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* DrawModelクラス：3Dモデルを描画するクラス
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