#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* DrawAnimModel�N���X�F�A�j���[�V�������郂�f������`�悷��N���X
********************************************************************************************/

#include"DrawInterface.h"

class DrawAnimModel : public DrawInterface
{
private:

	int mModelID = -1;

	color mColor{};
	string mAnimName{};
	string mNextAnimName{};
	float mBlend{};
	float mAddBlend{};
	int mTime{};
	quat mQuaternion{};

	unordered_map<string, float> mAddBlends{};

public:

	DrawAnimModel(
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
	void Uninit()override;

	void Set(crfloat set)override { mAddBlend = set; }
	void Set(crstring set)override;

	void SetAnimName(crstring name)override;
	void SetAnimName(crstring name, crfloat blend)override;

	void SetRotation(crfloat set = 0.0f);

	cfloat GetFloat()const override { return mBlend; }

};