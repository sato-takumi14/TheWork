
#include"main.h"

#include "LoadObject.h"

#include"Draw2D.h"
#include"TextureManager.h"
#include"Time.h"

void LoadObject::Init()
{

	list<int> tex{};
	tex.push_back(TextureManager::Load(
		"asset\\texture\\Load.png",
		"LoadObject"
	));

	mDraw = new Draw2D(
		this,
		"shader\\UnlitTextureVS.cso",
		"shader\\UnlitTexturePS.cso",
		Vector3::back(),
		Vector4::one(),
		tex
	);

	mInfo.wh = ver3(400.0f, 300.0f, 0.0f) * 0.20f;
	mInfo.u = DrawInfo::CreateUV(1.0f / 4);
	mInfo.v = Vector3::up();

	mTransform.pos = Vector3::windowsize() - mInfo.wh;

	mLayer.push_back("2D");

}

void LoadObject::Update()
{

	GameObject::Update();

	if (mTime++ >= 10) {
		mInfo.AddU();
		mTime = 0;
	}

}
