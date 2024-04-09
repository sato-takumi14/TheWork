
#include"Main.h"
#include "Renderer.h"
#include <io.h>

#include<dxgi.h>
#pragma comment(lib, "DXGI.lib")

cuint DepthSize = 1024;

D3D_FEATURE_LEVEL       Renderer::mFeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device* Renderer::mDevice{};
ID3D11DeviceContext* Renderer::mDeviceContext{};
IDXGISwapChain* Renderer::mSwapChain{};
ID3D11RenderTargetView* Renderer::mRenderTargetView{};
ID3D11DepthStencilView* Renderer::mDepthStencilView{};

ID3D11Buffer* Renderer::mWorldBuffer{};
ID3D11Buffer* Renderer::mViewBuffer{};
ID3D11Buffer* Renderer::mProjectionBuffer{};
ID3D11Buffer* Renderer::mMaterialBuffer{};
ID3D11Buffer* Renderer::mLightBuffer{};
ID3D11Buffer* Renderer::mCameraBuffer{};
ID3D11Buffer* Renderer::mParameterBuffer{};
ID3D11Buffer* Renderer::mWipeBuffer{};
ID3D11Buffer* Renderer::mDissolveBuffer{};

ID3D11BlendState* Renderer::mBlendState{};
ID3D11BlendState* Renderer::mBlendStateATC{};

ID3D11DepthStencilState* Renderer::mDepthStateEnable{};
ID3D11DepthStencilState* Renderer::mDepthStateDisable{};

ID3D11RenderTargetView* Renderer::mPPRenderTargetView{};
ID3D11ShaderResourceView* Renderer::mPPShaderResourceView{};

ID3D11RenderTargetView* Renderer::mBGRenderTargetView{};
ID3D11ShaderResourceView* Renderer::mBGShaderResourceView{};

ID3D11DepthStencilView* Renderer::mDepthShadowDepthStencilView{};
ID3D11ShaderResourceView* Renderer::mDepthShadowShaderResourceView{};

ID3D11RenderTargetView* Renderer::mLuminaceRenderTargetView{};
ID3D11ShaderResourceView* Renderer::mLuminaceShaderResourceView{};
ID3D11DepthStencilView* Renderer::mLuminaceDepthStencilView{};

ID3D11RenderTargetView* Renderer::mBloomRenderTargetView[4]{};
ID3D11ShaderResourceView* Renderer::mBloomShaderResourceView[4]{};
ID3D11DepthStencilView* Renderer::mBloomDepthStencilView[4]{};

IDXGIAdapter* Renderer::GetFastestAdapter()
{
	IDXGIFactory* pFactory = nullptr;
	CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pFactory));

	IDXGIAdapter* pBestAdapter = nullptr;
	size_t highestScore = 0;

	if (pFactory) {
		IDXGIAdapter* pAdapter = nullptr;
		for (UINT i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
			DXGI_ADAPTER_DESC desc;
			pAdapter->GetDesc(&desc);

			// �����Ő��\��]�����đI�����郍�W�b�N����������
			size_t adapterScore = desc.DedicatedVideoMemory + desc.DedicatedSystemMemory + desc.SharedSystemMemory;

			if (adapterScore > highestScore) {
				highestScore = adapterScore;
				if (pBestAdapter)
					pBestAdapter->Release();
				pBestAdapter = pAdapter;
			}
			else {
				pAdapter->Release();
			}
		}
	}

	return pBestAdapter;
}

