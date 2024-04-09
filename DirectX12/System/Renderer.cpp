
#include "Main.h"
#include "Renderer.h"
#include "System\Window.h"

uint								Renderer::mCurrentBackBufferIndex{};
ComPtr<ID3D12Device6>				Renderer::mDevice{};
ComPtr<ID3D12CommandQueue>			Renderer::mQueue{};
ComPtr<IDXGISwapChain3>				Renderer::mSwapChain{};
ComPtr<ID3D12CommandAllocator>		Renderer::mAllocator[FreamBufferCount]{};
ComPtr<ID3D12GraphicsCommandList>	Renderer::mCommandList{};
HANDLE								Renderer::mFenceEvent{};
ComPtr<ID3D12Fence>					Renderer::mFence{};
UINT64								Renderer::mFenceValue[FreamBufferCount]{};
D3D12_VIEWPORT						Renderer::mViewport{};
D3D12_RECT							Renderer::mScissor{};
uint								Renderer::mRtvDescriptorSize{};
ComPtr<ID3D12DescriptorHeap>		Renderer::mRtvHeap{};
ComPtr<ID3D12Resource>				Renderer::mRenderTargets[FreamBufferCount]{};
uint								Renderer::mDsvDescriptorSize{};
ComPtr<ID3D12DescriptorHeap>		Renderer::mDsvHeap{};
ComPtr<ID3D12Resource>				Renderer::mDepthStencilBuffer{};
ID3D12Resource*                     Renderer::mCurrentRenderTarget{};
RootSignature*                      Renderer::mRootSignature{};
PipeLineState*                      Renderer::mPipelineState{};
Buffer::Constant*                   Renderer::mMatrix{};

const D3D12_INPUT_ELEMENT_DESC Vertex3D::InputElements[] =
{
    { "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,     0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,     0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    { "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    { "TANGENT",     0, DXGI_FORMAT_R32G32B32_FLOAT,     0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,        0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
};

const D3D12_INPUT_LAYOUT_DESC Vertex3D::InputLayout =
{
    Vertex3D::InputElements,
    static_cast<UINT>(std::size(Vertex3D::InputElements))
};

/*******************************************************************
* デバイスの生成
********************************************************************/
cbool Renderer::CreateDevice()
{
    auto hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(mDevice.ReleaseAndGetAddressOf()));
    return hr == S_OK;
}

/*******************************************************************
* コマンドキュー生成
********************************************************************/
cbool Renderer::CreateCommandQueue()
{

    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    auto hr = mDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(mQueue.ReleaseAndGetAddressOf()));
    
    return SUCCEEDED(hr);

}

/*******************************************************************
* スワップチェーンの生成
********************************************************************/
cbool Renderer::CreateSwapChain()
{
    
    /*DXGIファクトリーの生成*/
    IDXGIFactory4* factory = nullptr;
    auto hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
    if (FAILED(hr))return false;
    
    /*スワップチェーンの生成*/
    DXGI_SWAP_CHAIN_DESC desc = {};
    desc.BufferDesc.Width = static_cast<UINT>(WindowWidth);
    desc.BufferDesc.Height = static_cast<UINT>(WindowHeight);
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = FreamBufferCount;
    desc.OutputWindow = Window::Device::GetWindow();
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    /*スワップチェーンの生成*/
    IDXGISwapChain* swapchain{};
    hr = factory->CreateSwapChain(mQueue.Get(), &desc, &swapchain);
    if (FAILED(hr)) {
        factory->Release();
        return false;
    }

    /*IDXGISwapChain3を取得*/
    hr = swapchain->QueryInterface(IID_PPV_ARGS(mSwapChain.ReleaseAndGetAddressOf()));
    if (FAILED(hr)) {
        factory->Release();
        swapchain->Release();
        return false;
    }

    /*バックバッファ番号を取得*/
    mCurrentBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();

    factory->Release();
    swapchain->Release();

    return true;

}

