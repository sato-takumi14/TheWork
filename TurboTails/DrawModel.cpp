
#include"main.h"

#include "DrawModel.h"
#include"ModelManager.h"
#include"GameObject.h"
#include"TextureManager.h"

DrawModel::DrawModel(
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

void DrawModel::Draw()
{

	if (!mDraw)return;

	SetShader();

	{

		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = mColor;
		Renderer::SetMaterial(material);

	}

	{

		auto pos = mTarget->GetPosition();
		auto rot = mTarget->GetRotation();
		auto scale = mTarget->GetScale();
		quat quat{};

		/*マトリックス設定*/
		D3DXMATRIX World, Scale, Rot, Trans;
		D3DXMatrixScaling(&Scale, scale.x, scale.y, scale.z);
		D3DXQuaternionRotationYawPitchRoll(&quat, rot.y, rot.x, rot.z);
		D3DXMatrixRotationQuaternion(&Rot, &quat);
		D3DXMatrixTranslation(&Trans, pos.x, pos.y, pos.z);
		World = Scale * Rot * Trans;
		Renderer::SetWorldMatrix(&World);

	}

	int index{};
	for (auto& data : mTexIDs) {
		Renderer::GetDeviceContext()->PSSetShaderResources(index++, 1, TextureManager::GetTexture(data));
	}

	ModelManager::GetModel(mModelID)->Draw();

}
