#pragma once

/***********************************************************************************************************
*
*	Texture
*
*																				制作者：佐藤匠
*																				制作日：02/25
*
************************************************************************************************************
* テクスチャ関連
************************************************************************************************************/

#include"Main.h"

namespace Texture
{

	/*テクスチャ読み込み*/
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

	/*ディスクリプタヒープハンドル*/
	class DescriptorHandle
	{
	public:
		D3D12_CPU_DESCRIPTOR_HANDLE mHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE mHandleGPU;
	};

	/*ディスクリプタヒープ*/
	class DescriptorHeap
	{
	private:

		bool mIsValid = false;						/*生成したかどうか*/
		uint mIncrementSize = 0;					/*インクリメントサイズ*/
		ComPtr<ID3D12DescriptorHeap> mHeap{};		/*ディスクリプタヒープ本体*/
		std::vector<DescriptorHandle*> mHandles{};	/*登録されているハンドル*/

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

	/*テクスチャ読み込み管理クラス*/
	class Manager
	{
	private:

		/*テクスチャデータ*/
		std::unordered_map<uint, TextureData> mTexture{};
		/*管理用インデックス*/
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