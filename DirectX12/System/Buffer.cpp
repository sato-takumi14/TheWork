#include "Main.h"
#include "Buffer.h"
#include"System\Renderer.h"

cbool Buffer::Vertex::Create(const size_t& size, const size_t& stride, const void* data)
{

	if (mIsValid)return false;

	/*�q�[�v�v���p�e�B*/
	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	/*���\�[�X�̐ݒ�*/
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);

	/*���\�[�X�̐���*/
	if (FAILED(Renderer::GetDevice()->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(mBuffer.GetAddressOf())
	)))return false;

	/*���_�o�b�t�@�r���[�̐ݒ�*/
	mView.BufferLocation = mBuffer->GetGPUVirtualAddress();
	mView.SizeInBytes = static_cast<uint>(size);
	mView.StrideInBytes = static_cast<uint>(stride);

	/*�}�b�s���O*/
	if (data != nullptr) {

		void* ptr = nullptr;
		if (FAILED(mBuffer->Map(0, nullptr, &ptr)))return false;

		/*���_�f�[�^���}�b�s���O��ɐݒ�*/
		memcpy(ptr, data, size);

		/*�}�b�s���O����*/
		mBuffer->Unmap(0, nullptr);

	}

	mIsValid = true;

	return mIsValid;

}

cbool Buffer::Index::Create(const size_t& size, const uint32_t* data)
{

	if (mIsValid)return false;

	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(size);

	/*���\�[�X�𐶐�*/
	if (FAILED(
		Renderer::GetDevice()->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(mBuffer.GetAddressOf())
		)
	))return false;

	/*�C���f�b�N�X�o�b�t�@�̐ݒ�*/
	mView = {};
	mView.BufferLocation = mBuffer->GetGPUVirtualAddress();
	mView.Format = DXGI_FORMAT_R32_UINT;
	mView.SizeInBytes = static_cast<uint>(size);

	/*�}�b�s���O����*/
	if (data) {

		void* ptr = nullptr;
		if (FAILED(mBuffer->Map(0, nullptr, &ptr)))return false;

		/*�C���f�b�N�X�o�b�t�@���}�b�s���O��ɐݒ�*/
		memcpy(ptr, data, size);

		/*�}�b�s���O����*/
		mBuffer->Unmap(0, nullptr);

	}

	mIsValid = true;

	return mIsValid;

}

cbool Buffer::Constant::Create(const size_t& size)
{
	
	if (mIsValid)return false;

	size_t align = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
	/*align�ɐ؂�グ��*/
	UINT64 sizeAligend = (size + (align - 1)) & ~(align - 1);

	/*�q�[�v�v���p�e�B*/
	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	/*���\�[�X�̐ݒ�*/
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);

	/*���\�[�X�̐���*/
	if (FAILED(
		Renderer::GetDevice()->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(mBuffer.GetAddressOf())
		)
	))return false;

	if (FAILED(mBuffer->Map(0, nullptr, &mMappedPtr)))return false;

	mDesc = {};
	mDesc.BufferLocation = mBuffer->GetGPUVirtualAddress();
	mDesc.SizeInBytes = static_cast<uint>(sizeAligend);

	mIsValid = true;

	return mIsValid;

}
