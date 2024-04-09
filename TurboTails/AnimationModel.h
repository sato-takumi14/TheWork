#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* AnimationModel�N���X�F3D���f���ƃA�j���[�V���������邽�߂̃N���X
********************************************************************************************/

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp-vc143-mt.lib")

#include"Component.h"

//�ό`�㒸�_�\����
struct DEFORM_VERTEX
{
	aiVector3D Position;
	aiVector3D Normal;
	int				BoneNum;
	std::string		BoneName[4];//�{���̓{�[���C���f�b�N�X�ŊǗ�����ׂ�
	float			BoneWeight[4];
};

//�{�[���\����
struct BONE
{
	aiMatrix4x4 Matrix;
	aiMatrix4x4 AnimationMatrix;
	aiMatrix4x4 OffsetMatrix;
};

class AnimationModel : public Component
{
private:
	const aiScene* mAiScene = nullptr;
	unordered_map<string, const aiScene*> mAnimation;

	ID3D11Buffer** mVertexBuffer;
	ID3D11Buffer** mIndexBuffer;

	unordered_map<string, ID3D11ShaderResourceView*> mTexture;

	vector<DEFORM_VERTEX>* mDeformVertex;//�ό`�㒸�_�f�[�^
	unordered_map<string, BONE> mBone;//�{�[���f�[�^�i���O�ŎQ�Ɓj

	unordered_map<string, float> mBlend{};

private:

	void CreateBone(aiNode* Node);
	void UpdateBoneMatrix(aiNode* Node, aiMatrix4x4 Matrix);

public:

	AnimationModel() {}
	AnimationModel(crstring file) { Load(file.c_str()); }

	void Load(const char* FileName);
	void LoadAnimation(const char* FileName, const char* Name);
	void Unload();

	void Update(const char* AnimationName, int Frame);

	void Update(
		const string& animnameA, const int& freamA,
		const string& animnameB, const int& freamB, const float& blend);

	void Draw()override;

	void SetBlend(crstring name, crfloat blend) { mBlend[name] = blend; }
	cfloat GetBlend(crstring name) { return mBlend[name]; }

};