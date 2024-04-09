
#include"Main.h"

#include "Light.h"
#include"Renderer.h"

void Light::SetLight()
{
	//���C�g�J�����\���̂̏�����
	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���C�g�J�����̃r���[�s����쐬
	D3DXVECTOR3 lightpos = D3DXVECTOR3(-10.0f, 10.0f, -10.0f) * 4;
	D3DXVECTOR3 lightTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&light.ViewMatrix, &lightpos, &lightTarget, &lightUp);
	//���C�g�J�����̃v���W�F�N�V�����s����쐬
	D3DXMatrixPerspectiveFovLH(
		&light.ProjectionMatrix, 1.0f,
		static_cast<float>(SCREEN_WIDTH / SCREEN_HEIGHT),
		5.0f, 100.0f);
}

void Light::SetLightMatrix()
{

	//���C�g�J�����\���̂̏�����
	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���C�g�J�����̃r���[�s����쐬
	D3DXVECTOR3 lightpos = D3DXVECTOR3(-10.0f, 10.0f, -10.0f) * 4;
	D3DXVECTOR3 lightTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&light.ViewMatrix, &lightpos, &lightTarget, &lightUp);
	//���C�g�J�����̃v���W�F�N�V�����s����쐬
	D3DXMatrixPerspectiveFovLH(
		&light.ProjectionMatrix, 1.0f,
		static_cast<float>(SCREEN_WIDTH / SCREEN_HEIGHT),
		5.0f, 100.0f);

	Renderer::SetViewMatrix(&light.ViewMatrix);
	Renderer::SetProjectionMatrix(&light.ProjectionMatrix);
}