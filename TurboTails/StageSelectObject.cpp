
#include"Main.h"
#include "StageSelectObject.h"

#include"DrawModel.h"
#include"Draw2D.h"

#include"ModelManager.h"
#include"MeshField.h"
#include"Scene.h"
#include"CircleCollider.h"
#include"File.h"
#include"Player.h"
#include"TextureManager.h"
#include"ActionKey.h"
#include"UIManager.h"
#include"MouseUI.h"
#include"Load.h"
#include"SelectButton.h"

unordered_map<int, StageSelectObject::SelectInfo> StageSelectObject::mLoadSceneData{};
int StageSelectObject::mSelectIndex = 0;

void StageSelectObject::SceneDataLoad()
{
	if (static_cast<int>(mLoadSceneData.size()) != 0) return;
	mLoadSceneData = File::JsonLoad<unordered_map<int, SelectInfo>>("asset\\stage\\stageselect.json");
}

void StageSelectObject::Init()
{

	AddComponent(new CircleCollider(this, Vector3::zero(), 1.0f));

	mTransform.scale *= 0.1f;

	mLayer.push_back("3D");

	StageSelectObject::SceneDataLoad();
	mInfo = mLoadSceneData[mSelectIndex++];

}

void StageSelectObject::Start()
{
	mModelID = ModelManager::LoadModel("asset\\model\\Gate.obj", "StageSelectObject");
	MeshField* field = dynamic_cast<MeshField*>(mScene->GetGameObjectTag("MeshField"));
	mTransform.pos.y = field->GetHeight(mTransform.pos) + 1.0f;
	mDraw = new DrawModel(
		this,
		mModelID,
		"shader\\MaterialColorVS.cso",
		"shader\\MaterialColorPS.cso",
		list<int>(),
		mInfo.color
	);
}

void StageSelectObject::Update()
{

	GameObject::Update();

}

void StageSelectObject::Draw()
{
	MATERIAL param{};
	ZeroMemory(&param, sizeof(MATERIAL));
	param.Diffuse = mInfo.color;
	Renderer::SetMaterial(param);
	GameObject::Draw();
}

void StageSelectObject::Uninit()
{

	GameObject::Uninit();
	mSelectIndex = 0;
	mLoadSceneData.clear();

}

void StageSelectObject::OnCollisionEnter(crint id)
{
}

void StageSelectObject::OnCollisionExit(crint id)
{

	auto obj = mScene->GetGameObject(id);
	auto tag = obj->GetTag();

	if (tag == "Player") {

		dynamic_cast<MouseUI*>(dynamic_cast<UIManager*>(mScene->GetManagerTag("UIManager"))->GetUI("MouseUI"))->Set(MouseUI::SelectMouseUI::left);
		dynamic_cast<SelectButton*>(dynamic_cast<UIManager*>(mScene->GetManagerTag("UIManager"))->GetUI("SelectButton"))->Set(true);

	}

}

void StageSelectObject::OnCollisionStay(crint id)
{

	auto obj = mScene->GetGameObject(id);

	if (obj->GetTag() == "Player") {

		auto ui = dynamic_cast<MouseUI*>(dynamic_cast<UIManager*>(mScene->GetManagerTag("UIManager"))->GetUI("MouseUI"));
		auto mouse = ui->GetMouseUI(MouseUI::SelectMouseUI::left);
		mouse->SetActivate(true);

		auto button = dynamic_cast<SelectButton*>(dynamic_cast<UIManager*>(mScene->GetManagerTag("UIManager"))->GetUI("SelectButton"));
		button->SetActivate(true);

		if (ActionKey::Action("select")) {

			dynamic_cast<Load*>(mScene->GetGameObjectTag("Load"))->Set(mInfo.scene);

		}

	}

}

void StageSelectObject::SelectUI::Init()
{
	mInfo.wh = Vector3::windowcenter();
	mTransform.pos = Vector3::windowcenter();
}

void StageSelectObject::SelectUI::Start()
{
	list<int> tex{};
	tex.push_back(mTexID);
	mDraw = new Draw2D(
		this,
		"shader\\unlitTextureVS.cso",
		"shader\\unlitTexturePS.cso",
		Vector3::zero(),
		Vector4::one(),
		tex
	);
}

void StageSelectObject::SelectUI::Update()
{

}
