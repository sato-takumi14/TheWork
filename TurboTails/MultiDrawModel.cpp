
#include"main.h"

#include "MultiDrawModel.h"

#include"GameObject.h"
#include"Shader.h"
#include"Geometry.h"
#include"ModelManager.h"

MultiDrawModel::MultiDrawModel(
	GameObject* obj, 
	cruint model, 
	crstring shadername,
	const list<int>& tex)
{

	mTarget = obj;
	mModelID = model;
	mShaderName = shadername;
	mTexIDs = tex;

	mGeometry = new Geometry();
	mGeometry->Create(mTarget);

}

MultiDrawModel::MultiDrawModel(
	GameObject* obj, 
	cruint model, 
	crstring vertex, 
	crstring pixel, 
	const list<int>& tex)
{



}

void MultiDrawModel::Draw()
{

	Shader::SetShader(mShaderName);

	mGeometry->SetGeometry(mTarget);

	ModelManager::GetModel(mModelID)->Draw(static_cast<uint>(mTarget->GetGameObjects().size()));

}

void MultiDrawModel::Uninit()
{

	DrawInterface::Uninit();

	if (mGeometry) {
		mGeometry->Uninit();
		delete mGeometry;
	}

}
