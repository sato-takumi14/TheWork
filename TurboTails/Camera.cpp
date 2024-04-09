
#include"main.h"
#include "Camera.h"

#include"Input.h"
#include"Scene.h"
#include"Renderer.h"
#include"CameraState.h"
#include"ActionKey.h"
#include"CircleCollider.h"
#include"Debug.h"

Camera::Camera(crbool main)
{

	if (main) {
		mTag = "MainCamera";
		mClass = "MainCamera";
	}
	else {
		mTag = "SubCamera";
		mClass = "SubCamera";
	}

}

void Camera::Init()
{

	mTransform.pos = ver3(0.0f, 5.0f, -10.0f);
	mTransform.rot = Vector3::zero();
	mTargetPos = ver3(0.0f, 0.0f, 0.0f);
	
	mLayer.emplace_back("bg");
	mSensitivity = 0.001f;

	mAddRot = Vector3::zero();

	AddComponent(new CircleCollider(this, Vector3::zero(), 1.0f));

}

void Camera::Start()
{

	mStateManager = new StateManager();

	mTarget = mScene->GetGameObjectTag("Player");
	if (mScene->GetName() != "title")mStateManager->AddState("move", new CameraState::CameraNormal(this, mTarget, Vector3::zero(), 5.0f));
	else mStateManager->AddState("move", new CameraState::NoneCamera(this, mTarget, Vector3::zero(), 5.0f));

}

void Camera::Draw()
{

	/*ビューマトリックス設定*/
	ver3 up = Vector3::up();
	ver3 pos = Vector3::zero();
	ver3 target = Vector3::zero();

	State* state = mStateManager->GetState("move");
	float offset = state->GetFloat();

	pos = mTransform.pos + ver3(0.0f, offset, 0.0f);
	target = state->GetVer3() + ver3(0.0f, offset, 0.0f);
	

	D3DXMatrixLookAtLH(&mViewMatrix, &pos, &target, &up);

	Renderer::SetViewMatrix(&mViewMatrix);

	/*プロジェクションマトリックス設定
	  引数2番目画角*/
	D3DXMatrixPerspectiveFovLH(&mProjectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&mProjectionMatrix);

	Renderer::SetCameraPosition(mTransform.pos);

}

void Camera::OnCollisionEnter(crint id)
{
	
}

void Camera::OnCollisionStay(crint id)
{
	mTransform.UndoPosition();
}

void Camera::OnCollisionExit(crint id)
{
}

cver3 Camera::GetVer3() const
{
	return dynamic_cast<CameraState::CameraStateInterface*>(mStateManager->GetState("move"))->GetRotation();
}

cbool Camera::CheckView(crver3 pos, crfloat len)
{

	D3DXMATRIX vp, invvp;

	vp = mViewMatrix * mProjectionMatrix;
	D3DXMatrixInverse(&invvp, NULL, &vp);

	ver3 vpos[4];
	ver3 wpos[4];

	vpos[0] = ver3(-1.0f, 1.0f, 1.0f);
	vpos[1] = Vector3::one();
	vpos[2] = ver3(-1.0f, -1.0f, 1.0f);
	vpos[3] = ver3(1.0f, -1.0f, 1.0f);

	D3DXVec3TransformCoord(&wpos[0], &vpos[0], &invvp);
	D3DXVec3TransformCoord(&wpos[1], &vpos[1], &invvp);
	D3DXVec3TransformCoord(&wpos[2], &vpos[2], &invvp);
	D3DXVec3TransformCoord(&wpos[3], &vpos[3], &invvp);

	ver3 v, v1, v2, n;

	v = pos - mTransform.pos;

	/*左面判定*/
	{

		v1 = wpos[0] - mTransform.pos;
		v2 = wpos[2] - mTransform.pos;
		D3DXVec3Cross(&n, &v1, &v2);
		D3DXVec3Normalize(&n, &n);

		if (D3DXVec3Dot(&n, &v) < 0.0f) {
			return false;
		}

	}

	/*右面判定*/
	{

		v1 = wpos[3] - mTransform.pos;
		v2 = wpos[1] - mTransform.pos;
		D3DXVec3Cross(&n, &v1, &v2);
		D3DXVec3Normalize(&n, &n);

		if (D3DXVec3Dot(&n, &v) < 0.0f) {
			return false;
		}

	}

	return true;
}
