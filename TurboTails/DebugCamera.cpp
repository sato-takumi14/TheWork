
#include"main.h"

#include "DebugCamera.h"

#include"Scene.h"
#include"Input.h"
#include"Time.h"

void DebugCamera::Init()
{

	mLayer.push_back("bg");
	mClass = "DebugCamera";

}

void DebugCamera::Start()
{

	mScene->SetDebugCamera(this);
	mTarget = mScene->GetGameObjectTag("Player");

}

void DebugCamera::Update()
{

	if (!mFlag)return;

	/*移動*/
	{

		if (Input::GetKeyPress('W')) {
			mTargetPos += mTransform.GetForward() * Time::GetDeltaTime();
		}
		if (Input::GetKeyPress('S')) {
			mTargetPos -= mTransform.GetForward() * Time::GetDeltaTime();
		}
		if (Input::GetKeyPress('A')) {
			mTargetPos -= mTransform.GetRight() * Time::GetDeltaTime();
		}
		if (Input::GetKeyPress('D')) {
			mTargetPos += mTransform.GetRight() * Time::GetDeltaTime();
		}
		if (Input::GetKeyPress('E')) {
			mTargetPos += mTransform.GetUp() * Time::GetDeltaTime();
		}
		if (Input::GetKeyPress('Q')) {
			mTargetPos -= mTransform.GetUp() * Time::GetDeltaTime();
		}

		mTransform.rot.x += static_cast<float>(Input::GetMouseY()) * 0.01f;
		mTransform.rot.y += static_cast<float>(Input::GetMouseX()) * 0.01f;

		mTransform.pos = mTargetPos - (mTransform.GetForward() * 5.0f) + Vector3::up();

	}

}

void DebugCamera::Draw()
{

	if (!mFlag)return;

	/*ビューマトリックス設定*/
	ver3 up = ver3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &mTransform.pos, &mTargetPos, &up);

	Renderer::SetViewMatrix(&mView);

	/*プロジェクションマトリックス設定
	  引数2番目画角*/
	D3DXMatrixPerspectiveFovLH(&mProjection, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&mProjection);

}

void DebugCamera::Uninit()
{



}

void DebugCamera::Set(crver3 set)
{

	mTargetPos = mTarget->GetPosition();

}

void DebugCamera::Set(crbool set)
{
	mFlag = !mFlag;
}
