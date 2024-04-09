#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* ModelManager�N���X�F���f�����Ǘ�����N���X
********************************************************************************************/

#include"Model.h"
#include"AnimationModel.h"

#include"ManagerData.h"

struct NormalModelData : public Data {
	Model* model;
};

struct AnimModelData : public Data {
	AnimationModel* model;
	list<string> anim;
};

class ModelManager
{
private:

	static unordered_map<uint, NormalModelData> mModelData;
	static unordered_map<uint, AnimModelData> mAnimModelData;

	static int mIndex;

	static class Scene* mScene;

public:

	static void Uninit();
	static void Uninit(crstring scene);

	static void SetScene(Scene* scene) { mScene = scene; }

	/*obj���f��*/
	static cuint LoadModel(crstring file, crstring name);
	static Model* GetModel(cruint id);

	/*�A�j���[�V����(fbx)���f��*/
	static cuint LoadAnimModel(crstring file, crstring name);
	static void LoadAnimMotion(cruint id, crstring file, crstring name);
	static void LoadAnimMotion(cruint id, crstring file, crstring name, crfloat blend);
	static AnimationModel* GetAnimModel(cruint id);

};