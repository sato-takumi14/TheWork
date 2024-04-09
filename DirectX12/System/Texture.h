#pragma once

/***********************************************************************************************************
*
*	Texture
*
*																				����ҁF������
*																				������F02/25
*
************************************************************************************************************
* �e�N�X�`���֘A
************************************************************************************************************/

#include"Main.h"

namespace Texture
{

	/*�e�N�X�`���ǂݍ���*/
	class Texture
	{
	private:

		bool mIsValid = false;
		ComPtr<ID3D12Resource> mResource{};

	private:

		const wstring GetWideString(crstring str);
		const wstring FileExtension(const wstring& path);
		cstring FileExtension(crstring path);

	public:

		inline cbool GetIsValid()const { return mIsValid; }
		cbool Load(crstring path);
		inline ID3D12Resource* GetResource() { return mResource.Get(); }
		D3D12_SHADER_RESOURCE_VIEW_DESC GetViewDesc();

	};

	cuint HANDLE_MAX = 512;

	/*�f�B�X�N���v�^�q�[�v�n���h��*/
	class DescriptorHandle
	{
	public:
		D3D12_CPU_DESCRIPTOR_HANDLE mHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE mHandleGPU;
	};

	/*�f�B�X�N���v�^�q�[�v*/
	class DescriptorHeap
	{
	private:

		bool mIsValid = false;						/*�����������ǂ���*/
		uint mIncrementSize = 0;					/*�C���N�������g�T�C�Y*/
		ComPtr<ID3D12DescriptorHeap> mHeap{};		/*�f�B�X�N���v�^�q�[�v�{��*/
		std::vector<DescriptorHandle*> mHandles{};	/*�o�^����Ă���n���h��*/

	public:

		DescriptorHeap();
		inline ID3D12DescriptorHeap* GetHeap() { return mHeap.Get(); }
		DescriptorHandle* Register(Texture* tex);
		inline cuint GetIncrementSize()const { return mIncrementSize; }
		inline const size_t GetHandleSize() { return mHandles.size(); }

	};

	struct TextureData 
	{
		Texture Texture;
		string File;
	};

	/*�e�N�X�`���ǂݍ��݊Ǘ��N���X*/
	class Manager
	{
	private:

		/*�e�N�X�`���f�[�^*/
		std::unordered_map<uint, TextureData> mTexture{};
		/*�Ǘ��p�C���f�b�N�X*/
		uint mIndex = 0;

	public:

		inline uint LoadTexture(crstring path) {

			for (auto& data : mTexture) { if (data.second.File == path)return data.first; }

			Texture tex;
			assert(tex.Load(path));
			mTexture[mIndex].Texture = tex;
			mTexture[mIndex].File = path;

			return mIndex++;

		}

		inline void Uninit() { mTexture.clear(); }

		inline ID3D12Resource* GetTexture(cruint id) {
			if (id < 0)return nullptr;
			return mTexture[id].Texture.GetResource();
		}

	};	

}