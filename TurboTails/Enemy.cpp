
#include"Main.h"

#include "Enemy.h"
#include"TargetManager.h"

#include"DrawModel.h"
#include"ModelManager.h"
#include"Scene.h"
#include"MeshField.h"
#include"CircleCollider.h"

void Enemy::Init()
{

	list<int> tex{};

	mDraw = new DrawModel(
		this,
		ModelManager::LoadModel("asset\\model\\slim.obj", "Enemy"),
		"shader\\VertexLightingVS.cso",
		"shader\\VertexLightingPS.cso",
		tex,
		Color::normal()
	);

	dynamic_cast<TargetManager*>(mScene->GetManagerTag("TargetManager"))->AddTarget(this);

	mLayer.push_back("3D");

	mTag = "Enemy";

	AddComponent(new CircleCollider(this, Vector3::zero(), 3.0f));

	mTransform.rot.y = PI;

}

void Enemy::Start()
{

	mTransform.pos.y = mScene->GetGameObject<MeshField>("MeshField")->GetHeight(mTransform.pos);

}

void Enemy::Update()
{

	GameObject::Update();

}

void Enemy::OnCollisionEnter(crint id)
{
}

void Enemy::OnCollisionExit(crint id)
{
}

void Enemy::OnCollisionStay(crint id)
{
}