/*******************************************************************
* コマンドリスト・コマンドアロケーターの生成
********************************************************************/
cbool Renderer::CreateCommandList()
{
    
    /*コマンドアロケーターの作成*/
    HRESULT hr;
    for (int i = 0; i < FreamBufferCount; i++) {

        hr = mDevice->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            IID_PPV_ARGS(mAllocator[i].ReleaseAndGetAddressOf())
        );

    }

    if (FAILED(hr))return false;

    /*コマンドリストの生成*/
    hr = mDevice->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        mAllocator[mCurrentBackBufferIndex].Get(),
        nullptr,
        IID_PPV_ARGS(&mCommandList)
    );

    if (FAILED(hr))return false;

    /*コマンドリストは開かれている状態で作成されるので、いったん閉じる*/
    mCommandList->Close();

    return true;

}

/*******************************************************************
* フェンスの生成
********************************************************************/
cbool Renderer::CreateFence()
{
    
    for (auto i = 0u; i < FreamBufferCount; i++) { mFenceValue[i] = 0; }

    auto hr = mDevice->CreateFence(
        0,
        D3D12_FENCE_FLAG_NONE,
        IID_PPV_ARGS(mFence.ReleaseAndGetAddressOf())
    );

    if (FAILED(hr))return false;

    mFenceValue[mCurrentBackBufferIndex]++;

    /*同期を行う時のイベントハンドラを作成する*/
    mFenceEvent = CreateEvent(nullptr, false, false, nullptr);
    return mFenceEvent != nullptr;

}

/*******************************************************************
* ビューポートの生成
********************************************************************/
void Renderer::CreateViewPort()
{

    mViewport.TopLeftX = 0;
    mViewport.TopLeftY = 0;
    mViewport.Width = WindowWidth;
    mViewport.Height = WindowHeight;
    mViewport.MinDepth = 0.0f;
    mViewport.MaxDepth = 1.0f;

}

/*******************************************************************
* シザー矩形の生成
********************************************************************/
void Renderer::CreateScissorRect()
{

    mScissor.left = 0;
    mScissor.right = static_cast<LONG>(WindowWidth);
    mScissor.top = 0;
    mScissor.bottom = static_cast<LONG>(WindowHeight);

}

/*******************************************************************
* レンダーターゲットの生成
********************************************************************/
cbool Renderer::CreateRenderTarget()
{
    
    /*RTV用のディスクリプタヒープを作成する*/
    D3D12_DESCRIPTOR_HEAP_DESC desc{};
    desc.NumDescriptors = FreamBufferCount;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    auto hr = mDevice->CreateDescriptorHeap(
        &desc,
        IID_PPV_ARGS(mRtvHeap.ReleaseAndGetAddressOf())
    );

    if (FAILED(hr))return false;

    /*ディスクリプタのサイズを取得*/
    mRtvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = mRtvHeap->GetCPUDescriptorHandleForHeapStart();

    for (uint i = 0; i < FreamBufferCount; i++) {

        mSwapChain->GetBuffer(i, IID_PPV_ARGS(mRenderTargets[i].ReleaseAndGetAddressOf()));
        mDevice->CreateRenderTargetView(mRenderTargets[i].Get(), nullptr, rtvHandle);
        rtvHandle.ptr += mRtvDescriptorSize;

    }

    return true;

}

