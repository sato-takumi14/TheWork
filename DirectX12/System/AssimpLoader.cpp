#include "Main.h"
#include "AssimpLoader.h"
#include"System\Renderer.h"

namespace Assimp
{

	void Model::CreateBone(aiNode* node)
	{

		Bone bone;
		mBone[node->mName.C_Str()] = bone;

		for (uint n = 0; n < node->mNumChildren; n++)
		{
			CreateBone(node->mChildren[n]);
		}

	}

	void Model::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
	{

		Bone* bone = &mBone[node->mName.C_Str()];

		//マトリクスの乗算順番に注意
		aiMatrix4x4 worldMatrix;

		worldMatrix *= matrix;
		worldMatrix *= bone->AnimationMatrix;

		bone->Matrix = worldMatrix;
		bone->Matrix *= bone->OffsetMatrix;

		for (uint n = 0; n < node->mNumChildren; n++)
		{
			UpdateBoneMatrix(node->mChildren[n], worldMatrix);
		}

	}

	void Model::Load(const char* file)
	{

		mAiScene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
		assert(mAiScene);

		/*頂点バッファサイズ分確保*/
		mVertexBuffer.resize(mAiScene->mNumMeshes);
		/*インデックスバッファサイズ分確保*/
		mIndexBuffer.resize(mAiScene->mNumMeshes);

		/*変形後頂点配列生成*/
		mDeformVertex = new std::vector<Assimp::DeformVertex>[mAiScene->mNumMeshes];
		/*再帰的にボーンを生成*/
		CreateBone(mAiScene->mRootNode);

		for (uint m = 0; m < mAiScene->mNumMeshes; m++) {

			aiMesh* mesh = mAiScene->mMeshes[m];

			/*頂点バッファ生成*/
			{

				Vertex3D* vertex = new Vertex3D[mesh->mNumVertices];

				for (uint v = 0; v < mesh->mNumVertices; v++) {

					vertex[v].Position = XMFLOAT3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
					vertex[v].Normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
					vertex[v].TexCoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
					vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				}

				mVertexBuffer[m] = new Buffer::Vertex(
					sizeof(Vertex3D) * mesh->mNumVertices,
					sizeof(Vertex3D),
					vertex);

				delete[] vertex;

			}

			/*インデックスバッファ生成*/
			{

				uint32_t* index = new uint32_t[mesh->mNumFaces * 3];

				for (uint f = 0; f < mesh->mNumFaces; f++) {

					const aiFace* face = &mesh->mFaces[f];
					assert(face->mNumIndices == 3);

					index[f * 3 + 0] = face->mIndices[0];
					index[f * 3 + 1] = face->mIndices[1];
					index[f * 3 + 2] = face->mIndices[2];

				}

				mIndexBuffer[m] = new Buffer::Index(
					sizeof(uint32_t) * mesh->mNumFaces * 3,
					index);

				delete[] index;

			}

			/*変形後頂点データ初期化*/
			for (uint v = 0; v < mesh->mNumVertices; v++) {

				DeformVertex deformVertex;
				deformVertex.Position = mesh->mVertices[v];
				deformVertex.Normal = mesh->mNormals[v];
				deformVertex.BoneNum = 0;

				for (uint b = 0; b < 4; b++) {

					deformVertex.BoneName[b] = "";
					deformVertex.BoneWeight[b] = 0.0f;

				}

				mDeformVertex[m].push_back(deformVertex);

			}

			/*ボーンデータ初期化*/
			for (uint b = 0; b < mesh->mNumBones; b++) {

				aiBone* bone = mesh->mBones[b];

				mBone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

				/*変形後頂点にボーンでたー格納*/
				for (uint w = 0; w < bone->mNumWeights; w++) {

					aiVertexWeight weight = bone->mWeights[w];

					int num = mDeformVertex[m][weight.mVertexId].BoneNum;

					mDeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
					mDeformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();
					mDeformVertex[m][weight.mVertexId].BoneNum++;

					assert(mDeformVertex[m][weight.mVertexId].BoneNum <= 4);

				}

			}

		}

		/*テクスチャ読み込み*/
		for (uint i = 0; i < mAiScene->mNumTextures; i++) {

			aiTexture* tex = mAiScene->mTextures[i];
			TexMetadata meta{};
			ScratchImage scratch;

			if (FAILED(LoadFromWICMemory(
				(const unsigned char*)(tex)->pcData,
				tex->mWidth,
				WIC_FLAGS_NONE,
				&meta,
				scratch)))continue;

			auto img = scratch.GetImage(0, 0, 0);
			auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
			auto desc = CD3DX12_RESOURCE_DESC::Tex2D(meta.format,
				meta.width,
				static_cast<UINT>(meta.height),
				static_cast<UINT16>(meta.arraySize),
				static_cast<UINT16>(meta.mipLevels));

			ID3D12Resource* resource{};

			/*リソースの生成*/
			if (FAILED(Renderer::GetDevice()->CreateCommittedResource(
				&prop,
				D3D12_HEAP_FLAG_NONE,
				&desc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				nullptr,
				IID_PPV_ARGS(&resource)
			)))continue;

			if (FAILED(
				resource->WriteToSubresource(
					0,
					nullptr,
					img->pixels,
					static_cast<UINT>(img->rowPitch),
					static_cast<UINT>(img->slicePitch)
				)
			))continue;

			Texture::DescriptorHandle* handle = new Texture::DescriptorHandle();
			auto descriptorheap = new Texture::DescriptorHeap();

			auto heap = descriptorheap->GetHeap();
			auto count = descriptorheap->GetHandleSize();
			auto incrementsize = descriptorheap->GetIncrementSize();

			auto handleCPU = heap->GetCPUDescriptorHandleForHeapStart();
			handleCPU.ptr += incrementsize * count;

			auto handleGPU = heap->GetGPUDescriptorHandleForHeapStart();
			handleGPU.ptr += incrementsize * count;

			handle->mHandleCPU = handleCPU;
			handle->mHandleGPU = handleGPU;

			D3D12_SHADER_RESOURCE_VIEW_DESC shaderresourceview{};
			shaderresourceview.Format = resource->GetDesc().Format;
			shaderresourceview.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			shaderresourceview.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			shaderresourceview.Texture2D.MipLevels = 1;

			Renderer::GetDevice()->CreateShaderResourceView(resource, &shaderresourceview, handle->mHandleCPU);

			mHandle[tex->mFilename.data] = handle;
			mResource[tex->mFilename.data] = resource;
			mDescriptorHeap[tex->mFilename.data] = descriptorheap;

		}

	}

