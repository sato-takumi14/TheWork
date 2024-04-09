#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* BoxColliderクラス：四角形の当たり判定をするためのコンポーネントを継承したクラス
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