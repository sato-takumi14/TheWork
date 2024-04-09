#pragma once

/***********************************************************************************************************
*
*	Assimp
*
*																				制作者：佐藤匠
*																				制作日：02/25
*
************************************************************************************************************
* Assimpを使ったモデルを読み込み描画するクラス
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

	/*変形後頂点構造体*/
	struct DeformVertex
	{
		aiVector3D Position;
		aiVector3D Normal;
		int BoneNum;
		string BoneName[4];
		float BoneWeight[4];
	};

	/*ボーン構造体*/
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
		
		std::vector<Buffer::Vertex*> mVertexBuffer{};								/*頂点バッファ*/
		std::vector<Buffer::Index*> mIndexBuffer{};									/*インデックスバッファ*/

		std::vector<DeformVertex>* mDeformVertex{};									/*変形後頂点データ*/
		std::unordered_map<string, Bone> mBone{};									/*ボーンデータ*/

		std::unordered_map<string, float> mBlend{};									/*ブレンドデータ*/

		std::unordered_map<string, Texture::Texture*> mTexture{};					/*テクスチャ*/
		std::unordered_map<string, Texture::DescriptorHeap*> mDescriptorHeap{};		/*ディスクリプタ*/
		std::unordered_map<string, Texture::DescriptorHandle*> mHandle{};			/*ディスクリプタハンドル*/
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
