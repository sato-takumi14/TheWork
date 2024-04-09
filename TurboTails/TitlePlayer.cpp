
#include "main.h"
#include "TitlePlayer.h"

#include"ModelManager.h"
#include"DrawAnimModel.h"
#include"Scene.h"
#include"MeshField.h"
#include"Time.h"

void TitlePlayer::Init()
{

	mTag = "Player";
	mLayer.push_back("3D");

	mTransform.scale *= 0.010f;
	mTransform.rot = Vector3::up() * PI;

}

void TitlePlayer::Start()
{
	mField = dynamic_cast<MeshField*>(mScene->GetGameObjectTag("MeshField"));
}

void TitlePlayer::Update()
{

	ver3 pos = mTransform.pos;
	pos.y = 0.0f;
	if (D3DXVec3Length(&pos) >= 295.0f) {
		mTransform.pos.z = 295.0f;
	}

	mTransform.pos += mTransform.GetForward() * 0.1f;
	mTransform.pos.y = mField->GetHeight(mTransform.pos);

}
