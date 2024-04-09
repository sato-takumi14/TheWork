#pragma once

/***********************************************************************************************************
*
*	Buffer
*
*																				制作者：佐藤匠
*																				制作日：02/24
*
************************************************************************************************************
* バッファ関連をまとめた
************************************************************************************************************/

#include"Main.h"

/*バッファ関連*/
namespace Buffer
{

	/*バッファインターフェース*/
	class BufferInterface
	{
	protected:

		bool mIsValid = false;					/*バッファを生成したか*/
		ComPtr<ID3D12Resource> mBuffer{};		/*バッファ*/

	public:

		inline cbool GetIsValid()const { return mIsValid; }		/*バッファを生成したかを取得*/
		inline ID3D12Resource* GetBuffer() { return mBuffer.Get(); }

	};

	/*頂点バッファ*/
	class Vertex : public BufferInterface
	{
	private:

		/*バッファビュー情報*/
		D3D12_VERTEX_BUFFER_VIEW mView{};

	public:

		inline Vertex() {}
		inline Vertex(
			const size_t& size,
			const size_t& stride,
			const void* data) {
			assert(Vertex::Create(size, stride, data));
		}

		/*バッファ生成*/
		cbool Create(
			const size_t& size,
			const size_t& stride,
			const void* data);

		/*バッファビュー情報取得*/
		inline const D3D12_VERTEX_BUFFER_VIEW* GetView() { return &mView; }

	};

	/*インデックスバッファ*/
	class Index : public BufferInterface
	{
	private:

		/*バッファビュー情報*/
		D3D12_INDEX_BUFFER_VIEW mView{};

	public:

		inline Index() {}
		inline Index(
			const size_t& size,
			const uint32_t* data = nullptr) {
			assert(Index::Create(size, data));
		}

		/*バッファを生成*/
		cbool Create(
			const size_t& size,
			const uint32_t* data = nullptr);

		/*バッファビュー情報取得*/
		inline const D3D12_INDEX_BUFFER_VIEW* GetView() { return &mView; }

	};

	/*コンスタントバッファ*/
	class Constant : public BufferInterface
	{
	private:

		D3D12_CONSTANT_BUFFER_VIEW_DESC mDesc{};	/*定数バッファビューの設定*/
		void* mMappedPtr{};							/*データ*/

	public:

		inline Constant() {}
		inline Constant(const size_t& size) { assert(Constant::Create(size)); }

		/*バッファ生成*/
		cbool Create(const size_t& size);

		/*アドレス取得*/
		inline D3D12_GPU_VIRTUAL_ADDRESS GetAddress()const { return mDesc.BufferLocation; }

		/*バッファ設定取得*/
		inline D3D12_CONSTANT_BUFFER_VIEW_DESC GetViewDesc() { return mDesc; }

		/*バッファデータ取得*/
		inline void* GetPtr()const { return mMappedPtr; }

		/*テンプレート関数でバッファデータを取得*/
		template <class T>
		inline T* GetPtr() { return reinterpret_cast<T*>(GetPtr()); }

	};

}