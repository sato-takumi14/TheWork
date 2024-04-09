#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* CircleCollider�N���X�F�~�̓����蔻������邽�߂̃R���|�[�l���g���p�������N���X
********************************************************************************************/

#include"Collider.h"
#include"Collision.h"
#include"GameObject.h"
#include"Scene.h"

class CircleCollider : public Collider
{
public:

	CircleCollider(
		GameObject* target,
		crver3 offset,
		crfloat radius,
		crint id = 0
	) {
		mTarget = target;
		mOffset = offset;
		mRadius = radius;
		mID = id;
		mTarget->GetScene()->GetCollision()->SetCircleCollider(this);
	}

};