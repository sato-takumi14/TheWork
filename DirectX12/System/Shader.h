#pragma once

/***********************************************************************************************************
*
*	Shader
*
*																				����ҁF������
*																				������F02/24
*
************************************************************************************************************
* �V�F�[�_�[�t�@�C����ǂݍ��ނ��߂̃N���X
************************************************************************************************************/

#include"Main.h"

class Shader
{
private:

	/*���[�g�V�O�l�`��*/
	bool mIsRootSignatureValid = false;					/*���[�g�V�O�l�`���̐����𐬌��������ǂ���*/
	ComPtr<ID3D12RootSignature> mRootSignature{};		/*���[�g�V�O�l�`��*/

	/*�p�C�v���C��*/
	bool mIsPipelineValid = false;						/*�p�C�v���C���̐����𐬌��������ǂ���*/
	D3D12_GRAPHICS_PIPELINE_STATE_DESC mDesc{};			/*�p�C�v���C���X�e�[�g�̐ݒ�*/
	ComPtr<ID3D12PipelineState> mPipelineState{};		/*�p�C�v���C���X�e�[�g*/
	ComPtr<ID3DBlob> mVsBlob{};							/*���_�V�F�[�_�[*/
	ComPtr<ID3DBlob> mPsBlob{};							/*�s�N�Z���V�F�[�_�[*/

public:

	Shader();
	Shader(const wstring& vertex, const wstring& pixel);

	/*���[�g�V�O�l�`���̐����𐬌��������ǂ������擾*/
	inline cbool GetIsRootSignatureValid()const { return mIsRootSignatureValid; }
	/*�p�C�v���C���̐����𐬌��������ǂ������擾*/
	inline cbool GetIsPipelineValid()const { return mIsPipelineValid; }
	/*���������������ǂ���*/
	inline cbool AllValid()const { return mIsRootSignatureValid && mIsPipelineValid; }

	/*���[�g�V�O�l�`�����擾*/
	inline ID3D12RootSignature* GetRootSignature() { return mRootSignature.Get(); }
	/*���_�V�F�[�_�[�𐶐�*/
	void CreateVertexShader(const wstring& file);
	/*�s�N�Z���V�F�[�_�[�𐶐�*/
	void CreatePixelShader(const wstring& file);
	/*�p�C�v���C���𐶐�*/
	void Create();

	/*�p�C�v���C�����擾*/
	inline ID3D12PipelineState* GetPipelineState() { return mPipelineState.Get(); }

};
