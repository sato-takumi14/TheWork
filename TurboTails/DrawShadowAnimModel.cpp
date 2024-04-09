
#include"main.h"

#include "DrawShadowAnimModel.h"
#include"ModelManager.h"
#include"TextureManager.h"
#include"GameObject.h"
#include"AnimationModel.h"

DrawShadowAnimModel::DrawShadowAnimModel(
	GameObject* target, 
	crint id, 
	crstring vertex, 
	crstring pixel, 
	const list<int> tex, 
	crcolor color, 
	crfloat addblend, 
	crstring anim)
{

	LoadShader(vertex, pixel);

	mTarget = target;
	mModelID = id;
	mTexIDs = tex;
	mColor = color;
	mAddBlend = addblend;
	mAnimName = mNextAnimName = anim;

}

void DrawShadowAnimModel::Draw()
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

		/*マトリックス設定*/
		D3DXMATRIX World, Scale, Rot, Trans;
		D3DXMatrixScaling(&Scale, scale.x, scale.y, scale.z);
		D3DXMatrixRotationYawPitchRoll(&Rot, rot.y, rot.x, rot.z);
		D3DXMatrixTranslation(&Trans, pos.x, pos.y, pos.z);
		World = Scale * Rot * Trans;
		Renderer::SetWorldMatrix(&World);

	}

	int index{};
	for (auto& data : mTexIDs) {
		Renderer::GetDeviceContext()->PSSetShaderResources(index++, 1, TextureManager::GetTexture(data));
	}

	auto shadow = Renderer::GetDepthShadowTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &shadow);

	{

		auto model = ModelManager::GetAnimModel(mModelID);
		model->Update(
			mAnimName, mTime,
			mNextAnimName, mTime++,
			mBlend);

		mBlend += mAddBlend;

		if (mBlend > 1.0f)mBlend = 1.0f;

		model->Draw();

	}

}

void DrawShadowAnimModel::Set(crstring set)
{

	if (mNextAnimName != set) {
		mAnimName = mNextAnimName;
		mNextAnimName = set;
		mBlend = 0.0f;
		mTime = 0;
	}

}