void Renderer::Init()
{
	HRESULT hr = S_OK;

	// �f�o�C�X�A�X���b�v�`�F�[���A�R���e�L�X�g����
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = GetWindow();
	sd.SampleDesc.Count = 4;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	
	hr = D3D11CreateDeviceAndSwapChain(
		Renderer::GetFastestAdapter(),
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&mSwapChain,
		&mDevice,
		&mFeatureLevel,
		&mDeviceContext);


	// �����_�[�^�[�Q�b�g�r���[�����A�ݒ�
	ID3D11Texture2D* pBackBuffer = NULL;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	mDevice->CreateRenderTargetView(pBackBuffer, NULL, &mRenderTargetView);
	pBackBuffer->Release();



	//�X�e���V���p�e�N�X�`���[�쐬
	ID3D11Texture2D* depthTexture = NULL;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = sd.BufferDesc.Width;
	td.Height = sd.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc = sd.SampleDesc;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	mDevice->CreateTexture2D(&td, NULL, &depthTexture);

	//�X�e���V���^�[�Q�b�g�쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	mDevice->CreateDepthStencilView(depthTexture, &dsvd, &mDepthStencilView);


	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);


	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	mDeviceContext->RSSetViewports(1, &vp);



	// ���X�^���C�U�X�e�[�g�ݒ�
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	ID3D11RasterizerState* rs;
	mDevice->CreateRasterizerState(&rd, &rs);

	mDeviceContext->RSSetState(rs);




	// �u�����h�X�e�[�g�ݒ�
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	mDevice->CreateBlendState(&blendDesc, &mBlendState);

	blendDesc.AlphaToCoverageEnable = TRUE;
	mDevice->CreateBlendState(&blendDesc, &mBlendStateATC);

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	mDeviceContext->OMSetBlendState(mBlendState, blendFactor, 0xffffffff);



	// �[�x�X�e���V���X�e�[�g�ݒ�
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStateEnable);//�[�x�L���X�e�[�g

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStateDisable);//�[�x�����X�e�[�g

	mDeviceContext->OMSetDepthStencilState(mDepthStateEnable, NULL);




	// �T���v���[�X�e�[�g�ݒ�
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = NULL;
	mDevice->CreateSamplerState(&samplerDesc, &samplerState);

	mDeviceContext->PSSetSamplers(0, 1, &samplerState);



	// �萔�o�b�t�@����
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	mDevice->CreateBuffer(&hBufferDesc, NULL, &mWorldBuffer);
	mDeviceContext->VSSetConstantBuffers(0, 1, &mWorldBuffer);

	mDevice->CreateBuffer(&hBufferDesc, NULL, &mViewBuffer);
	mDeviceContext->VSSetConstantBuffers(1, 1, &mViewBuffer);

	mDevice->CreateBuffer(&hBufferDesc, NULL, &mProjectionBuffer);
	mDeviceContext->VSSetConstantBuffers(2, 1, &mProjectionBuffer);


	hBufferDesc.ByteWidth = sizeof(MATERIAL);

	mDevice->CreateBuffer(&hBufferDesc, NULL, &mMaterialBuffer);
	mDeviceContext->VSSetConstantBuffers(3, 1, &mMaterialBuffer);


	hBufferDesc.ByteWidth = sizeof(LIGHT);

	mDevice->CreateBuffer(&hBufferDesc, NULL, &mLightBuffer);
	mDeviceContext->VSSetConstantBuffers(4, 1, &mLightBuffer);
	mDeviceContext->PSSetConstantBuffers(4, 1, &mLightBuffer);


	hBufferDesc.ByteWidth = sizeof(D3DXVECTOR4);

	mDevice->CreateBuffer(&hBufferDesc, NULL, &mCameraBuffer);
	mDeviceContext->PSSetConstantBuffers(5, 1, &mCameraBuffer);


	hBufferDesc.ByteWidth = sizeof(PARAMETER);

	mDevice->CreateBuffer(&hBufferDesc, NULL, &mParameterBuffer);
	mDeviceContext->VSSetConstantBuffers(6, 1, &mParameterBuffer);
	mDeviceContext->PSSetConstantBuffers(6, 1, &mParameterBuffer);

	hBufferDesc.ByteWidth = sizeof(WIPE);

	mDevice->CreateBuffer(&hBufferDesc, NULL, &mWipeBuffer);
	mDeviceContext->PSSetConstantBuffers(7, 1, &mWipeBuffer);

	hBufferDesc.ByteWidth = sizeof(DISSOLVE);

	mDevice->CreateBuffer(&hBufferDesc, NULL, &mDissolveBuffer);
	mDeviceContext->PSSetConstantBuffers(8, 1, &mDissolveBuffer);

	// ���C�g������
	LIGHT light;
	light.Enable = false;
	SetLight(light);



	// �}�e���A��������
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);


	//�}���`�p�X�����_�����O
	{
		//�e�N�X�`���쐬
		ID3D11Texture2D* ppTexture{};
		D3D11_TEXTURE2D_DESC td;				//�e�N�X�`���쐬�p�f�X�N���v�^�\���̕ϐ�
		ZeroMemory(&td, sizeof(td));			//�\���̂�0������

		td.Width = sd.BufferDesc.Width;			//�\����sd��Init�֐��̍ŏ��ō����
		td.Height = sd.BufferDesc.Height;		//�o�b�N�o�b�t�@�̏�񂪊i�[����Ă���

		td.MipLevels = 1;						//�~�j�}�b�v�̐��A0�͌��E�܂ō��
		td.ArraySize = 1;

		td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	//�s�N�Z���t�H�[�}�b�g
		td.SampleDesc = sd.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;

		//�g�p�@�̃t���O��ݒ�
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		//�\���̂̐ݒ�ɏ]���ăe�N�X�`���̈���쐬
		mDevice->CreateTexture2D(&td, NULL, &ppTexture);

		//�����_�[�^�[�Q�b�g�r���[�쐬
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		mDevice->CreateRenderTargetView(ppTexture, &rtvd, &mPPRenderTargetView);

		//�V�F�[�_�[���\�[�X�r���[�쐬
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		mDevice->CreateShaderResourceView(ppTexture, &srvd, &mPPShaderResourceView);

		ppTexture->Release();

	}

	{
		//�e�N�X�`���쐬
		ID3D11Texture2D* ppTexture{};
		D3D11_TEXTURE2D_DESC td;				//�e�N�X�`���쐬�p�f�X�N���v�^�\���̕ϐ�
		ZeroMemory(&td, sizeof(td));			//�\���̂�0������

		td.Width = sd.BufferDesc.Width;			//�\����sd��Init�֐��̍ŏ��ō����
		td.Height = sd.BufferDesc.Height;		//�o�b�N�o�b�t�@�̏�񂪊i�[����Ă���

		td.MipLevels = 1;						//�~�j�}�b�v�̐��A0�͌��E�܂ō��
		td.ArraySize = 1;

		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//�s�N�Z���t�H�[�}�b�g
		td.SampleDesc = sd.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;

		//�g�p�@�̃t���O��ݒ�
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		//�\���̂̐ݒ�ɏ]���ăe�N�X�`���̈���쐬
		mDevice->CreateTexture2D(&td, NULL, &ppTexture);

		//�����_�[�^�[�Q�b�g�r���[�쐬
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		mDevice->CreateRenderTargetView(ppTexture, &rtvd, &mBGRenderTargetView);

		//�V�F�[�_�[���\�[�X�r���[�쐬
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		mDevice->CreateShaderResourceView(ppTexture, &srvd, &mBGShaderResourceView);

		ppTexture->Release();

	}

	{

		//�V���h�E�o�b�t�@�쐬
		ID3D11Texture2D* depthTexture{};
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = DepthSize;
		td.Height = DepthSize;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R32_TYPELESS;//32bit�̎��R�Ȍ`���̃f�[�^�Ƃ���
		td.SampleDesc = sd.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		mDevice->CreateTexture2D(&td, NULL, &depthTexture);

		//�f�v�X�X�e���V���^�[�Q�b�g�r���[�쐬
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format = DXGI_FORMAT_D32_FLOAT;//�s�N�Z���t�H�[�}�b�g��32BitFLOAT�^
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		mDevice->CreateDepthStencilView(depthTexture, &dsvd, &mDepthShadowDepthStencilView);

		//�V�F�[�_�[���\�[�X�r���[�쐬
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R32_FLOAT;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		mDevice->CreateShaderResourceView(depthTexture, &srvd, &mDepthShadowShaderResourceView);

		depthTexture->Release();

	}

	{

		//�e�N�X�`���[�쐬
		ID3D11Texture2D* ppTexture = NULL;
		D3D11_TEXTURE2D_DESC td; //�e�N�X�`���쐬�p�f�X�N���v�^�\���̕ϐ�
		ZeroMemory(&td, sizeof(td)); //�\���̂��O������

		td.Width = static_cast<int>(SCREEN_WIDTH * 0.5f);
		td.Height = static_cast<int>(SCREEN_HEIGHT * 0.5f);
		td.MipLevels = 1;//�~�b�v�}�b�v�̐� 0�͌��E�܂ō��
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; //�s�N�Z���t�H�[�}�b�g
		td.SampleDesc = sd.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;
		//�g�p�@�̃t���O��ݒ�
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		//�\���̂̐ݒ�ɏ]���ăe�N�X�`���̈���쐬
		mDevice->CreateTexture2D(&td, NULL, &ppTexture);

		//�����_�[�^�[�Q�b�g�r���[�쐬
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		mDevice->CreateRenderTargetView(ppTexture, &rtvd,
			&mLuminaceRenderTargetView);

		//�f�v�X�X�e���V���^�[�Q�b�g�r���[�쐬
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format = DXGI_FORMAT_D32_FLOAT;//�s�N�Z���t�H�[�}�b�g��32BitFLOAT�^
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		mDevice->CreateDepthStencilView(ppTexture, &dsvd, &mLuminaceDepthStencilView);

		//�V�F�[�_�[���\�[�X�r���[�쐬
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		mDevice->CreateShaderResourceView(ppTexture, &srvd,
			&mLuminaceShaderResourceView);
		ppTexture->Release();

	}

	{

		float size = 0.25f;

		for (int i = 0; i < 4; i++) {

			//�e�N�X�`���[�쐬
			ID3D11Texture2D* ppTexture = NULL;
			D3D11_TEXTURE2D_DESC td; //�e�N�X�`���쐬�p�f�X�N���v�^�\���̕ϐ�
			ZeroMemory(&td, sizeof(td)); //�\���̂��O������

			td.Width = static_cast<int>(SCREEN_WIDTH * size);
			td.Height = static_cast<int>(SCREEN_HEIGHT * size);

			size *= 0.5f;

			td.MipLevels = 1;//�~�b�v�}�b�v�̐� 0�͌��E�܂ō��
			td.ArraySize = 1;
			td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; //�s�N�Z���t�H�[�}�b�g
			td.SampleDesc = sd.SampleDesc;
			td.Usage = D3D11_USAGE_DEFAULT;
			//�g�p�@�̃t���O��ݒ�
			td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			td.CPUAccessFlags = 0;
			td.MiscFlags = 0;
			//�\���̂̐ݒ�ɏ]���ăe�N�X�`���̈���쐬
			mDevice->CreateTexture2D(&td, NULL, &ppTexture);

			//�����_�[�^�[�Q�b�g�r���[�쐬
			D3D11_RENDER_TARGET_VIEW_DESC rtvd;
			ZeroMemory(&rtvd, sizeof(rtvd));
			rtvd.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			mDevice->CreateRenderTargetView(ppTexture, &rtvd,
				&mBloomRenderTargetView[i]);

			//�f�v�X�X�e���V���^�[�Q�b�g�r���[�쐬
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
			ZeroMemory(&dsvd, sizeof(dsvd));
			dsvd.Format = DXGI_FORMAT_D32_FLOAT;//�s�N�Z���t�H�[�}�b�g��32BitFLOAT�^
			dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			mDevice->CreateDepthStencilView(ppTexture, &dsvd, &mBloomDepthStencilView[i]);

			//�V�F�[�_�[���\�[�X�r���[�쐬
			D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
			ZeroMemory(&srvd, sizeof(srvd));
			srvd.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvd.Texture2D.MipLevels = 1;
			mDevice->CreateShaderResourceView(ppTexture, &srvd,
				&mBloomShaderResourceView[i]);

			ppTexture->Release();

		}


	}

}


