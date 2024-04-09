#include "main.h"
#include "MouseUI.h"

#include"Draw2D.h"
#include"TextureManager.h"
#include"Scene.h"
#include"Camera.h"
#include"UIManager.h"

/*マウスUI*/
MouseUI::MouseObject::MouseObject(crstring file, crstring tag)
{
	mTexID = TextureManager::Load(file, tag);
}

void MouseUI::MouseObject::Init()
{
	mInfo.wh = ver3(383.0f, 578.0f, 0.0f) * 0.1f;
	SetActivate(false);
	mLayer.push_back("2D");
	mTransform.pos = Vector3::windowcenter() + ((Vector3::down() * 100.0f) + (Vector3::right() * 100.0f));
}

void MouseUI::MouseObject::Start()
{

	list<int> tex{};
	tex.push_back(mTexID);
	mColor = Vector4::create(Vector3::one(), 0.0f);
	mDraw = new Draw2D(
		this,
		"shader\\UnlitTextureVS.cso",
		"shader\\UnlitTexturePS.cso",
		Vector3::zero(),
		mColor,
		tex
	);

}

void MouseUI::MouseObject::Update()
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

/*管理マウスUI*/
void MouseUI::Init()
{
	mMouse[SelectMouseUI::left] = mScene->AddGameObject(new MouseObject("asset\\texture\\マウス左クリック.png", "LeftClick"));
	mMouse[SelectMouseUI::right] = mScene->AddGameObject(new MouseObject("asset\\texture\\マウス右クリック.png", "RightClick"));
}

void MouseUI::Start()
{

}

void MouseUI::Update()
{

}

void MouseUI::Uninit()
{
	GameObject::Uninit();
}

void MouseUI::Set(const SelectMouseUI& ui)
{

	dynamic_cast<MouseObject*>(mMouse[ui])->Set(true);

}
