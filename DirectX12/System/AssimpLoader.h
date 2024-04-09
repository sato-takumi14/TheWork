#pragma once

/***********************************************************************************************************
*
*	Assimp
*
*																				����ҁF������
*																				������F02/25
*
************************************************************************************************************
* Assimp���g�������f����ǂݍ��ݕ`�悷��N���X
************************************************************************************************************/

#include"Main.h"
#include"assimp\cimport.h"
#include"assimp\scene.h"
#include"assimp\postprocess.h"
#include"assimp\matrix4x4.h"

#include"System\Buffer.h"
#include"System\Texture.h"
#include"System\Shader.h"

namespace Assimp
{

	/*�ό`�㒸�_�\����*/
	struct DeformVertex
	{
		aiVector3D Position;
		aiVector3D Normal;
		int BoneNum;
		string BoneName[4];
		float BoneWeight[4];
	};

	/*�{�[���\����*/
	struct Bone
	{
		aiMatrix4x4 Matrix;
		aiMatrix4x4 AnimationMatrix;
		aiMatrix4x4 OffsetMatrix;
	};

	class Model
	{
	private:

		const aiScene* mAiScene{};
		std::unordered_map<string, const aiScene*> mAnimation{};
		
		std::vector<Buffer::Vertex*> mVertexBuffer{};								/*���_�o�b�t�@*/
		std::vector<Buffer::Index*> mIndexBuffer{};									/*�C���f�b�N�X�o�b�t�@*/

		std::vector<DeformVertex>* mDeformVertex{};									/*�ό`�㒸�_�f�[�^*/
		std::unordered_map<string, Bone> mBone{};									/*�{�[���f�[�^*/

		std::unordered_map<string, float> mBlend{};									/*�u�����h�f�[�^*/

		std::unordered_map<string, Texture::Texture*> mTexture{};					/*�e�N�X�`��*/
		std::unordered_map<string, Texture::DescriptorHeap*> mDescriptorHeap{};		/*�f�B�X�N���v�^*/
		std::unordered_map<string, Texture::DescriptorHandle*> mHandle{};			/*�f�B�X�N���v�^�n���h��*/
		std::unordered_map<string, ID3D12Resource*> mResource{};
		Shader* mShader{};

	private:

		void CreateBone(aiNode* node);
		void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

	public:

		inline Model() {}
		inline Model(Shader* shader) : mShader(shader) {}
		inline Model(crstring file) {}

		void Load(const char* file);
		void LoadAnimation(crstring file, crstring name);
		void Unload();

		void Update(crstring anim, crint frame);

		void Update(
			crstring animA, crint frameA,
			crstring animB, crint frameB,
			crfloat blend);

		void Draw();

		inline void SetBlend(crstring name, crfloat blend) { mBlend[name] = blend; }
		inline cfloat GetBlend(crstring name) { return mBlend[name]; }

	};

}
