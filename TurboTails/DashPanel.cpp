
#include"Main.h"

#include "DashPanel.h"

#include"Scene.h"
#include"DrawModel.h"
#include"ModelManager.h"
#include"MeshField.h"

#include"CircleCollider.h"
#include"UIManager.h"

void DashPanel::Init()
{

	mTransform.scale *= 1.5f;

	mInfo.transform = mTransform;

	AddComponent(new CircleCollider(this, -mTransform.GetForward() * 2.0f, 0.1f));

	mLayer.push_back("3D");

}

void DashPanel::Start()
{

	mDraw = new DrawModel(
		this,
		ModelManager::LoadModel("asset\\model\\dashpanel.obj", "DashPanel"),
		"shader\\VertexLightingVS.cso",
		"shader\\VertexLightingPS.cso",
		list<int>(),
		Color::normal()
	);

	mTransform.pos.y = mScene->GetGameObject<MeshField>("MeshField")->GetHeight(mTransform.pos) + 0.01f;
}

void DashPanel::Update()
{

	GameObject::Update();

}

void DashPanel::OnCollisionEnter(crint id)
{

	auto obj = mScene->GetGameObject(id);

	if (obj->GetTag() == "Player") {

		obj->Set(0.5f);
		dynamic_cast<UIManager*>(mScene->GetManagerTag("UIManager"))->GetUI("line")->SetActivate(true);

	}

}

void DashPanel::OnCollisionStay(crint id)
{

}

void DashPanel::OnCollisionExit(crint id)
{

}
