#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* MultiDrawObjectクラス：複数の3Dモデルを描画するクラス
********************************************************************************************/

#include"DrawInterface.h"

class MultiDrawModel : public DrawInterface
{
private:

	class Geometry* mGeometry{};
	list<Transform> mTransform{};
	uint mNumModel = 0;
	uint mModelID = -1;
	string mShaderName{};

public:

	MultiDrawModel(
		GameObject* obj, 
		cruint model, 
		crstring shadername,
		const list<int>& tex);

	MultiDrawModel(
		GameObject* obj,
		cruint model,
		crstring vertex,
		crstring pixel,
		const list<int>& tex
	);

	void Draw()override;
	void Uninit()override;

	void Set(cruint set)override { mNumModel = set; }

};