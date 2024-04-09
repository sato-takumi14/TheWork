
#include"main.h"

#include "DrawShadowModel.h"
#include"GameObject.h"
#include"ModelManager.h"
#include"TextureManager.h"

DrawShadowModel::DrawShadowModel(
	GameObject* target, 
	crint id, 
	crstring vertex, 
	crstring pixel, 
	const list<int> tex, 
	crcolor color)
{

	LoadShader(vertex, pixel);

	mTarget = target;
	mModelID = id;
	mTexIDs = tex;
	mColor = color;

}

void DrawShadowModel::Draw()
{

	if (!mDraw)return;

	SetShader();

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	//material.TextureEnable = true;
	material.Diffuse = mColor;
	Renderer::SetMaterial(material);

	{

		auto pos = mTarget->GetPosition();
		auto rot = mTarget->GetRotation();
		auto scale = mTarget->GetScale();

		/*マトリックス設定*/
		D3DXMATRIX World, Scale, Rot, Trans;
		D3DXMatrixScaling(&Scale, scale.x, scale.y, scale.z);
		D3DXMatrixRotationYawPitchRoll(&Rot, rot.y, rot.x, rot.z);
		D3DXMatrixTranslation(&Trans, pos.x, pos.y, pos.z);
		World = Scale * Rot * Trans;
		Renderer::SetWorldMatrix(&World);

	}

	auto shadow = Renderer::GetDepthShadowTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &shadow);

	ModelManager::GetModel(mModelID)->Draw();

}
