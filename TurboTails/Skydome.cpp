
#include"main.h"

#include "Skydome.h"

#include"ModelManager.h"
#include"DrawModel.h"
#include"Scene.h"

void Skydome::Init()
{

	mModelID = ModelManager::LoadModel("asset\\model\\sky001.obj", "Sky");

	mLayer.push_back("sky");

	mTransform.pos.y = -5.0f;
	mTransform.scale *= 300.0f;

	mClass = "Skydome";

}

void Skydome::Start()
{

	mPlayer = mScene->GetGameObjectTag("Player");

	mDraw = new DrawModel(
		this,
		mModelID,
		"shader\\UnlitTextureVS.cso",
		"shader\\UnlitTexturePS.cso",
		list<int>(),
		Color::normal()
	);

}

void Skydome::Update()
{
	
}
