
#include"Main.h"
#include"ModelManager.h"
#include"System\AssimpLoader.h"
#include"Interface\Scene.h"

std::unordered_map<uint, ModelManager::Data> ModelManager::Device::mAssimpModelData{};
uint ModelManager::Device::mIndex{};
Scene* ModelManager::Device::mScene{};

void ModelManager::Device::MasterInit()
{
	
}

void ModelManager::Device::MasterUninit()
{

	string name = mScene->GetName();
	std::erase_if(mAssimpModelData, [name](auto& data)->bool {

		if (data.second.scene == name) {
			data.second.model->Unload();
			delete data.second.model;
			return true;
		}

		return false;

		});

}

cuint ModelManager::Device::LoadModel(crstring file)
{
	
	for (auto& data : mAssimpModelData) {
		if (data.second.file == file) return data.first;
	}

	mAssimpModelData[mIndex].model = new Assimp::Model(file);
	mAssimpModelData[mIndex].file = file;
	mAssimpModelData[mIndex].scene = mScene->GetName();

	return mIndex++;

}

cbool ModelManager::Device::LoadAnimation(cruint id, crstring file, crstring name)
{
	
	if (id < 0)return false;

	for (auto& data : mAssimpModelData[id].anim) {
		if (data == file)return false;
	}

	mAssimpModelData[id].anim.push_back(file);
	mAssimpModelData[id].model->LoadAnimation(file, name);

	return true;

}

cbool ModelManager::Device::LoadAnimation(cruint id, crstring file, crstring name, crfloat blend)
{
	
	if (id < 0)return false;

	for (auto& data : mAssimpModelData[id].anim) {
		if (data == file)return false;
	}

	mAssimpModelData[id].anim.push_back(file);
	mAssimpModelData[id].model->LoadAnimation(file, name);
	mAssimpModelData[id].model->SetBlend(name, blend);

	return true;

}

Assimp::Model* ModelManager::Device::GetModel(cruint id)
{
	return nullptr;
}
