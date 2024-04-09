
#include"main.h"

#include "ConcentrationLine.h"

#include"Draw2D.h"
#include"TextureManager.h"
#include"Shader.h"
#include"UIManager.h"

ConcentrationLine::ConcentrationLine()
{
	mTexID = TextureManager::Load("asset\\texture\\blue_001.png", "ConcentrationLine");	
}

void ConcentrationLine::Init()
{

	mTransform.pos = Vector3::windowcenter();

	mInfo.wh = Vector3::windowsize() * 0.5f;
	mInfo.u = mInfo.v = Vector3::up();
	mInfo.transform = mTransform;

	mLayer.push_back("2D");

	mActivate = false;

	mClass = "ConcentrationLine";

}

void ConcentrationLine::Start()
{

	list<int> tex{};
	tex.push_back(mTexID);
	mDraw = new Draw2D(
		this,
		"shader\\UnlitTextureVS.cso",
		"shader\\UnlitTexturePS.cso",
		Vector3::back(),
		Vector4::one(),
		tex
	);

}

void ConcentrationLine::Update()
{



}

void ConcentrationLine::Uninit()
{
	GameObject::Uninit();
}
