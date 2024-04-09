
#include"Main.h"

#include "ModelManager.h"
#include"Scene.h"

unordered_map<uint, NormalModelData> ModelManager::mModelData{};
unordered_map<uint, AnimModelData> ModelManager::mAnimModelData{};

int ModelManager::mIndex{};
Scene* ModelManager::mScene{};

void ModelManager::Uninit()
{

	for (auto& data : mModelData) {
		data.second.model->Unload();
		delete data.second.model;
	}
	mModelData.clear();

	for (auto& data : mAnimModelData) {
		data.second.model->Unload();
		delete data.second.model;
	}
	mAnimModelData.clear();

	mIndex = 0;

}

void ModelManager::Uninit(crstring scene)
{

	erase_if(mModelData, [&](auto& data)->bool {

		if (data.second.scene == scene) {
			data.second.model->Unload();
			delete data.second.model;
			return true;
		}

		return false;

		});

	erase_if(mAnimModelData, [&](auto& data)->bool {

		if (data.second.scene == scene) {
			data.second.model->Unload();
			delete data.second.model;
			return true;
		}

		return false;

		});

}

cuint ModelManager::LoadModel(crstring file, crstring name)
{

	for (auto& data : mModelData) {
		if (data.second.name == name) {
			if (data.second.file == file) {
				if (data.second.scene == mScene->GetName()) {
					return data.second.id;
				}
			}
		}
	}

	mModelData[mIndex].model = new Model(file.c_str());
	mModelData[mIndex].file = file;
	mModelData[mIndex].name = name;
	mModelData[mIndex].id = mIndex;
	mModelData[mIndex].scene = mScene->GetName();

	return mIndex++;

}

cuint ModelManager::LoadAnimModel(crstring file, crstring name)
{

	for (auto& data : mAnimModelData) {
		if (data.second.name == name) {
			if (data.second.file == file) {
				if (data.second.scene == mScene->GetName()) {
					return data.second.id;
				}				
			}
		}
	}

	mAnimModelData[mIndex].model = new AnimationModel(file.c_str());
	mAnimModelData[mIndex].file = file;
	mAnimModelData[mIndex].name = name;
	mAnimModelData[mIndex].id = mIndex;
	mAnimModelData[mIndex].scene = mScene->GetName();

	return mIndex++;

}

void ModelManager::LoadAnimMotion(cruint id, crstring file, crstring name)
{
	if (id < 0)return;

	bool flag = true;

	for (auto& data : mAnimModelData[id].anim) {
		if (data == file) {
			flag = false;
			break;
		}
	}

	if (flag) {
		mAnimModelData[id].anim.push_back(file);
		mAnimModelData[id].model->LoadAnimation(file.c_str(), name.c_str());
	}
}

void ModelManager::LoadAnimMotion(cruint id, crstring file, crstring name, crfloat blend)
{
	if (id < 0)return;

	bool flag = true;

	for (auto& data : mAnimModelData[id].anim) {
		if (data == file) {
			flag = false;
			break;
		}
	}

	if (flag) {
		mAnimModelData[id].anim.push_back(file);
		mAnimModelData[id].model->LoadAnimation(file.c_str(), name.c_str());
		mAnimModelData[id].model->SetBlend(name, blend);
	}
}

Model* ModelManager::GetModel(cruint id)
{
	if (id < 0)return nullptr;

	return mModelData[id].model;
}

AnimationModel* ModelManager::GetAnimModel(cruint id)
{
	if (id < 0)return nullptr;

	return mAnimModelData[id].model;
}
