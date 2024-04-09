#pragma once

/***********************************************************************************************************
*
*	Buffer
*
*																				����ҁF������
*																				������F02/24
*
************************************************************************************************************
* �o�b�t�@�֘A���܂Ƃ߂�
************************************************************************************************************/

#include"Main.h"

/*�o�b�t�@�֘A*/
namespace Buffer
{

	/*�o�b�t�@�C���^�[�t�F�[�X*/
	class BufferInterface
	{
	protected:

		bool mIsValid = false;					/*�o�b�t�@�𐶐�������*/
		ComPtr<ID3D12Resource> mBuffer{};		/*�o�b�t�@*/

	public:

		inline cbool GetIsValid()const { return mIsValid; }		/*�o�b�t�@�𐶐����������擾*/
		inline ID3D12Resource* GetBuffer() { return mBuffer.Get(); }

	};

	/*���_�o�b�t�@*/
	class Vertex : public BufferInterface
	{
	private:

		/*�o�b�t�@�r���[���*/
		D3D12_VERTEX_BUFFER_VIEW mView{};

	public:

		inline Vertex() {}
		inline Vertex(
			const size_t& size,
			const size_t& stride,
			const void* data) {
			assert(Vertex::Create(size, stride, data));
		}

		/*�o�b�t�@����*/
		cbool Create(
			const size_t& size,
			const size_t& stride,
			const void* data);

		/*�o�b�t�@�r���[���擾*/
		inline const D3D12_VERTEX_BUFFER_VIEW* GetView() { return &mView; }

	};

	/*�C���f�b�N�X�o�b�t�@*/
	class Index : public BufferInterface
	{
	private:

		/*�o�b�t�@�r���[���*/
		D3D12_INDEX_BUFFER_VIEW mView{};

	public:

		inline Index() {}
		inline Index(
			const size_t& size,
			const uint32_t* data = nullptr) {
			assert(Index::Create(size, data));
		}

		/*�o�b�t�@�𐶐�*/
		cbool Create(
			const size_t& size,
			const uint32_t* data = nullptr);

		/*�o�b�t�@�r���[���擾*/
		inline const D3D12_INDEX_BUFFER_VIEW* GetView() { return &mView; }

	};

	/*�R���X�^���g�o�b�t�@*/
	class Constant : public BufferInterface
	{
	private:

		D3D12_CONSTANT_BUFFER_VIEW_DESC mDesc{};	/*�萔�o�b�t�@�r���[�̐ݒ�*/
		void* mMappedPtr{};							/*�f�[�^*/

	public:

		inline Constant() {}
		inline Constant(const size_t& size) { assert(Constant::Create(size)); }

		/*�o�b�t�@����*/
		cbool Create(const size_t& size);

		/*�A�h���X�擾*/
		inline D3D12_GPU_VIRTUAL_ADDRESS GetAddress()const { return mDesc.BufferLocation; }

		/*�o�b�t�@�ݒ�擾*/
		inline D3D12_CONSTANT_BUFFER_VIEW_DESC GetViewDesc() { return mDesc; }

		/*�o�b�t�@�f�[�^�擾*/
		inline void* GetPtr()const { return mMappedPtr; }

		/*�e���v���[�g�֐��Ńo�b�t�@�f�[�^���擾*/
		template <class T>
		inline T* GetPtr() { return reinterpret_cast<T*>(GetPtr()); }

	};

}