	void Model::LoadAnimation(crstring file, crstring name)
	{

		mAnimation[name] = aiImportFile(file.c_str(), aiProcess_ConvertToLeftHanded);
		assert(mAnimation[name]);

	}

	void Model::Unload()
	{

		for (uint m = 0; m < mAiScene->mNumMeshes; m++) {

			delete mVertexBuffer[m];
			delete mIndexBuffer[m];

		}

		for (auto& data : mResource) {
			data.second->Release();
		}
		mResource.clear();
		for (auto& data : mHandle) {
			delete data.second;
		}
		mHandle.clear();
		for (auto& data : mDescriptorHeap) {
			delete data.second;
		}
		mDescriptorHeap.clear();

		delete[] mDeformVertex;

		aiReleaseImport(mAiScene);

		for (std::pair<const string, const aiScene*> pair : mAnimation) {

			aiReleaseImport(pair.second);

		}

	}

	void Model::Update(crstring anim, crint frame)
	{

		if (mAnimation.count(anim) == 0)return;
		if (not mAnimation[anim]->HasAnimations())return;

		/*アニメーションデータからボーンマトリックス算出*/
		aiAnimation* animation = mAnimation[anim]->mAnimations[0];

		for (auto pair : mBone) {

			Bone* bone = &mBone[pair.first];

			aiNodeAnim* node = nullptr;

			for (uint c = 0; c < animation->mNumChannels; c++) {

				if (animation->mChannels[c]->mNodeName == aiString(pair.first)) {
					node = animation->mChannels[c];
					break;
				}

			}

			aiQuaternion rot;
			aiVector3D pos;
			int f;

			if (node) {

				/*簡易実装*/
				f = frame % node->mNumRotationKeys;
				rot = node->mRotationKeys[f].mValue;

				f = frame % node->mNumPositionKeys;
				pos = node->mPositionKeys[f].mValue;

			}

			bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);

		}