/*******************************************************************
* 深度ステンシルバッファの生成
********************************************************************/
cbool Renderer::CretaeDepthStenccil()
{
    
    /*DSV用のディスクリプタヒープを作成する*/
    D3D12_DESCRIPTOR_HEAP_DESC heap{};
    heap.NumDescriptors = 1;
    heap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    heap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    auto hr = mDevice->CreateDescriptorHeap(&heap, IID_PPV_ARGS(mDsvHeap.ReleaseAndGetAddressOf()));
    if (FAILED(hr))return false;

    /*ディスクリプタのサイズを取得*/
    mDsvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

    D3D12_CLEAR_VALUE dsvClearValue{};
    dsvClearValue.Format = DXGI_FORMAT_D32_FLOAT;
    dsvClearValue.DepthStencil.Depth = 1.0f;
    dsvClearValue.DepthStencil.Stencil = 0;

    auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC resourceDesc(
        D3D12_RESOURCE_DIMENSION_TEXTURE2D,
        0,
        static_cast<UINT64>(WindowWidth),
        static_cast<UINT>(WindowHeight),
        1,
        1,
        DXGI_FORMAT_D32_FLOAT,
        1,
        0,
        D3D12_TEXTURE_LAYOUT_UNKNOWN,
        D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);
    hr = mDevice->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &dsvClearValue,
        IID_PPV_ARGS(mDepthStencilBuffer.ReleaseAndGetAddressOf())
    );

    if (FAILED(hr))return false;

    /*ディスクリプタを作成*/
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDsvHeap->GetCPUDescriptorHandleForHeapStart();

    mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, dsvHandle);

    return true;

}

/*******************************************************************
* 描画完了を待つ処理
********************************************************************/
void Renderer::WaitRender()
{

    /*秒が終了待ち*/
    const UINT64 fenceValue = mFenceValue[mCurrentBackBufferIndex];
    mQueue->Signal(mFence.Get(), fenceValue);
    mFenceValue[mCurrentBackBufferIndex]++;

    /*次のフレーム描画準備がまだであれば待機*/
    if (mFence->GetCompletedValue() < fenceValue) {

        /*完了にイベントを設定*/
        auto hr = mFence->SetEventOnCompletion(fenceValue, mFenceEvent);
        if (FAILED(hr))return;

        /*待機処理*/
        if (WAIT_OBJECT_0 != WaitForSingleObjectEx(mFenceEvent, INFINITE, false))return;

    }

}

/*******************************************************************
* デバイス取得
********************************************************************/
ID3D12Device6* Renderer::GetDevice()
{
    return mDevice.Get();
}

/*******************************************************************
* コマンドリスト取得
********************************************************************/
ID3D12GraphicsCommandList* Renderer::GetCommandList()
{
    return mCommandList.Get();
}

/*******************************************************************
* バッグバッファインデックス
********************************************************************/
uint Renderer::CurrentBackBufferIndex()
{
    return mCurrentBackBufferIndex;
}

/*******************************************************************
* ワールドマトリックス
********************************************************************/
void Renderer::SetWorldMatrix(const XMMATRIX* world)
{
    auto ptr = reinterpret_cast<Matrix*>(mMatrix->GetPtr());
    ptr->World = (*world);
    mCommandList->SetGraphicsRootConstantBufferView(0, mMatrix->GetAddress());
}

/*******************************************************************
* ビューマトリックス
********************************************************************/
void Renderer::SetViewMatrix(const XMMATRIX* view)
{
    auto ptr = reinterpret_cast<Matrix*>(mMatrix->GetPtr());
    ptr->View = (*view);
    mCommandList->SetGraphicsRootConstantBufferView(0, mMatrix->GetAddress());
}

/*******************************************************************
* プロジェクションマトリックス
********************************************************************/
void Renderer::SetProjectionMatrix(const XMMATRIX* projection)
{
    auto ptr = reinterpret_cast<Matrix*>(mMatrix->GetPtr());
    ptr->Projection = (*projection);
    mCommandList->SetGraphicsRootConstantBufferView(0, mMatrix->GetAddress());
}

