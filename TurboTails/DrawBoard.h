#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* DrawBoardクラス：3Dの板ポリゴンを描画するクラス
********************************************************************************************/

#include"DrawInterface.h"
#include"Vector.h"

class DrawBoard : public DrawInterface
{
private:

	ID3D11Buffer* mVertexBuffer{};
	ver3 mNormal{};
	ver4 mColor = Vector4::one();

public:

	DrawBoard(
		GameObject* target,
		crstring vertex,
		crstring pixel,
		crver3 normal,
		crver4 color,
		const list<int> tex,
		crbool set = false);

	void Draw()override;
	void Uninit()override;

	void Set(crver3 set)override { mNormal = set; }
	void Set(crver4 set)override { mColor = set; }

};