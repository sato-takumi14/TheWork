
#include "main.h"
#include "TitleLogo.h"

#include"Draw2D.h"
#include"TextureManager.h"
#include"ActionKey.h"

void TitleLogo::Init()
{

	mTexID = TextureManager::Load("asset\\texture\\TurboTails.png", "TitleLogo");
	
	mColor = Vector4::one();

	mLayer.push_back("2D");

	mTransform.pos = Vector3::windowcenter();
	mInfo.transform = mTransform;
	mInfo.wh = ver3(300.0f, 100.0f, 0.0f);

	mSubtraction = 0.080f;
	mStart = false;

}

void TitleLogo::Start()
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

void TitleLogo::Update()
{

	if (ActionKey::Action("NextScene"))mStart = true;

	if (mStart) {

		mColor.w -= mSubtraction;

		dynamic_cast<Draw2D*>(mDraw)->Set(mColor);

		if (mColor.w <= 0.0f)mSubtraction *= (-1.0f);
		else if (mColor.w >= 1.0f)mSubtraction *= (-1.0f);

		mTime++;

	}
	

}
