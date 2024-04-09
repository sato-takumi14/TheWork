
#include"main.h"

#include "Score.h"

#include"Draw2D.h"
#include"TextureManager.h"
#include"UIManager.h"

void Score::Init()
{

	mTexID = TextureManager::Load("asset\\texture\\êîéö.png", "Score");
	
	DrawInfo info{};
	ver3 pos = ver3(260.0f, 40.0f, 0.0f) * 0.5f;

	for (int i = 0; i < 4; i++) {

		info.transform = Transform(pos, Vector3::zero(), Vector3::zero());
		pos.x -= 25.0f;

		info.u = info.CreateUV(0.1f);
		info.v = Vector3::up();
		info.wh = info.CreateWH(25.0f);

		mInfo.push_back(info);

	}

	mLayer.push_back("2D");

	mTag = "Score";
	mClass = "Score";

}

void Score::Start()
{

	list<int> tex{};
	tex.push_back(mTexID);

	mDraw = new Draw2D(
		this,
		"shader\\UnlitTextureVS.cso",
		"shader\\UnlitTexturePS.cso",
		Vector3::zero(),
		Vector4::one(),
		tex,
		true
	);

}

void Score::Update()
{

	int score = mScore;

	for (auto& data : mInfo) {
		
		int num = score % 10;

		data.u.x = data.u.z * num;
		data.u.y = data.u.z * num + data.u.z;

		score /= 10;

	}

}

void Score::Uninit()
{
	GameObject::Uninit();
}
