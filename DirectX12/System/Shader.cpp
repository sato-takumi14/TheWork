#include "Main.h"
#include "Shader.h"
#include"System\Renderer.h"

/*******************************************************************
* �R���X�g���N�^
********************************************************************/
Shader::Shader()
{

	/*���[�g�V�O�l�`���𐶐�*/
	if (not mIsRootSignatureValid) {
        /*�A�v���P�[�V�����̓��̓A�Z���u�����g�p����*/
        auto flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        /*�h���C���V�F�[�_�[�̃��[�g�V�O�l�`���ւ̃A�N�Z�X�����ۂ���*/
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
        /*�n���V�F�[�_�[�̃��[�g�V�O�l�`���ւ̃A�N�Z�X�����ۂ���*/
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
        /*�W�I���g���V�F�[�_�[�̃��[�g�V�O�l�`���ւ̃A�N�Z�X�����ۂ���*/
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

        CD3DX12_ROOT_PARAMETER rootParam[2]{};
        rootParam[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

        CD3DX12_DESCRIPTOR_RANGE tableRange[1]{};
        tableRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
        rootParam[1].InitAsDescriptorTable(static_cast<UINT>(std::size(tableRange)), tableRange, D3D12_SHADER_VISIBILITY_ALL);

        /*�X�^�e�B�b�N�T���v���[�̐ݒ�*/
        auto sampler = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

        /*���[�g�V�O�j�`���̐ݒ�i�ݒ肵�������[�g�p�����[�^�[�ƃX�^�e�B�b�N�T���v���[������j*/
        D3D12_ROOT_SIGNATURE_DESC desc = {};
        /*���[�g�p�����[�^�[�̌�������*/
        desc.NumParameters = static_cast<UINT>(std::size(rootParam));
        /*�T���v���[�̌�������*/
        desc.NumStaticSamplers = 1;
        /*���[�g�p�����[�^�[�̃|�C���^������*/
        desc.pParameters = rootParam;
        /*�T���v���[�̃|�C���^������*/
        desc.pStaticSamplers = &sampler;
        /*�t���O��ݒ�*/
        desc.Flags = flag;

        ComPtr<ID3DBlob> blob;
        ComPtr<ID3DBlob> error;

        /*�V���A���C�Y*/
        auto hr = D3D12SerializeRootSignature(
            &desc,
            D3D_ROOT_SIGNATURE_VERSION_1_0,
            blob.GetAddressOf(),
            error.GetAddressOf()
        );
        if (FAILED(hr))return;

        /*���[�g�V�O�l�`������*/
        hr = Renderer::GetDevice()->CreateRootSignature(
            0,                                          /*GPU����������ꍇ�̃m�[�h�}�X�N�i�����1�����Ȃ��z��j*/
            blob->GetBufferPointer(),                   /*�V���A���Y�����f�[�^�̃|�C���^*/
            blob->GetBufferSize(),                      /*�V���A���C�Y�����f�[�^�̃T�C�Y*/
            IID_PPV_ARGS(mRootSignature.GetAddressOf()) /*���[�g�V�O�j�`���i�[��|�C���^*/
        );

        if (FAILED(hr))return;

        mIsRootSignatureValid = true;
	}

    /*�p�C�v���C���X�e�[�g����*/
    if (not mIsPipelineValid) {
        /*���X�^���C�U�[�̓f�t�H���g*/
        mDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        /*�J�����O��*/
        mDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
        /*�u�����h�X�e�[�g���f�t�H���g*/
        mDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        /*�k�x�X�e���V���̓f�t�H���g*/
        mDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        mDesc.SampleMask = UINT_MAX;
        /*�O�p�`��`��*/
        mDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        /*�`��Ώۂ�1*/
        mDesc.NumRenderTargets = 1;
        mDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        mDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
        /*�T���v���[��1*/
        mDesc.SampleDesc.Count = 1;
        /*���[�g�V�O�l�`�����Z�b�g*/
        //mDesc.pRootSignature = mRootSignature.Get();
    }

}

/*******************************************************************
* �V�F�[�_�[�ǂݍ��݂��܂߂��R���X�g���N�^
********************************************************************/
Shader::Shader(const wstring& vertex, const wstring& pixel)
{

    /*���[�g�V�O�l�`���𐶐�*/
    if (not mIsRootSignatureValid) {
        /*�A�v���P�[�V�����̓��̓A�Z���u�����g�p����*/
        auto flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        /*�h���C���V�F�[�_�[�̃��[�g�V�O�l�`���ւ̃A�N�Z�X�����ۂ���*/
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
        /*�n���V�F�[�_�[�̃��[�g�V�O�l�`���ւ̃A�N�Z�X�����ۂ���*/
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
        /*�W�I���g���V�F�[�_�[�̃��[�g�V�O�l�`���ւ̃A�N�Z�X�����ۂ���*/
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

        CD3DX12_ROOT_PARAMETER rootParam[1]{};
        rootParam[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

        /*�X�^�e�B�b�N�T���v���[�̐ݒ�*/
        auto sampler = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

        /*���[�g�V�O�j�`���̐ݒ�i�ݒ肵�������[�g�p�����[�^�[�ƃX�^�e�B�b�N�T���v���[������j*/
        D3D12_ROOT_SIGNATURE_DESC desc = {};
        /*���[�g�p�����[�^�[�̌�������*/
        desc.NumParameters = static_cast<UINT>(std::size(rootParam));
        /*�T���v���[�̌�������*/
        desc.NumStaticSamplers = 1;
        /*���[�g�p�����[�^�[�̃|�C���^������*/
        desc.pParameters = rootParam;
        /*�T���v���[�̃|�C���^������*/
        desc.pStaticSamplers = &sampler;
        /*�t���O��ݒ�*/
        desc.Flags = flag;

        ComPtr<ID3DBlob> blob;
        ComPtr<ID3DBlob> error;

        /*�V���A���C�Y*/
        auto hr = D3D12SerializeRootSignature(
            &desc,
            D3D_ROOT_SIGNATURE_VERSION_1_0,
            blob.GetAddressOf(),
            error.GetAddressOf()
        );
        if (FAILED(hr))return;

        /*���[�g�V�O�l�`������*/
        hr = Renderer::GetDevice()->CreateRootSignature(
            0,                                          /*GPU����������ꍇ�̃m�[�h�}�X�N�i�����1�����Ȃ��z��j*/
            blob->GetBufferPointer(),                   /*�V���A���Y�����f�[�^�̃|�C���^*/
            blob->GetBufferSize(),                      /*�V���A���C�Y�����f�[�^�̃T�C�Y*/
            IID_PPV_ARGS(mRootSignature.GetAddressOf()) /*���[�g�V�O�j�`���i�[��|�C���^*/
        );

        if (FAILED(hr))return;

        mIsRootSignatureValid = true;
    }

    /*�p�C�v���C���X�e�[�g����*/
    if (not mIsPipelineValid) {
        /*���X�^���C�U�[�̓f�t�H���g*/
        mDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        /*�J�����O��*/
        mDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
        /*�u�����h�X�e�[�g���f�t�H���g*/
        mDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        /*�k�x�X�e���V���̓f�t�H���g*/
        mDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        mDesc.SampleMask = UINT_MAX;
        /*�O�p�`��`��*/
        mDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        /*�`��Ώۂ�1*/
        mDesc.NumRenderTargets = 1;
        mDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        mDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
        /*�T���v���[��1*/
        mDesc.SampleDesc.Count = 1;
        /*���[�g�V�O�l�`�����Z�b�g*/
        //mDesc.pRootSignature = mRootSignature.Get();
    }

	CreateVertexShader(vertex);
	CreatePixelShader(pixel);
	Create();

}

/*******************************************************************
* ���_�V�F�[�_�[�𐶐�
********************************************************************/
void Shader::CreateVertexShader(const wstring& file)
{

    /*���_�V�F�[�_�[�ǂݍ���*/
    if (FAILED(D3DReadFileToBlob(file.c_str(), mVsBlob.GetAddressOf())))return;

    mDesc.VS = CD3DX12_SHADER_BYTECODE(mVsBlob.Get());

}

/*******************************************************************
* �s�N�Z���V�F�[�_�[����
********************************************************************/
void Shader::CreatePixelShader(const wstring& file)
{

    /*�s�N�Z���V�F�[�_�[�ǂݍ���*/
    if (FAILED(D3DReadFileToBlob(file.c_str(), mPsBlob.GetAddressOf())))return;

    mDesc.PS = CD3DX12_SHADER_BYTECODE(mPsBlob.Get());

}

/*******************************************************************
* �p�C�v���C���X�e�[�g�𐶐�
********************************************************************/
void Shader::Create()
{

    mDesc.pRootSignature = mRootSignature.Get();
    mDesc.InputLayout = Vertex3D::InputLayout;
    if (FAILED(
        Renderer::GetDevice()->CreateGraphicsPipelineState(
        &mDesc,
        IID_PPV_ARGS(&mPipelineState))))return;

    mIsPipelineValid = true;

}
