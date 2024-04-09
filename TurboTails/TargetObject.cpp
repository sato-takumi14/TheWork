
#include"main.h"

#include <DirectXMath.h>

#include "TargetObject.h"

#include"TextureManager.h"
#include"Draw2D.h"
#include"TargetManager.h"
#include"Scene.h"
#include"Camera.h"
#include"ActionKey.h"
#include"Player.h"

void TargetObject::Init()
{

	mTexID = TextureManager::Load(
		"asset\\texture\\target.png",
		"Target"
	);

	mTransform.pos = Vector3::windowcenter();

	mInfo.transform = mTransform;

	mInfo.u = mInfo.v = Vector3::up();

	mInfo.wh = ver3(1.0f, 1.0f, 0.0f) * 50.0f;

	mLayer.push_back("2D");

	mClass = "TargetObject";

}

void TargetObject::Start()
{

	mPlayer = dynamic_cast<Player*>(mScene->GetGameObjectTag("Player"));
	mCamera = mScene->GetGameObject<Camera>("MainCamera");

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

void TargetObject::Update()
{

	if (ActionKey::Action("charge") &&
		not mPlayer->GetGoal())mActivate = true;
	else mActivate = false;

	if (!mActivate)return;

	mTarget = dynamic_cast<TargetManager*>(mScene->GetManagerTag("TargetManager"))->GetTarget(mPlayer->GetPosition());

	if (!mTarget) {
		mActivate = false;
		return;
	}

	matrix world{}, view{}, projection{};

	ver3 target = mPlayer->GetPosition() + Vector3::up() * 2.0f;
	ver3 cameraPos = mCamera->GetPosition();

	ver3 up = ver3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&view, &cameraPos, &target, &up);

	D3DXMatrixPerspectiveFovLH(&projection, 1.0f,
		static_cast<float>(SCREEN_WIDTH / SCREEN_HEIGHT), 1.0f, 1000.0f);

	ver3 pos = mTarget->GetPosition();
	D3DXMatrixTranslation(&world, pos.x, pos.y, pos.z);

	matrix transform = world * view * projection;

	ver3 screenPos = ver3(transform._41, transform._42, transform._43);
	D3DXVec3Normalize(&screenPos, &screenPos);

	mTransform.pos.x = (SCREEN_WIDTH * 0.5f) + (static_cast<float>(screenPos.x) * (SCREEN_WIDTH * 0.5f));
	mTransform.pos.y = (SCREEN_HEIGHT * 0.5f) - (static_cast<float>(screenPos.y) * (SCREEN_HEIGHT * 0.5f));

	mTransform.pos.z = 0.0f;

}

void TargetObject::Draw()
{

	GameObject::Draw();

}
