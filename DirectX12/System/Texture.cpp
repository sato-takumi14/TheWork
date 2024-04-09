
#include "Main.h"
#include "Texture.h"
#include"System\Renderer.h"

const wstring Texture::Texture::GetWideString(crstring str)
{
    auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

    std::wstring wstr;
    wstr.resize(num1);

    auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);

    assert(num1 == num2);
    return wstr;
}

const wstring Texture::Texture::FileExtension(const wstring& path)
{
    auto idx = path.rfind(L'.');
    return path.substr(idx + 1, path.length() - idx - 1);
}

cstring Texture::Texture::FileExtension(crstring path)
{
    auto idx = path.rfind(".");
    return path.substr(idx + 1, path.length() - idx - 1);
}

cbool Texture::Texture::Load(crstring path)
{
    
    /*WICテクスチャのロード*/
    TexMetadata meta{};
    ScratchImage scratch{};
    wstring file = GetWideString(path);
    string ext = FileExtension(path);
    

    HRESULT hr = S_FALSE;
    if (ext == "png" || 
        ext == "jpeg" ||
        ext == "jpg") {
        hr = LoadFromWICFile(file.c_str(), WIC_FLAGS_NONE, &meta, scratch);
    }
    else if (ext == "tga") {
        hr = LoadFromTGAFile(file.c_str(), &meta, scratch);
    }

    if (FAILED(hr))return false;

    auto img = scratch.GetImage(0, 0, 0);
    auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
    auto desc = CD3DX12_RESOURCE_DESC::Tex2D(meta.format,
        meta.width,
        static_cast<UINT>(meta.height),
        static_cast<UINT16>(meta.arraySize),
        static_cast<UINT16>(meta.mipLevels));

    /*リソースの生成*/
    if (FAILED(
        Renderer::GetDevice()->CreateCommittedResource(
            &prop,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
            nullptr,
            IID_PPV_ARGS(mResource.ReleaseAndGetAddressOf())
        )
    ))return false;

    if (FAILED(
        mResource->WriteToSubresource(
            0,
            nullptr,                            /*全領域へのコピー*/
            img->pixels,                        /*元データアドレス*/
            static_cast<UINT>(img->rowPitch),   /*1ラインサイズ*/
            static_cast<UINT>(img->slicePitch)  /*全サイズ*/
        )
    ))return false;

    return true;

}

D3D12_SHADER_RESOURCE_VIEW_DESC Texture::Texture::GetViewDesc()
{
    D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
    desc.Format = mResource->GetDesc().Format;
    desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipLevels = 1;
    return desc;
}

Texture::DescriptorHeap::DescriptorHeap()
{

    mHandles.clear();
    mHandles.reserve(HANDLE_MAX);

    D3D12_DESCRIPTOR_HEAP_DESC desc{};
    desc.NodeMask = 1;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.NumDescriptors = HANDLE_MAX;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    auto device = Renderer::GetDevice();

    /*ディスクリプタヒープを生成*/
    if (FAILED(device->CreateDescriptorHeap(
        &desc,
        IID_PPV_ARGS(mHeap.ReleaseAndGetAddressOf())
    ))) {
        mIsValid = false;
        return;
    }

    /*ディスクリプタヒープ1個のメモリサイズを返す*/
    mIncrementSize = device->GetDescriptorHandleIncrementSize(desc.Type);
    mIsValid = true;

}

Texture::DescriptorHandle* Texture::DescriptorHeap::Register(Texture* tex)
{
    
    auto count = mHandles.size();
    if (HANDLE_MAX <= count)return nullptr;

    DescriptorHandle* handle = new DescriptorHandle();

    auto handleCPU = mHeap->GetCPUDescriptorHandleForHeapStart();
    handleCPU.ptr += mIncrementSize * count;

    auto handleGPU = mHeap->GetGPUDescriptorHandleForHeapStart();
    handleGPU.ptr += mIncrementSize * count;

    handle->mHandleCPU = handleCPU;
    handle->mHandleGPU = handleGPU;

    auto desc = tex->GetViewDesc();
    Renderer::GetDevice()->CreateShaderResourceView(tex->GetResource(), &desc, handle->mHandleCPU);

    mHandles.push_back(handle);

    return handle;

}