/*******************************************************************
* サンプラーステート生成
********************************************************************/
cbool Renderer::CreateSampler()
{
    
    D3D12_DESCRIPTOR_HEAP_DESC desc{};
    ZeroMemory(&desc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));

    ID3D12DescriptorHeap* smp{};

    desc.NumDescriptors = 1;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc.NodeMask = 0;
    if (FAILED(mDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&smp))))return false;

    D3D12_CPU_DESCRIPTOR_HANDLE handle;
    D3D12_SAMPLER_DESC descSamp;
    ZeroMemory(&descSamp, sizeof(D3D12_SAMPLER_DESC));

    descSamp.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    descSamp.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    descSamp.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    descSamp.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    descSamp.MaxLOD = FLT_MAX;
    descSamp.MinLOD = -FLT_MAX;
    descSamp.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    descSamp.MipLODBias = 0.0F;
    descSamp.MaxAnisotropy = 0;

    handle = smp->GetCPUDescriptorHandleForHeapStart();
    mDevice->CreateSampler(&descSamp, handle);

    return true;

}

/*******************************************************************
* 2Dマトリックス設定
********************************************************************/
void Renderer::SetWorldViewProjection2D()
{

    /*XMMATRIX world = DirectX::XMMatrixIdentity();
    world = DirectX::XMMatrixTranspose(world);

    XMMATRIX view = DirectX::XMMatrixIdentity();
    view = DirectX::XMMatrixTranspose(view);

    XMMATRIX projection = DirectX::XMMatrixIdentity();
    projection = DirectX::XMMatrixTranspose(projection);

    auto ptr = mMatrix[mCurrentBackBufferIndex]->GetPtr<Matrix>();
    ptr->World = world;
    ptr->View = view;
    ptr->Projection = projection;

    mCommandList->SetGraphicsRootConstantBufferView(0, mMatrix[mCurrentBackBufferIndex]->GetAddress());*/

}

