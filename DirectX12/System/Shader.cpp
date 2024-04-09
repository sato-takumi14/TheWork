#include "Main.h"
#include "Shader.h"
#include"System\Renderer.h"

/*******************************************************************
* コンストラクタ
********************************************************************/
Shader::Shader()
{

	/*ルートシグネチャを生成*/
	if (not mIsRootSignatureValid) {
        /*アプリケーションの入力アセンブラを使用する*/
        auto flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        /*ドメインシェーダーのルートシグネチャへのアクセスを拒否する*/
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
        /*ハルシェーダーのルートシグネチャへのアクセスを拒否する*/
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
        /*ジオメトリシェーダーのルートシグネチャへのアクセスを拒否する*/
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

        CD3DX12_ROOT_PARAMETER rootParam[2]{};
        rootParam[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

        CD3DX12_DESCRIPTOR_RANGE tableRange[1]{};
        tableRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
        rootParam[1].InitAsDescriptorTable(static_cast<UINT>(std::size(tableRange)), tableRange, D3D12_SHADER_VISIBILITY_ALL);

        /*スタティックサンプラーの設定*/
        auto sampler = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

        /*ルートシグニチャの設定（設定したいルートパラメーターとスタティックサンプラーを入れる）*/
        D3D12_ROOT_SIGNATURE_DESC desc = {};
        /*ルートパラメーターの個数を入れる*/
        desc.NumParameters = static_cast<UINT>(std::size(rootParam));
        /*サンプラーの個数を入れる*/
        desc.NumStaticSamplers = 1;
        /*ルートパラメーターのポインタを入れる*/
        desc.pParameters = rootParam;
        /*サンプラーのポインタを入れる*/
        desc.pStaticSamplers = &sampler;
        /*フラグを設定*/
        desc.Flags = flag;

        ComPtr<ID3DBlob> blob;
        ComPtr<ID3DBlob> error;

        /*シリアライズ*/
        auto hr = D3D12SerializeRootSignature(
            &desc,
            D3D_ROOT_SIGNATURE_VERSION_1_0,
            blob.GetAddressOf(),
            error.GetAddressOf()
        );
        if (FAILED(hr))return;

        /*ルートシグネチャ生成*/
        hr = Renderer::GetDevice()->CreateRootSignature(
            0,                                          /*GPUが複数ある場合のノードマスク（今回は1個しかない想定）*/
            blob->GetBufferPointer(),                   /*シリアリズしたデータのポインタ*/
            blob->GetBufferSize(),                      /*シリアライズしたデータのサイズ*/
            IID_PPV_ARGS(mRootSignature.GetAddressOf()) /*ルートシグニチャ格納先ポインタ*/
        );

        if (FAILED(hr))return;

        mIsRootSignatureValid = true;
	}

    /*パイプラインステート生成*/
    if (not mIsPipelineValid) {
        /*ラスタライザーはデフォルト*/
        mDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        /*カリング無*/
        mDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
        /*ブレンドステートもデフォルト*/
        mDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        /*震度ステンシルはデフォルト*/
        mDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        mDesc.SampleMask = UINT_MAX;
        /*三角形を描画*/
        mDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        /*描画対象は1*/
        mDesc.NumRenderTargets = 1;
        mDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        mDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
        /*サンプラーは1*/
        mDesc.SampleDesc.Count = 1;
        /*ルートシグネチャをセット*/
        //mDesc.pRootSignature = mRootSignature.Get();
    }

}

/*******************************************************************
* シェーダー読み込みを含めたコンストラクタ
********************************************************************/
Shader::Shader(const wstring& vertex, const wstring& pixel)
{

    /*ルートシグネチャを生成*/
    if (not mIsRootSignatureValid) {
        /*アプリケーションの入力アセンブラを使用する*/
        auto flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        /*ドメインシェーダーのルートシグネチャへのアクセスを拒否する*/
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
        /*ハルシェーダーのルートシグネチャへのアクセスを拒否する*/
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
        /*ジオメトリシェーダーのルートシグネチャへのアクセスを拒否する*/
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

        CD3DX12_ROOT_PARAMETER rootParam[1]{};
        rootParam[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

        /*スタティックサンプラーの設定*/
        auto sampler = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

        /*ルートシグニチャの設定（設定したいルートパラメーターとスタティックサンプラーを入れる）*/
        D3D12_ROOT_SIGNATURE_DESC desc = {};
        /*ルートパラメーターの個数を入れる*/
        desc.NumParameters = static_cast<UINT>(std::size(rootParam));
        /*サンプラーの個数を入れる*/
        desc.NumStaticSamplers = 1;
        /*ルートパラメーターのポインタを入れる*/
        desc.pParameters = rootParam;
        /*サンプラーのポインタを入れる*/
        desc.pStaticSamplers = &sampler;
        /*フラグを設定*/
        desc.Flags = flag;

        ComPtr<ID3DBlob> blob;
        ComPtr<ID3DBlob> error;

        /*シリアライズ*/
        auto hr = D3D12SerializeRootSignature(
            &desc,
            D3D_ROOT_SIGNATURE_VERSION_1_0,
            blob.GetAddressOf(),
            error.GetAddressOf()
        );
        if (FAILED(hr))return;

        /*ルートシグネチャ生成*/
        hr = Renderer::GetDevice()->CreateRootSignature(
            0,                                          /*GPUが複数ある場合のノードマスク（今回は1個しかない想定）*/
            blob->GetBufferPointer(),                   /*シリアリズしたデータのポインタ*/
            blob->GetBufferSize(),                      /*シリアライズしたデータのサイズ*/
            IID_PPV_ARGS(mRootSignature.GetAddressOf()) /*ルートシグニチャ格納先ポインタ*/
        );

        if (FAILED(hr))return;

        mIsRootSignatureValid = true;
    }

    /*パイプラインステート生成*/
    if (not mIsPipelineValid) {
        /*ラスタライザーはデフォルト*/
        mDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        /*カリング無*/
        mDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
        /*ブレンドステートもデフォルト*/
        mDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        /*震度ステンシルはデフォルト*/
        mDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        mDesc.SampleMask = UINT_MAX;
        /*三角形を描画*/
        mDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        /*描画対象は1*/
        mDesc.NumRenderTargets = 1;
        mDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        mDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
        /*サンプラーは1*/
        mDesc.SampleDesc.Count = 1;
        /*ルートシグネチャをセット*/
        //mDesc.pRootSignature = mRootSignature.Get();
    }

	CreateVertexShader(vertex);
	CreatePixelShader(pixel);
	Create();

}

/*******************************************************************
* 頂点シェーダーを生成
********************************************************************/
void Shader::CreateVertexShader(const wstring& file)
{

    /*頂点シェーダー読み込み*/
    if (FAILED(D3DReadFileToBlob(file.c_str(), mVsBlob.GetAddressOf())))return;

    mDesc.VS = CD3DX12_SHADER_BYTECODE(mVsBlob.Get());

}

/*******************************************************************
* ピクセルシェーダー生成
********************************************************************/
void Shader::CreatePixelShader(const wstring& file)
{

    /*ピクセルシェーダー読み込み*/
    if (FAILED(D3DReadFileToBlob(file.c_str(), mPsBlob.GetAddressOf())))return;

    mDesc.PS = CD3DX12_SHADER_BYTECODE(mPsBlob.Get());

}

/*******************************************************************
* パイプラインステートを生成
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
