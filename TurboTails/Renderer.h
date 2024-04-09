#pragma once


#include"Main.h"


// 頂点構造体
struct VERTEX_3D
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR3 Tangent;
	D3DXVECTOR3 Binormal;
	D3DXVECTOR4 Diffuse;
	D3DXVECTOR2 TexCoord;
};



// マテリアル構造体
struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];//16byte境界用
};




// マテリアル構造体
struct DX11_MODEL_MATERIAL
{
	MATERIAL		Material;
	class CTexture* Texture;
};


// 描画サブセット構造体
struct DX11_SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	DX11_MODEL_MATERIAL	Material;
};

struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];//16byte境界用
	D3DXVECTOR4	Direction;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;
	D3DXMATRIX  ViewMatrix;
	D3DXMATRIX  ProjectionMatrix;
	D3DXVECTOR4 Position;
};

struct PARAMETER
{
	D3DXVECTOR4 hp;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;
	color		Color;
};

struct WIPE
{
	float range;
	float threshold;
	float dummy[2];
};

struct DISSOLVE
{
	float range;
	float threshold;
	float dummy[2];
	color color;
};

class CVertexBuffer;
class CIndexBuffer;
class CTexture;


class Renderer
{
private:

	static D3D_FEATURE_LEVEL       mFeatureLevel;

	static ID3D11Device* mDevice;
	static ID3D11DeviceContext* mDeviceContext;
	static IDXGISwapChain* mSwapChain;
	static ID3D11RenderTargetView* mRenderTargetView;
	static ID3D11DepthStencilView* mDepthStencilView;


	static ID3D11Buffer* mWorldBuffer;
	static ID3D11Buffer* mViewBuffer;
	static ID3D11Buffer* mProjectionBuffer;
	static ID3D11Buffer* mMaterialBuffer;
	static ID3D11Buffer* mLightBuffer;
	static ID3D11Buffer* mCameraBuffer;
	static ID3D11Buffer* mParameterBuffer;
	static ID3D11Buffer* mWipeBuffer;
	static ID3D11Buffer* mDissolveBuffer;

	static ID3D11BlendState* mBlendState;
	static ID3D11BlendState* mBlendStateATC;

	static ID3D11DepthStencilState* mDepthStateEnable;
	static ID3D11DepthStencilState* mDepthStateDisable;

	static ID3D11RenderTargetView* mPPRenderTargetView;
	static ID3D11ShaderResourceView* mPPShaderResourceView;

	static ID3D11RenderTargetView* mBGRenderTargetView;
	static ID3D11ShaderResourceView* mBGShaderResourceView;

	static ID3D11DepthStencilView* mDepthShadowDepthStencilView;
	static ID3D11ShaderResourceView* mDepthShadowShaderResourceView;

	static ID3D11RenderTargetView* mLuminaceRenderTargetView;
	static ID3D11ShaderResourceView* mLuminaceShaderResourceView;
	static ID3D11DepthStencilView* mLuminaceDepthStencilView;

	static ID3D11RenderTargetView* mBloomRenderTargetView[4];
	static ID3D11ShaderResourceView* mBloomShaderResourceView[4];
	static ID3D11DepthStencilView* mBloomDepthStencilView[4];

	static IDXGIAdapter* GetFastestAdapter();

public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void BeginDepth();
	static void BeginPP();
	static void BeginBG();
	static void BeginBloom(crint id);
	static void BeginLuminace();

	static void SetDepthEnable(bool Enable);
	static void SetATCEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX* WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX* ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);

	static void SetCameraPosition(D3DXVECTOR3 CameraPosition);
	static void SetParameter(PARAMETER param);
	static void SetWipe(const WIPE& wipe);
	static void SetDissolve(const DISSOLVE& dissolve);

	static void SetViewPortNormal();
	static void SetViewPortDepth();

	static ID3D11Device* GetDevice(void) { return mDevice; }
	static ID3D11DeviceContext* GetDeviceContext(void) { return mDeviceContext; }

	static ID3D11ShaderResourceView* GetPPTexture() { return mPPShaderResourceView; }
	static ID3D11ShaderResourceView* GetBGTexture() { return mBGShaderResourceView; }
	static ID3D11ShaderResourceView* GetDepthShadowTexture() { return mDepthShadowShaderResourceView; }

	static ID3D11ShaderResourceView** GetLuminaceTexture() { return &mLuminaceShaderResourceView; }
	static ID3D11ShaderResourceView** GetBloomTexture(cruint index) { return &mBloomShaderResourceView[index]; }

	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

	static void SetViewport(const int& size = 1);
	static void SetBloomViewport(crint index = 0);
	static void SetLuminaceViewport();

};
