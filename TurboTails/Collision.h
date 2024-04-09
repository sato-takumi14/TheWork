#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* Collisionクラス：当たり判定をするクラス
********************************************************************************************/

#include<list>

using namespace std;

class Collider;

class Collision
{
private:

	list<Collider*> mCircle;
	list<Collider*> mBox;
	list<Collider*> mCircle2D;

	void OnCollisionEnter();
	void OnCollisionExit();

	bool HitCircle(Collider* hit, Collider* washit);
	bool HitBox(Collider* hit, Collider* washit);

	const bool HitCircle2D(Collider* hit, Collider* washit);

public:

	void Update();
	void Uninit();

	void SetCircleCollider(Collider* set);
	void SetBoxCollider(Collider* set);
	void SetCircleCollider2D(Collider* set);

	void Erase(crint id);

};