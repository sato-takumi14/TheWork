#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* DrawBillBoardクラス：Billboard処理が入っている板ポリゴンを描画するクラス
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