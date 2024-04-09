
#include "main.h"
#include "SelectButton.h"

#include"TextureManager.h"
#include"Draw2D.h"

void SelectButton::Init()
{

	mTexID = TextureManager::Load("asset\\texture\\ƒ{ƒ^ƒ“.png", "SelectButton");
	mColor = Vector4::create(Vector3::one(), 0.0f);
	GameObject::SetActivate(false);

	mInfo.wh = DrawInfo::CreateWH(50.0f);
	mTransform.pos = Vector3::windowcenter() + ((Vector3::down() * 100.0f) + (Vector3::left() * 100.0f));

	mTag = "SelectButton";
	mLayer.push_back("2D");
	mFlag = false;

}

void SelectButton::Start()
{
	list<int> tex{};
	tex.push_back(mTexID);
	mDraw = new Draw2D(
		this,
		"shader\\UnlitTextureVS.cso",
		"shader\\UnlitTexturePS.cso",
		Vector3::zero(),
		mColor,
		tex
	);
}

void SelectButton::Update()
{

	if (mActivate && not mFlag) {
		mColor.w += 0.05f;
		if (mColor.w >= 1.0f)mColor.w = 1.0f;
		dynamic_cast<Draw2D*>(mDraw)->Set(mColor);
	}

	if (mFlag) {
		mColor.w -= 0.05f;
		if (mColor.w < 0.0f) {
			mColor.w = 0.0f;
			mFlag = false;
			mActivate = false;
		}
		dynamic_cast<Draw2D*>(mDraw)->Set(mColor);
	}


}

void SelectButton::Draw()
{
	GameObject::Draw();
}
