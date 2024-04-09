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

	/*�r���[�}�g���b�N�X�ݒ�*/
	mView = DirectX::XMMatrixLookAtLH(
		mTransform.pos.Vector(),
		target.Vector(),
		Vector3::Vector(Vector3::Up()));

	/*�v���W�F�N�V�����}�g���b�N�X�ݒ�*/
	mProjection = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(100.0f),
		WindowWidth / WindowHeight,
		0.3f,
		1000.0f);

	/*�r���[�}�g���b�N�X�Z�b�g*/
	Renderer::SetViewMatrix(&mView);
	/*�v���W�F�N�V�����}�g���b�N�X�Z�b�g*/
	Renderer::SetProjectionMatrix(&mProjection);

}

void Camera::Uninit()
{
}
