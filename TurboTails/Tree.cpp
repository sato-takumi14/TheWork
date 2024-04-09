
#include"main.h"

#include "Tree.h"
#include"Scene.h"
#include"DrawBillboard.h"
#include"TextureManager.h"
#include"MeshField.h"

void Tree::Init()
{

	list<int> tex{};

	tex.push_back(TextureManager::Load("asset\\texture\\tree000.png", "tree"));

	mDraw = new DrawBillBoard(
		this,
		"shader\\UnlitTextureVS.cso",
		"shader\\UnlitTexturePS.cso",
		tex,
		Vector4::one()
	);

	mLayer.push_back("3D");

	mTransform.pos.x = -5.0f;
	mTransform.pos.y = 2.0f;

	mInfo.transform = mTransform;
	mInfo.wh = ver3(3.0f, 3.0f, 0.0f);
	mInfo.u = mInfo.v = Vector3::up();

}

void Tree::Start()
{
	mMeshField = mScene->GetGameObject<MeshField>("MeshField");
}

void Tree::Update()
{

	mTransform.pos.y = mMeshField->GetHeight(mTransform.pos) + 1.0f;

}
