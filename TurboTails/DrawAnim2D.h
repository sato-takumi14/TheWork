#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F11/7
*
********************************************************************************************
* Draw2D�N���X�F2D��`�悷��N���X
********************************************************************************************/

#include"DrawInterface.h"
#include"Vector.h"

class DrawAnim2D : public DrawInterface
{
private:

	ID3D11Buffer* mVertexBuffer{};
	ver3 mNormal{};
	ver4 mColor = Vector4::one();

	typedef struct {
		ver3 u;
		ver3 v;
	}UV;

	UV mInfo{};
	float mTime{};
	float mAddTime{};

	void Add();

public:

	DrawAnim2D(
		GameObject* target,
		crstring vertex,
		crstring pixel,
		crver3 normal,
		crver4 color,
		const list<int> tex,
		crbool set = false
	);
	void Draw()override;
	void Uninit()override;

};