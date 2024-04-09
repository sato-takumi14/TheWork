
#include"main.h"

#include "DrawAnimModel.h"
#include"ModelManager.h"
#include"TextureManager.h"
#include"GameObject.h"
#include"AnimationModel.h"

DrawAnimModel::DrawAnimModel(
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

void DrawAnimModel::Draw()
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
		quat quat;

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

	{

		auto model = ModelManager::GetAnimModel(mModelID);
		model->Update(
			mAnimName, mTime,
			mNextAnimName, mTime++,
			mBlend);

		mBlend += model->GetBlend(mNextAnimName);

		if (mBlend > 1.0f)mBlend = 1.0f;

		model->Draw();

	}

}

void DrawAnimModel::Uninit()
{
	DrawInterface::Uninit();
}

void DrawAnimModel::Set(crstring set)
{

	if (mNextAnimName != set) {
		mAnimName = mNextAnimName;
		mNextAnimName = set;
		mBlend = 0.0f;
		mTime = 0;
	}

}

void DrawAnimModel::SetAnimName(crstring name)
{

	if (mNextAnimName != name) {
		mAnimName = mNextAnimName;
		mNextAnimName = name;
		mBlend = 0.0f;
		mTime = 0;
	}

}

void DrawAnimModel::SetAnimName(crstring name, crfloat blend)
{
	if (mNextAnimName != name) {
		mAnimName = mNextAnimName;
		mNextAnimName = name;
		mBlend = 0.0f;
		mTime = 0;
		mAddBlends[name] = blend;
	}
}

void DrawAnimModel::SetRotation(crfloat set)
{

	quat quat{};
	ver3 rot = mTarget->GetRotation();

	D3DXQuaternionRotationYawPitchRoll(&quat, rot.y, rot.x, rot.z);

	D3DXQuaternionSlerp(&mQuaternion, &mQuaternion, &quat, set);

}
