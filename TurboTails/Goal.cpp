
#include"Main.h"

#include "Goal.h"

#include"CircleCollider.h"
#include"Scene.h"
#include"ModelManager.h"

#include"MeshField.h"
#include"Player.h"

#include"DrawModel.h"
#include"Load.h"
#include"UIManager.h"
#include"MouseUI.h"
#include"SelectButton.h"

void Goal::Init()
{

	AddComponent(new CircleCollider(this, Vector3::zero(), 1.0f));
	
	mTransform.scale *= 0.1f;

	mLayer.push_back("3D");
	mTag = "Goal";

}

void Goal::Start()
{
	mModelID = ModelManager::LoadModel("asset\\model\\Gate.obj", "Gate");
	MeshField* field = dynamic_cast<MeshField*>(mScene->GetGameObjectTag("MeshField"));
	float height = field->GetHeight(mTransform.pos) + 1.0f;

	if (height > mTransform.pos.y)mTransform.pos.y = height;

	mDraw = new DrawModel(
		this,
		mModelID,
		"shader\\MaterialColorVS.cso",
		"shader\\MaterialColorPS.cso",
		list<int>(),
		Color::normal()
	);
}

void Goal::Update()
{

	GameObject::Update();

}

void Goal::Draw()
{
	MATERIAL param{};
	ZeroMemory(&param, sizeof(MATERIAL));
	param.Diffuse = Color::black();
	Renderer::SetMaterial(param);
	GameObject::Draw();
}

void Goal::OnCollisionEnter(crint id)
{

	auto obj = mScene->GetGameObject(id);

	if (obj->GetTag() == "Player") {

	}

}

void Goal::OnCollisionExit(crint id)
{

	auto obj = mScene->GetGameObject(id);
	auto tag = obj->GetTag();

	if (tag == "Player") {

		if (dynamic_cast<Player*>(obj)->GetKey()) {

			dynamic_cast<MouseUI*>(dynamic_cast<UIManager*>(mScene->GetManagerTag("UIManager"))->GetUI("MouseUI"))->Set(MouseUI::SelectMouseUI::left);
			dynamic_cast<SelectButton*>(dynamic_cast<UIManager*>(mScene->GetManagerTag("UIManager"))->GetUI("SelectButton"))->Set(true);

		}
		
	}

}

void Goal::OnCollisionStay(crint id)
{

	auto obj = mScene->GetGameObject(id);

	if (obj->GetTag() == "Player") {
		
		if (dynamic_cast<Player*>(obj)->GetKey()) {

			auto ui = dynamic_cast<MouseUI*>(dynamic_cast<UIManager*>(mScene->GetManagerTag("UIManager"))->GetUI("MouseUI"));
			auto mouse = ui->GetMouseUI(MouseUI::SelectMouseUI::left);
			mouse->SetActivate(true);

			auto button = dynamic_cast<UIManager*>(mScene->GetManagerTag("UIManager"))->GetUI("SelectButton");
			button->SetActivate(true);

			dynamic_cast<Load*>(mScene->GetGameObjectTag("Load"))->Set("StageSelect");

		}

	}

}
