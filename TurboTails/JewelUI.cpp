
#include"main.h"

#include "JewelUI.h"

#include"TextureManager.h"
#include"Draw2D.h"
#include"UIManager.h"

void JewelUI::Init()
{

	
	mTexID = TextureManager::Load("asset\\texture\\jewel.png", "JewelUI");

	float size = 2.0f;
	mTransform.pos = ver3(10.0f, 10.0f, 0.0f) * size;

	mInfo.transform = mTransform;
	mInfo.wh = ver3(13.0f, 10.0f, 0.0f) * size;
	mInfo.u = mInfo.v = Vector3::up();

	mLayer.push_back("2D");

	mTag = "JewelUI";
	mClass = "JewelUI";

}

void JewelUI::Start()
{

	list<int> tex{};
	tex.push_back(mTexID);

	mDraw = new Draw2D(
		this,
		"shader\\UnlitTextureVS.cso",
		"shader\\UnlitTexturePS.cso",
		Vector3::zero(),
		Vector4::one(),
		tex
	);

}

void JewelUI::Update()
{



}

void JewelUI::Uninit()
{
	GameObject::Uninit();
}