		/*再帰的にボーンマトリックスを更新*/
		aiMatrix4x4 root = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion((float)AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
		UpdateBoneMatrix(mAiScene->mRootNode, root);

		/*頂点変更*/
		for (uint m = 0; m < mAiScene->mNumMeshes; m++) {

			aiMesh* mesh = mAiScene->mMeshes[m];

			void* data = nullptr;
			mVertexBuffer[m]->GetBuffer()->Map(0, nullptr, &data);

			if (data) {

				Vertex3D* vertex = (Vertex3D*)(data);

				for (uint v = 0; v < mesh->mNumVertices; v++) {

					DeformVertex* deformVertex = &mDeformVertex[m][v];

					aiMatrix4x4 matrix[4];
					aiMatrix4x4 outMatrix;
					for (int i = 0; i < 4; i++) {

						matrix[i] = mBone[deformVertex->BoneName[i]].Matrix;

					}

					{
						outMatrix.a1 = matrix[0].a1 * deformVertex->BoneWeight[0]
							+ matrix[1].a1 * deformVertex->BoneWeight[1]
							+ matrix[2].a1 * deformVertex->BoneWeight[2]
							+ matrix[3].a1 * deformVertex->BoneWeight[3];

						outMatrix.a2 = matrix[0].a2 * deformVertex->BoneWeight[0]
							+ matrix[1].a2 * deformVertex->BoneWeight[1]
							+ matrix[2].a2 * deformVertex->BoneWeight[2]
							+ matrix[3].a2 * deformVertex->BoneWeight[3];

						outMatrix.a3 = matrix[0].a3 * deformVertex->BoneWeight[0]
							+ matrix[1].a3 * deformVertex->BoneWeight[1]
							+ matrix[2].a3 * deformVertex->BoneWeight[2]
							+ matrix[3].a3 * deformVertex->BoneWeight[3];

						outMatrix.a4 = matrix[0].a4 * deformVertex->BoneWeight[0]
							+ matrix[1].a4 * deformVertex->BoneWeight[1]
							+ matrix[2].a4 * deformVertex->BoneWeight[2]
							+ matrix[3].a4 * deformVertex->BoneWeight[3];



						outMatrix.b1 = matrix[0].b1 * deformVertex->BoneWeight[0]
							+ matrix[1].b1 * deformVertex->BoneWeight[1]
							+ matrix[2].b1 * deformVertex->BoneWeight[2]
							+ matrix[3].b1 * deformVertex->BoneWeight[3];

						outMatrix.b2 = matrix[0].b2 * deformVertex->BoneWeight[0]
							+ matrix[1].b2 * deformVertex->BoneWeight[1]
							+ matrix[2].b2 * deformVertex->BoneWeight[2]
							+ matrix[3].b2 * deformVertex->BoneWeight[3];

						outMatrix.b3 = matrix[0].b3 * deformVertex->BoneWeight[0]
							+ matrix[1].b3 * deformVertex->BoneWeight[1]
							+ matrix[2].b3 * deformVertex->BoneWeight[2]
							+ matrix[3].b3 * deformVertex->BoneWeight[3];

						outMatrix.b4 = matrix[0].b4 * deformVertex->BoneWeight[0]
							+ matrix[1].b4 * deformVertex->BoneWeight[1]
							+ matrix[2].b4 * deformVertex->BoneWeight[2]
							+ matrix[3].b4 * deformVertex->BoneWeight[3];



						outMatrix.c1 = matrix[0].c1 * deformVertex->BoneWeight[0]
							+ matrix[1].c1 * deformVertex->BoneWeight[1]
							+ matrix[2].c1 * deformVertex->BoneWeight[2]
							+ matrix[3].c1 * deformVertex->BoneWeight[3];

						outMatrix.c2 = matrix[0].c2 * deformVertex->BoneWeight[0]
							+ matrix[1].c2 * deformVertex->BoneWeight[1]
							+ matrix[2].c2 * deformVertex->BoneWeight[2]
							+ matrix[3].c2 * deformVertex->BoneWeight[3];

						outMatrix.c3 = matrix[0].c3 * deformVertex->BoneWeight[0]
							+ matrix[1].c3 * deformVertex->BoneWeight[1]
							+ matrix[2].c3 * deformVertex->BoneWeight[2]
							+ matrix[3].c3 * deformVertex->BoneWeight[3];

						outMatrix.c4 = matrix[0].c4 * deformVertex->BoneWeight[0]
							+ matrix[1].c4 * deformVertex->BoneWeight[1]
							+ matrix[2].c4 * deformVertex->BoneWeight[2]
							+ matrix[3].c4 * deformVertex->BoneWeight[3];



						outMatrix.d1 = matrix[0].d1 * deformVertex->BoneWeight[0]
							+ matrix[1].d1 * deformVertex->BoneWeight[1]
							+ matrix[2].d1 * deformVertex->BoneWeight[2]
							+ matrix[3].d1 * deformVertex->BoneWeight[3];

						outMatrix.d2 = matrix[0].d2 * deformVertex->BoneWeight[0]
							+ matrix[1].d2 * deformVertex->BoneWeight[1]
							+ matrix[2].d2 * deformVertex->BoneWeight[2]
							+ matrix[3].d2 * deformVertex->BoneWeight[3];

						outMatrix.d3 = matrix[0].d3 * deformVertex->BoneWeight[0]
							+ matrix[1].d3 * deformVertex->BoneWeight[1]
							+ matrix[2].d3 * deformVertex->BoneWeight[2]
							+ matrix[3].d3 * deformVertex->BoneWeight[3];

						outMatrix.d4 = matrix[0].d4 * deformVertex->BoneWeight[0]
							+ matrix[1].d4 * deformVertex->BoneWeight[1]
							+ matrix[2].d4 * deformVertex->BoneWeight[2]
							+ matrix[3].d4 * deformVertex->BoneWeight[3];

					}

					deformVertex->Position = mesh->mVertices[v];
					deformVertex->Position *= outMatrix;

					/*法線変換用に移動線分を削除*/
					outMatrix.a4 = 0.0f;
					outMatrix.b4 = 0.0f;
					outMatrix.c4 = 0.0f;

					deformVertex->Normal = mesh->mNormals[v];
					deformVertex->Normal *= outMatrix;

					/*頂点バッファへ書き込み*/
					vertex[v].Position.x = deformVertex->Position.x;
					vertex[v].Position.y = deformVertex->Position.y;
					vertex[v].Position.z = deformVertex->Position.z;

					vertex[v].Normal.x = deformVertex->Normal.x;
					vertex[v].Normal.y = deformVertex->Normal.y;
					vertex[v].Normal.z = deformVertex->Normal.z;

					vertex[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
					vertex[v].TexCoord.y = mesh->mTextureCoords[0][v].y;

					vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				}

			}

			mVertexBuffer[m]->GetBuffer()->Unmap(0, nullptr);

		}

	}

	void Model::Update(
		crstring animA, crint frameA,
		crstring animB, crint frameB,
		crfloat blend)
	{

		if (mAnimation.count(animA.c_str()) == 0)return;
		if (not mAnimation[animA]->HasAnimations())return;
		if (mAnimation.count(animB.c_str()) == 0)return;
		if (not mAnimation[animB]->HasAnimations())return;

		/*アニメーションデータからボーンマトリクス算出*/
		aiAnimation* animation1 = mAnimation[animA]->mAnimations[0];
		aiAnimation* animation2 = mAnimation[animB]->mAnimations[0];

		for (auto pair : mBone)
		{
			Bone* bone = &mBone[pair.first];


			aiNodeAnim* nodeAnim1 = nullptr;

			for (unsigned int c = 0; c < animation1->mNumChannels; c++)
			{
				if (animation1->mChannels[c]->mNodeName == aiString(pair.first))
				{
					nodeAnim1 = animation1->mChannels[c];
					break;
				}
			}

			aiNodeAnim* nodeAnim2 = nullptr;

			for (unsigned int c = 0; c < animation2->mNumChannels; c++)
			{
				if (animation2->mChannels[c]->mNodeName == aiString(pair.first))
				{
					nodeAnim2 = animation2->mChannels[c];
					break;
				}
			}

			int f;
			aiQuaternion rot1;
			aiVector3D pos1;

			if (nodeAnim1)
			{
				/*簡易実装*/
				f = frameA % nodeAnim1->mNumRotationKeys;
				rot1 = nodeAnim1->mRotationKeys[f].mValue;

				f = frameA % nodeAnim1->mNumPositionKeys;
				pos1 = nodeAnim1->mPositionKeys[f].mValue;
			}

			aiQuaternion rot2;
			aiVector3D pos2;

			if (nodeAnim2)
			{
				/*簡易実装*/
				f = frameB % nodeAnim2->mNumRotationKeys;
				rot2 = nodeAnim2->mRotationKeys[f].mValue;

				f = frameB % nodeAnim2->mNumPositionKeys;
				pos2 = nodeAnim2->mPositionKeys[f].mValue;
			}

			/*線形補間*/
			aiVector3D pos = (pos1 * (1.0f - blend)) + (pos2 * blend);
			/*球面線形補間*/
			aiQuaternion rot;
			aiQuaternion::Interpolate(rot, rot1, rot2, blend);


			bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);

		}




		/*再帰的にボーンマトリクスを更新*/
		aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion((float)AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
		UpdateBoneMatrix(mAiScene->mRootNode, rootMatrix);


		/*頂点変換（CPUスキニング）*/
		for (unsigned int m = 0; m < mAiScene->mNumMeshes; m++)
		{
			aiMesh* mesh = mAiScene->mMeshes[m];

			void* data;
			mVertexBuffer[m]->GetBuffer()->Map(0, nullptr, &data);

			Vertex3D* vertex = (Vertex3D*)data;

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				DeformVertex* deformVertex = &mDeformVertex[m][v];

				aiMatrix4x4 matrix[4];
				aiMatrix4x4 outMatrix;
				matrix[0] = mBone[deformVertex->BoneName[0]].Matrix;
				matrix[1] = mBone[deformVertex->BoneName[1]].Matrix;
				matrix[2] = mBone[deformVertex->BoneName[2]].Matrix;
				matrix[3] = mBone[deformVertex->BoneName[3]].Matrix;

				{
					outMatrix.a1 = matrix[0].a1 * deformVertex->BoneWeight[0]
						+ matrix[1].a1 * deformVertex->BoneWeight[1]
						+ matrix[2].a1 * deformVertex->BoneWeight[2]
						+ matrix[3].a1 * deformVertex->BoneWeight[3];

					outMatrix.a2 = matrix[0].a2 * deformVertex->BoneWeight[0]
						+ matrix[1].a2 * deformVertex->BoneWeight[1]
						+ matrix[2].a2 * deformVertex->BoneWeight[2]
						+ matrix[3].a2 * deformVertex->BoneWeight[3];

					outMatrix.a3 = matrix[0].a3 * deformVertex->BoneWeight[0]
						+ matrix[1].a3 * deformVertex->BoneWeight[1]
						+ matrix[2].a3 * deformVertex->BoneWeight[2]
						+ matrix[3].a3 * deformVertex->BoneWeight[3];

					outMatrix.a4 = matrix[0].a4 * deformVertex->BoneWeight[0]
						+ matrix[1].a4 * deformVertex->BoneWeight[1]
						+ matrix[2].a4 * deformVertex->BoneWeight[2]
						+ matrix[3].a4 * deformVertex->BoneWeight[3];



					outMatrix.b1 = matrix[0].b1 * deformVertex->BoneWeight[0]
						+ matrix[1].b1 * deformVertex->BoneWeight[1]
						+ matrix[2].b1 * deformVertex->BoneWeight[2]
						+ matrix[3].b1 * deformVertex->BoneWeight[3];

					outMatrix.b2 = matrix[0].b2 * deformVertex->BoneWeight[0]
						+ matrix[1].b2 * deformVertex->BoneWeight[1]
						+ matrix[2].b2 * deformVertex->BoneWeight[2]
						+ matrix[3].b2 * deformVertex->BoneWeight[3];

					outMatrix.b3 = matrix[0].b3 * deformVertex->BoneWeight[0]
						+ matrix[1].b3 * deformVertex->BoneWeight[1]
						+ matrix[2].b3 * deformVertex->BoneWeight[2]
						+ matrix[3].b3 * deformVertex->BoneWeight[3];

					outMatrix.b4 = matrix[0].b4 * deformVertex->BoneWeight[0]
						+ matrix[1].b4 * deformVertex->BoneWeight[1]
						+ matrix[2].b4 * deformVertex->BoneWeight[2]
						+ matrix[3].b4 * deformVertex->BoneWeight[3];



					outMatrix.c1 = matrix[0].c1 * deformVertex->BoneWeight[0]
						+ matrix[1].c1 * deformVertex->BoneWeight[1]
						+ matrix[2].c1 * deformVertex->BoneWeight[2]
						+ matrix[3].c1 * deformVertex->BoneWeight[3];

					outMatrix.c2 = matrix[0].c2 * deformVertex->BoneWeight[0]
						+ matrix[1].c2 * deformVertex->BoneWeight[1]
						+ matrix[2].c2 * deformVertex->BoneWeight[2]
						+ matrix[3].c2 * deformVertex->BoneWeight[3];

					outMatrix.c3 = matrix[0].c3 * deformVertex->BoneWeight[0]
						+ matrix[1].c3 * deformVertex->BoneWeight[1]
						+ matrix[2].c3 * deformVertex->BoneWeight[2]
						+ matrix[3].c3 * deformVertex->BoneWeight[3];

					outMatrix.c4 = matrix[0].c4 * deformVertex->BoneWeight[0]
						+ matrix[1].c4 * deformVertex->BoneWeight[1]
						+ matrix[2].c4 * deformVertex->BoneWeight[2]
						+ matrix[3].c4 * deformVertex->BoneWeight[3];



					outMatrix.d1 = matrix[0].d1 * deformVertex->BoneWeight[0]
						+ matrix[1].d1 * deformVertex->BoneWeight[1]
						+ matrix[2].d1 * deformVertex->BoneWeight[2]
						+ matrix[3].d1 * deformVertex->BoneWeight[3];

					outMatrix.d2 = matrix[0].d2 * deformVertex->BoneWeight[0]
						+ matrix[1].d2 * deformVertex->BoneWeight[1]
						+ matrix[2].d2 * deformVertex->BoneWeight[2]
						+ matrix[3].d2 * deformVertex->BoneWeight[3];

					outMatrix.d3 = matrix[0].d3 * deformVertex->BoneWeight[0]
						+ matrix[1].d3 * deformVertex->BoneWeight[1]
						+ matrix[2].d3 * deformVertex->BoneWeight[2]
						+ matrix[3].d3 * deformVertex->BoneWeight[3];

					outMatrix.d4 = matrix[0].d4 * deformVertex->BoneWeight[0]
						+ matrix[1].d4 * deformVertex->BoneWeight[1]
						+ matrix[2].d4 * deformVertex->BoneWeight[2]
						+ matrix[3].d4 * deformVertex->BoneWeight[3];

				}

				deformVertex->Position = mesh->mVertices[v];
				deformVertex->Position *= outMatrix;


				/*法線変換用に移動成分を削除*/
				outMatrix.a4 = 0.0f;
				outMatrix.b4 = 0.0f;
				outMatrix.c4 = 0.0f;

				deformVertex->Normal = mesh->mNormals[v];
				deformVertex->Normal *= outMatrix;


				/*頂点バッファへ書き込み*/
				vertex[v].Position.x = deformVertex->Position.x;
				vertex[v].Position.y = deformVertex->Position.y;
				vertex[v].Position.z = deformVertex->Position.z;

				vertex[v].Normal.x = deformVertex->Normal.x;
				vertex[v].Normal.y = deformVertex->Normal.y;
				vertex[v].Normal.z = deformVertex->Normal.z;

				vertex[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
				vertex[v].TexCoord.y = mesh->mTextureCoords[0][v].y;

				vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			mVertexBuffer[m]->GetBuffer()->Unmap(0, nullptr);

		}

	}

	void Model::Draw()
	{

		auto commandlist = Renderer::GetCommandList();
		/*プリミティブとポロ時設定*/
		commandlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		uint i = 0;

		for (uint m = 0; m < mAiScene->mNumMeshes; m++) {

			aiMesh* mesh = mAiScene->mMeshes[m];

			/*マテリアル設定*/
			aiString tex;
			aiColor3D diffuse;
			float opacity;

			aiMaterial* material = mAiScene->mMaterials[mesh->mMaterialIndex];
			material->GetTexture(aiTextureType_DIFFUSE, 0, &tex);
			material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
			material->Get(AI_MATKEY_OPACITY, opacity);

			if (tex != aiString("")) {

				/*テクスチャ設定*/
				auto heap = mDescriptorHeap[tex.data]->GetHeap();
				commandlist->SetDescriptorHeaps(1, &heap);
				commandlist->SetGraphicsRootDescriptorTable(1, mHandle[tex.data]->mHandleGPU);

			}

			/*頂点バッファ設定*/
			commandlist->IASetVertexBuffers(0, 1, mVertexBuffer[m]->GetView());

			/*インデックスバッファ設定*/
			commandlist->IASetIndexBuffer(mIndexBuffer[m]->GetView());

			/*描画*/
			commandlist->DrawIndexedInstanced(mesh->mNumFaces * 3, 1, 0, 0, 0);

		}

	}

}