/*******************************************************************
* 初期化
********************************************************************/
void Renderer::MasterInit()
{

    assert(CreateDevice());
    assert(CreateCommandQueue());
    assert(CreateSwapChain());
    assert(CreateCommandList());
    assert(CreateFence());
    CreateViewPort();
    CreateScissorRect();
    assert(CreateRenderTarget());
    assert(CretaeDepthStenccil());

    mMatrix = new Buffer::Constant(sizeof(Matrix));

    Matrix* ptr = mMatrix->GetPtr<Matrix>();
    ptr->World = DirectX::XMMatrixIdentity();
    ptr->View = DirectX::XMMatrixLookAtRH(
        XMVectorSet(0.0f, 120.0f, 300.0f, 0.0f), 
        XMVectorSet(0.0f, 100.0f, 0.0f, 0.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
    ptr->Projection = DirectX::XMMatrixPerspectiveFovRH(
        DirectX::XMConvertToRadians(100.0f),
        WindowWidth / WindowHeight,
        0.3f, 
        1000.0f);

    /*ptr->View = DirectX::XMMatrixLookAtRH(
        XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
    ptr->Projection = DirectX::XMMatrixPerspectiveFovRH(
        DirectX::XMConvertToRadians(37.5f),
        WindowWidth / WindowHeight,
        0.3f,
        1000.0f);*/

    mCommandList->SetGraphicsRootConstantBufferView(0, mMatrix->GetAddress());

}

/*******************************************************************
* 終了処理
********************************************************************/
void Renderer::MasterUninit()
{

    if (mRootSignature) delete mRootSignature;
    if (mPipelineState)delete mPipelineState;
    if (mMatrix)delete mMatrix;

}

/*******************************************************************
* 描画開始処理
********************************************************************/
void Renderer::Begin()
{

    /*現在のレンダーターゲットを更新*/
    mCurrentRenderTarget = mRenderTargets[mCurrentBackBufferIndex].Get();

    /*コマンドを初期化してためる準備をする*/
    mAllocator[mCurrentBackBufferIndex]->Reset();
    mCommandList->Reset(mAllocator[mCurrentBackBufferIndex].Get(), nullptr);

    /*ビューポートとシザー矩形を設定*/
    mCommandList->RSSetViewports(1, &mViewport);
    mCommandList->RSSetScissorRects(1, &mScissor);

    /*現在のフレームのレンダーターゲットビューのディスクリプタヒープの開始アドレスを取得*/
    auto currentRtvHandle = mRtvHeap->GetCPUDescriptorHandleForHeapStart();
    currentRtvHandle.ptr += mCurrentBackBufferIndex * mRtvDescriptorSize;

    /*深度ステンシルバッファのディスクリプタヒープの開始アドレスを取得*/
    auto currentDsvHandle = mDsvHeap->GetCPUDescriptorHandleForHeapStart();

    /*レンダーターゲットが使用可能になるまで待つ*/
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mCurrentRenderTarget, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    mCommandList->ResourceBarrier(1, &barrier);

    /*レンダーターゲットを設定*/
    mCommandList->OMSetRenderTargets(1, &currentRtvHandle, false, &currentDsvHandle);

    /*レンダーターゲットをクリア*/
    cfloat ClearColor[4] = { 0.3f,0.3f,0.3f,0.0f };
    mCommandList->ClearRenderTargetView(currentRtvHandle, ClearColor, 0, nullptr);

    /*深度ステンシルビューをクリア*/
    mCommandList->ClearDepthStencilView(currentDsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

/*******************************************************************
* 描画終了処理
********************************************************************/
void Renderer::End()
{

    /*レンダーターゲットに書き込みを終わるまで待つ*/
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mCurrentRenderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    mCommandList->ResourceBarrier(1, &barrier);

    /*コマンドの記録を終了*/
    mCommandList->Close();

    /*コマンドを実行*/
    ID3D12CommandList* command[] = { mCommandList.Get() };
    mQueue->ExecuteCommandLists(1, command);

    /*スワップチェーンを切り替え*/
    mSwapChain->Present(1, 0);

    /*描画完了を待つ*/
    WaitRender();

    /*バックバッファ番号更新*/
    mCurrentBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();

}

/*******************************************************************
* ルートシグニチャコンストラクタ
********************************************************************/
RootSignature::RootSignature()
{

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

    mIsValid = true;

}

/*******************************************************************
* パイプラインラインステートコンストラクタ
********************************************************************/
PipeLineState::PipeLineState()
{

    /*パイプラインステートの設定*/
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

}

/*******************************************************************
* セットレイアウト
********************************************************************/
void PipeLineState::SetInputLayout(D3D12_INPUT_LAYOUT_DESC layout)
{
    mDesc.InputLayout = layout;
}

/*******************************************************************
* セットルートシグニチャ
********************************************************************/
void PipeLineState::SetRootSignature(ID3D12RootSignature* rootSignature)
{
    mDesc.pRootSignature = rootSignature;
}

/*******************************************************************
* セットバーテックスシェーダー
********************************************************************/
void PipeLineState::CreateVertexShader(const wstring& path)
{

    /*頂点シェーダー読み込み*/
    if (FAILED(D3DReadFileToBlob(path.c_str(), mVsBlob.GetAddressOf())))return;
    
    mDesc.VS = CD3DX12_SHADER_BYTECODE(mVsBlob.Get());

}

/*******************************************************************
* セットピクセルシェーダー
********************************************************************/
void PipeLineState::CreatePixelShader(const wstring& path)
{

    /*ピクセルシェーダー読み込み*/
    if (FAILED(D3DReadFileToBlob(path.c_str(), mPsBlob.GetAddressOf())))return;
    
    mDesc.PS = CD3DX12_SHADER_BYTECODE(mPsBlob.Get());

}

/*******************************************************************
* 生成
********************************************************************/
void PipeLineState::Create()
{

    mDesc.InputLayout = Vertex3D::InputLayout;
    auto hr = Renderer::GetDevice()->CreateGraphicsPipelineState(
        &mDesc,
        IID_PPV_ARGS(&mPipeLineState));
    if (FAILED(hr))return;
    
    mIsValid = true;

}
