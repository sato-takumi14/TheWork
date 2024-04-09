#include "Main.h"
#include "Camera.h"

Camera::Camera(crbool main)
{

	if (main) mTag = "MainCamera";
	else mTag = "SubCamera";

}

void Camera::Init()
{
	mLayer.push_back("camera");
}

void Camera::Start()
{
}

void Camera::Update()
{
}

void Camera::Draw()
{		

	cVector3 target = mTarget->GetWorldPosition();

	/*ビューマトリックス設定*/
	mView = DirectX::XMMatrixLookAtLH(
		mTransform.pos.Vector(),
		target.Vector(),
		Vector3::Vector(Vector3::Up()));

	/*プロジェクションマトリックス設定*/
	mProjection = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(100.0f),
		WindowWidth / WindowHeight,
		0.3f,
		1000.0f);

	/*ビューマトリックスセット*/
	Renderer::SetViewMatrix(&mView);
	/*プロジェクションマトリックスセット*/
	Renderer::SetProjectionMatrix(&mProjection);

}

void Camera::Uninit()
{
}
