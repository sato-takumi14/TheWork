
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
* �f�o�C�X�̐���
********************************************************************/
cbool Renderer::CreateDevice()
{
    auto hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(mDevice.ReleaseAndGetAddressOf()));
    return hr == S_OK;
}

/*******************************************************************
* �R�}���h�L���[����
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
* �X���b�v�`�F�[���̐���
********************************************************************/
cbool Renderer::CreateSwapChain()
{
    
    /*DXGI�t�@�N�g���[�̐���*/
    IDXGIFactory4* factory = nullptr;
    auto hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
    if (FAILED(hr))return false;
    
    /*�X���b�v�`�F�[���̐���*/
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

    /*�X���b�v�`�F�[���̐���*/
    IDXGISwapChain* swapchain{};
    hr = factory->CreateSwapChain(mQueue.Get(), &desc, &swapchain);
    if (FAILED(hr)) {
        factory->Release();
        return false;
    }

    /*IDXGISwapChain3���擾*/
    hr = swapchain->QueryInterface(IID_PPV_ARGS(mSwapChain.ReleaseAndGetAddressOf()));
    if (FAILED(hr)) {
        factory->Release();
        swapchain->Release();
        return false;
    }

    /*�o�b�N�o�b�t�@�ԍ����擾*/
    mCurrentBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();

    factory->Release();
    swapchain->Release();

    return true;

}

/*******************************************************************
* �R�}���h���X�g�E�R�}���h�A���P�[�^�[�̐���
********************************************************************/
cbool Renderer::CreateCommandList()
{
    
    /*�R�}���h�A���P�[�^�[�̍쐬*/
    HRESULT hr;
    for (int i = 0; i < FreamBufferCount; i++) {

        hr = mDevice->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            IID_PPV_ARGS(mAllocator[i].ReleaseAndGetAddressOf())
        );

    }

    if (FAILED(hr))return false;

    /*�R�}���h���X�g�̐���*/
    hr = mDevice->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        mAllocator[mCurrentBackBufferIndex].Get(),
        nullptr,
        IID_PPV_ARGS(&mCommandList)
    );

    if (FAILED(hr))return false;

    /*�R�}���h���X�g�͊J����Ă����Ԃō쐬�����̂ŁA�����������*/
    mCommandList->Close();

    return true;

}

/*******************************************************************
* �t�F���X�̐���
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

    /*�������s�����̃C�x���g�n���h�����쐬����*/
    mFenceEvent = CreateEvent(nullptr, false, false, nullptr);
    return mFenceEvent != nullptr;

}

/*******************************************************************
* �r���[�|�[�g�̐���
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
* �V�U�[��`�̐���
********************************************************************/
void Renderer::CreateScissorRect()
{

    mScissor.left = 0;
    mScissor.right = static_cast<LONG>(WindowWidth);
    mScissor.top = 0;
    mScissor.bottom = static_cast<LONG>(WindowHeight);

}

