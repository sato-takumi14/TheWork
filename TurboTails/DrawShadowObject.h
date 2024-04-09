#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* DrawShadowObjectクラス：3Dモデルと影をを描画するクラス
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