void Renderer::Uninit()
{
	// �I�u�W�F�N�g���
	mWorldBuffer->Release();
	mViewBuffer->Release();
	mProjectionBuffer->Release();
	mLightBuffer->Release();
	mMaterialBuffer->Release();
	mCameraBuffer->Release();
	mWipeBuffer->Release();
	mDissolveBuffer->Release();

	mDeviceContext->ClearState();
	mRenderTargetView->Release();
	mSwapChain->Release();
	mDeviceContext->Release();
	mDevice->Release();
}



void Renderer::Begin()
{

	/*�f�t�H���g�̃o�b�N�o�b�t�@�Ɛ[�x�o�b�t�@�Ƀ����_�����O�^�[�Q�b�g��ݒ肷��*/
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	// �o�b�N�o�b�t�@�N���A
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, ClearColor);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}



void Renderer::End()
{
	mSwapChain->Present(1, 0);
}

void Renderer::BeginDepth()
{
	mDeviceContext->OMSetRenderTargets(0, NULL, mDepthShadowDepthStencilView);
	mDeviceContext->ClearDepthStencilView(mDepthShadowDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::BeginPP()
{
	//�����_�[�^�[�Q�b�g��Z�o�b�t�@�̃Z�b�g
	mDeviceContext->OMSetRenderTargets(1, &mPPRenderTargetView, mDepthStencilView);

	//������₷���悤�Ƀ����_�[�^�[�Q�b�g��΂ŃN���A���Ă���
	float ClearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
	mDeviceContext->ClearRenderTargetView(mPPRenderTargetView, ClearColor);

	//Z�o�b�t�@�̃N���A
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::BeginBG()
{
	//�����_�[�^�[�Q�b�g��Z�o�b�t�@�̃Z�b�g
	mDeviceContext->OMSetRenderTargets(1, &mBGRenderTargetView, mDepthStencilView);

	//������₷���悤�Ƀ����_�[�^�[�Q�b�g��΂ŃN���A���Ă���
	float ClearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
	mDeviceContext->ClearRenderTargetView(mBGRenderTargetView, ClearColor);

	//Z�o�b�t�@�̃N���A
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::BeginBloom(crint id)
{
	//�����_�[�^�[�Q�b�g���o�b�N�o�b�t�@�ɖ߂�
	mDeviceContext->OMSetRenderTargets(1, &mBloomRenderTargetView[id], mBloomDepthStencilView[id]);

	//�o�b�N�o�b�t�@�N���A
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };//��
	mDeviceContext->ClearRenderTargetView(mBloomRenderTargetView[id], ClearColor);
	mDeviceContext->ClearDepthStencilView(mBloomDepthStencilView[id], D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::BeginLuminace()
{
	//�����_�[�^�[�Q�b�g���o�b�N�o�b�t�@�ɖ߂�
	mDeviceContext->OMSetRenderTargets(1, &mLuminaceRenderTargetView, mLuminaceDepthStencilView);

	//�o�b�N�o�b�t�@�N���A
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };//��
	mDeviceContext->ClearRenderTargetView(mLuminaceRenderTargetView, ClearColor);
	mDeviceContext->ClearDepthStencilView(mLuminaceDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}




void Renderer::SetDepthEnable(bool Enable)
{
	if (Enable)
		mDeviceContext->OMSetDepthStencilState(mDepthStateEnable, NULL);
	else
		mDeviceContext->OMSetDepthStencilState(mDepthStateDisable, NULL);

}

void Renderer::SetATCEnable(bool Enable)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		mDeviceContext->OMSetBlendState(mBlendStateATC, blendFactor, 0xffffffff);
	else
		mDeviceContext->OMSetBlendState(mBlendState, blendFactor, 0xffffffff);
}

void Renderer::SetWorldViewProjection2D()
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranspose(&world, &world);

	mDeviceContext->UpdateSubresource(mWorldBuffer, 0, NULL, &world, 0, 0);

	D3DXMATRIX view;
	D3DXMatrixIdentity(&view);
	D3DXMatrixTranspose(&view, &view);
	mDeviceContext->UpdateSubresource(mViewBuffer, 0, NULL, &view, 0, 0);

	D3DXMATRIX projection;
	D3DXMatrixOrthoOffCenterLH(&projection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3DXMatrixTranspose(&projection, &projection);
	mDeviceContext->UpdateSubresource(mProjectionBuffer, 0, NULL, &projection, 0, 0);

}


void Renderer::SetWorldMatrix(D3DXMATRIX* WorldMatrix)
{
	D3DXMATRIX world;
	D3DXMatrixTranspose(&world, WorldMatrix);
	mDeviceContext->UpdateSubresource(mWorldBuffer, 0, NULL, &world, 0, 0);
}

void Renderer::SetViewMatrix(D3DXMATRIX* ViewMatrix)
{
	D3DXMATRIX view;
	D3DXMatrixTranspose(&view, ViewMatrix);
	mDeviceContext->UpdateSubresource(mViewBuffer, 0, NULL, &view, 0, 0);
}

void Renderer::SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix)
{
	D3DXMATRIX projection;
	D3DXMatrixTranspose(&projection, ProjectionMatrix);
	mDeviceContext->UpdateSubresource(mProjectionBuffer, 0, NULL, &projection, 0, 0);
}



void Renderer::SetMaterial(MATERIAL Material)
{
	mDeviceContext->UpdateSubresource(mMaterialBuffer, 0, NULL, &Material, 0, 0);
}

void Renderer::SetLight(LIGHT Light)
{
	//�V�F�[�_�[���̓s����ōs���]�u���Ă���
	D3DXMatrixTranspose(&Light.ViewMatrix, &Light.ViewMatrix);
	D3DXMatrixTranspose(&Light.ProjectionMatrix, &Light.ProjectionMatrix);

	mDeviceContext->UpdateSubresource(mLightBuffer, 0, NULL, &Light, 0, 0);
}

void Renderer::SetCameraPosition(D3DXVECTOR3 CameraPosition)
{
	D3DXVECTOR4 cpos(CameraPosition.x, CameraPosition.y, CameraPosition.z, 1.0f);
	mDeviceContext->UpdateSubresource(mCameraBuffer, 0, NULL, &cpos, 0, 0);
}

void Renderer::SetParameter(PARAMETER param)
{
	mDeviceContext->UpdateSubresource(mParameterBuffer, 0, NULL, &param, 0, 0);
}

void Renderer::SetWipe(const WIPE& wipe)
{
	mDeviceContext->UpdateSubresource(mWipeBuffer, 0, NULL, &wipe, 0, 0);
}

void Renderer::SetDissolve(const DISSOLVE& dissolve)
{
	mDeviceContext->UpdateSubresource(mDissolveBuffer, 0, NULL, &dissolve, 0, 0);
}

void Renderer::SetViewPortNormal()
{
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	mDeviceContext->RSSetViewports(1, &vp);
}

void Renderer::SetViewPortDepth()
{
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)DepthSize;
	vp.Height = (FLOAT)DepthSize;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	mDeviceContext->RSSetViewports(1, &vp);
}

// ���_�V�F�[�_����
void Renderer::CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName)
{

	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	mDevice->CreateVertexShader(buffer, fsize, NULL, VertexShader);


	// ���̓��C�A�E�g����
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 9, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	mDevice->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}



// �s�N�Z���V�F�[�_����
void Renderer::CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName)
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	mDevice->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}

void Renderer::SetViewport(const int& size)
{

	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH * size;
	vp.Height = (FLOAT)SCREEN_HEIGHT * size;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	mDeviceContext->RSSetViewports(1, &vp);

}

void Renderer::SetBloomViewport(crint index)
{
	float size = 0.25f;

	for (int i = 0; i < index; i++) { size *= 0.5f; }

	D3D11_VIEWPORT vp;
	vp.Width = static_cast<float>(SCREEN_WIDTH) * size;
	vp.Height = static_cast<float>(SCREEN_HEIGHT) * size;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	mDeviceContext->RSSetViewports(1, &vp);
}

void Renderer::SetLuminaceViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH * 0.5f;
	vp.Height = (FLOAT)SCREEN_HEIGHT * 0.5f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	mDeviceContext->RSSetViewports(1, &vp);
}


