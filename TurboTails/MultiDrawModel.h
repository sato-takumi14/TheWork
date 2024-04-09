#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* MultiDrawObject�N���X�F������3D���f����`�悷��N���X
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