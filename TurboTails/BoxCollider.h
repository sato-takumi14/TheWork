#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* BoxCollider�N���X�F�l�p�`�̓����蔻������邽�߂̃R���|�[�l���g���p�������N���X
********************************************************************************************/

#include"Collider.h"
#include"Collision.h"
#include"GameObject.h"
#include"Scene.h"

class BoxCollider : public Collider
{
public:

	BoxCollider(
		GameObject* target,
		crver3 offset,
		crfloat radius
	) {
		mTarget = target;
		mOffset = offset;
		mRadius = radius;
		mTarget->GetScene()->GetCollision()->SetBoxCollider(this);
	}

};