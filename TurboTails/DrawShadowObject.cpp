
#include"main.h"

#include "DrawShadowObject.h"
#include"ModelManager.h"
#include"TextureManager.h"
#include"GameObject.h"

DrawShadowObject::DrawShadowObject(
	GameObject* target, 
	crint model, 
	crstring vertex, 
	crstring pixel, 
	crint tex, 
	crcolor color)
{

	LoadShader(vertex, pixel);

	mTarget = target;
	mModelID = model;
	mTexID = tex;
	mColor = color;

}

void DrawShadowObject::Draw()
{

	if (!mDraw)return;

	SetShader();

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = mColor;
	Renderer::SetMaterial(material);

	{

		auto transform = mTarget->GetReferenceTransform();

		/*マトリックス設定*/
		D3DXMATRIX world, scale, rot, trans;
		D3DXMatrixScaling(&scale, transform.scale.x, transform.scale.y, transform.scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, transform.rot.y, transform.rot.x, transform.rot.z);
		D3DXMatrixTranslation(&trans, transform.pos.x, transform.pos.y, transform.pos.z);
		world = scale * rot * trans;
		Renderer::SetWorldMatrix(&world);

	}

	{

		auto shadow = Renderer::GetDepthShadowTexture();
		Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &shadow);

		ModelManager::GetModel(mModelID)->Draw();

	}

}
