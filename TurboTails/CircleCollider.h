#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* CircleColliderクラス：円の当たり判定をするためのコンポーネントを継承したクラス
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