/*******************************************************************
* �����_�[�^�[�Q�b�g�̐���
********************************************************************/
cbool Renderer::CreateRenderTarget()
{
    
    /*RTV�p�̃f�B�X�N���v�^�q�[�v���쐬����*/
    D3D12_DESCRIPTOR_HEAP_DESC desc{};
    desc.NumDescriptors = FreamBufferCount;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    auto hr = mDevice->CreateDescriptorHeap(
        &desc,
        IID_PPV_ARGS(mRtvHeap.ReleaseAndGetAddressOf())
    );

    if (FAILED(hr))return false;

    /*�f�B�X�N���v�^�̃T�C�Y���擾*/
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
* �[�x�X�e���V���o�b�t�@�̐���
********************************************************************/
cbool Renderer::CretaeDepthStenccil()
{
    
    /*DSV�p�̃f�B�X�N���v�^�q�[�v���쐬����*/
    D3D12_DESCRIPTOR_HEAP_DESC heap{};
    heap.NumDescriptors = 1;
    heap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    heap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    auto hr = mDevice->CreateDescriptorHeap(&heap, IID_PPV_ARGS(mDsvHeap.ReleaseAndGetAddressOf()));
    if (FAILED(hr))return false;

    /*�f�B�X�N���v�^�̃T�C�Y���擾*/
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

    /*�f�B�X�N���v�^���쐬*/
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDsvHeap->GetCPUDescriptorHandleForHeapStart();

    mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, dsvHandle);

    return true;

}

/*******************************************************************
* �`�抮����҂���
********************************************************************/
void Renderer::WaitRender()
{

    /*�b���I���҂�*/
    const UINT64 fenceValue = mFenceValue[mCurrentBackBufferIndex];
    mQueue->Signal(mFence.Get(), fenceValue);
    mFenceValue[mCurrentBackBufferIndex]++;

    /*���̃t���[���`�揀�����܂��ł���Αҋ@*/
    if (mFence->GetCompletedValue() < fenceValue) {

        /*�����ɃC�x���g��ݒ�*/
        auto hr = mFence->SetEventOnCompletion(fenceValue, mFenceEvent);
        if (FAILED(hr))return;

        /*�ҋ@����*/
        if (WAIT_OBJECT_0 != WaitForSingleObjectEx(mFenceEvent, INFINITE, false))return;

    }

}

/*******************************************************************
* �f�o�C�X�擾
********************************************************************/
ID3D12Device6* Renderer::GetDevice()
{
    return mDevice.Get();
}

/*******************************************************************
* �R�}���h���X�g�擾
********************************************************************/
ID3D12GraphicsCommandList* Renderer::GetCommandList()
{
    return mCommandList.Get();
}

/*******************************************************************
* �o�b�O�o�b�t�@�C���f�b�N�X
********************************************************************/
uint Renderer::CurrentBackBufferIndex()
{
    return mCurrentBackBufferIndex;
}

/*******************************************************************
* ���[���h�}�g���b�N�X
********************************************************************/
void Renderer::SetWorldMatrix(const XMMATRIX* world)
{
    auto ptr = reinterpret_cast<Matrix*>(mMatrix->GetPtr());
    ptr->World = (*world);
    mCommandList->SetGraphicsRootConstantBufferView(0, mMatrix->GetAddress());
}

/*******************************************************************
* �r���[�}�g���b�N�X
********************************************************************/
void Renderer::SetViewMatrix(const XMMATRIX* view)
{
    auto ptr = reinterpret_cast<Matrix*>(mMatrix->GetPtr());
    ptr->View = (*view);
    mCommandList->SetGraphicsRootConstantBufferView(0, mMatrix->GetAddress());
}

/*******************************************************************
* �v���W�F�N�V�����}�g���b�N�X
********************************************************************/
void Renderer::SetProjectionMatrix(const XMMATRIX* projection)
{
    auto ptr = reinterpret_cast<Matrix*>(mMatrix->GetPtr());
    ptr->Projection = (*projection);
    mCommandList->SetGraphicsRootConstantBufferView(0, mMatrix->GetAddress());
}

/*******************************************************************
* �T���v���[�X�e�[�g����
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
* 2D�}�g���b�N�X�ݒ�
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
* ������
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
* �I������
********************************************************************/
void Renderer::MasterUninit()
{

    if (mRootSignature) delete mRootSignature;
    if (mPipelineState)delete mPipelineState;
    if (mMatrix)delete mMatrix;

}

/*******************************************************************
* �`��J�n����
********************************************************************/
void Renderer::Begin()
{

    /*���݂̃����_�[�^�[�Q�b�g���X�V*/
    mCurrentRenderTarget = mRenderTargets[mCurrentBackBufferIndex].Get();

    /*�R�}���h�����������Ă��߂鏀��������*/
    mAllocator[mCurrentBackBufferIndex]->Reset();
    mCommandList->Reset(mAllocator[mCurrentBackBufferIndex].Get(), nullptr);

    /*�r���[�|�[�g�ƃV�U�[��`��ݒ�*/
    mCommandList->RSSetViewports(1, &mViewport);
    mCommandList->RSSetScissorRects(1, &mScissor);

    /*���݂̃t���[���̃����_�[�^�[�Q�b�g�r���[�̃f�B�X�N���v�^�q�[�v�̊J�n�A�h���X���擾*/
    auto currentRtvHandle = mRtvHeap->GetCPUDescriptorHandleForHeapStart();
    currentRtvHandle.ptr += mCurrentBackBufferIndex * mRtvDescriptorSize;

    /*�[�x�X�e���V���o�b�t�@�̃f�B�X�N���v�^�q�[�v�̊J�n�A�h���X���擾*/
    auto currentDsvHandle = mDsvHeap->GetCPUDescriptorHandleForHeapStart();

    /*�����_�[�^�[�Q�b�g���g�p�\�ɂȂ�܂ő҂�*/
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mCurrentRenderTarget, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    mCommandList->ResourceBarrier(1, &barrier);

    /*�����_�[�^�[�Q�b�g��ݒ�*/
    mCommandList->OMSetRenderTargets(1, &currentRtvHandle, false, &currentDsvHandle);

    /*�����_�[�^�[�Q�b�g���N���A*/
    cfloat ClearColor[4] = { 0.3f,0.3f,0.3f,0.0f };
    mCommandList->ClearRenderTargetView(currentRtvHandle, ClearColor, 0, nullptr);

    /*�[�x�X�e���V���r���[���N���A*/
    mCommandList->ClearDepthStencilView(currentDsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

/*******************************************************************
* �`��I������
********************************************************************/
void Renderer::End()
{

    /*�����_�[�^�[�Q�b�g�ɏ������݂��I���܂ő҂�*/
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mCurrentRenderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    mCommandList->ResourceBarrier(1, &barrier);

    /*�R�}���h�̋L�^���I��*/
    mCommandList->Close();

    /*�R�}���h�����s*/
    ID3D12CommandList* command[] = { mCommandList.Get() };
    mQueue->ExecuteCommandLists(1, command);

    /*�X���b�v�`�F�[����؂�ւ�*/
    mSwapChain->Present(1, 0);

    /*�`�抮����҂�*/
    WaitRender();

    /*�o�b�N�o�b�t�@�ԍ��X�V*/
    mCurrentBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();

}

/*******************************************************************
* ���[�g�V�O�j�`���R���X�g���N�^
********************************************************************/
RootSignature::RootSignature()
{

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

    mIsValid = true;

}

/*******************************************************************
* �p�C�v���C�����C���X�e�[�g�R���X�g���N�^
********************************************************************/
PipeLineState::PipeLineState()
{

    /*�p�C�v���C���X�e�[�g�̐ݒ�*/
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

}

/*******************************************************************
* �Z�b�g���C�A�E�g
********************************************************************/
void PipeLineState::SetInputLayout(D3D12_INPUT_LAYOUT_DESC layout)
{
    mDesc.InputLayout = layout;
}

/*******************************************************************
* �Z�b�g���[�g�V�O�j�`��
********************************************************************/
void PipeLineState::SetRootSignature(ID3D12RootSignature* rootSignature)
{
    mDesc.pRootSignature = rootSignature;
}

/*******************************************************************
* �Z�b�g�o�[�e�b�N�X�V�F�[�_�[
********************************************************************/
void PipeLineState::CreateVertexShader(const wstring& path)
{

    /*���_�V�F�[�_�[�ǂݍ���*/
    if (FAILED(D3DReadFileToBlob(path.c_str(), mVsBlob.GetAddressOf())))return;
    
    mDesc.VS = CD3DX12_SHADER_BYTECODE(mVsBlob.Get());

}

/*******************************************************************
* �Z�b�g�s�N�Z���V�F�[�_�[
********************************************************************/
void PipeLineState::CreatePixelShader(const wstring& path)
{

    /*�s�N�Z���V�F�[�_�[�ǂݍ���*/
    if (FAILED(D3DReadFileToBlob(path.c_str(), mPsBlob.GetAddressOf())))return;
    
    mDesc.PS = CD3DX12_SHADER_BYTECODE(mPsBlob.Get());

}

/*******************************************************************
* ����
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
