#pragma once

/***********************************************************************************************************
*
*	Renderer
*
*																				制作者：佐藤匠
*																				制作日：02/19
*
************************************************************************************************************
* レンダリグに必要な機能を集めたクラス
************************************************************************************************************/

#include"Main.h"
#include"System\Buffer.h"
#include"System\Vector.h"

typedef struct Vertex3D
{
private:

	static const int InputElementCount = 5;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];

public:

	static const D3D12_INPUT_LAYOUT_DESC InputLayout;

	Vector3 Position;		/*ポジション*/
	Vector3 Normal;			/*法線*/
	Vector4 Diffuse;		/*頂点色*/
	Vector3 Tangent;		/*接空間*/
	Vector2 TexCoord;		/*UV値*/

}Vertex;

struct Mesh {

	std::vector<Vertex3D> Vertex;
	std::vector<uint32_t> Index;
	wstring TextureFile;

};

struct alignas(256) Matrix {

public:

	matrix World;
	matrix View;
	matrix Projection;

};

class RootSignature
{
private:

	bool mIsValid = false;
	ComPtr<ID3D12RootSignature> mRootSignature{};

public:

	RootSignature();
	inline cbool GetIsValid()const { return mIsValid; }
	inline ID3D12RootSignature* GetRootSignature() { return mRootSignature.Get(); }

};

class PipeLineState
{
private:

	bool mIsValid = false;							/*生成の成功したかどうか*/
	D3D12_GRAPHICS_PIPELINE_STATE_DESC mDesc{};		/*パイプラインステートの設定*/
	ComPtr<ID3D12PipelineState> mPipeLineState{};	/*パイプラインステート*/
	ComPtr<ID3DBlob> mVsBlob{};						/*頂点シェーダー*/
	ComPtr<ID3DBlob> mPsBlob{};						/*ピクセルシェーダー*/

public:

	PipeLineState();
	inline cbool GetIsValid()const { return mIsValid; }
	void SetInputLayout(D3D12_INPUT_LAYOUT_DESC layout);
	void SetRootSignature(ID3D12RootSignature* rootSignature);
	void CreateVertexShader(const wstring& path);
	void CreatePixelShader(const wstring& path);
	void Create();

	inline ID3D12PipelineState* GetPipleLineState() { return mPipeLineState.Get(); }

};

class Renderer
{
public:

	enum {
		FreamBufferCount = 2,
	};	

private:

	/*描画関連*/
	static uint									mCurrentBackBufferIndex;
	static ComPtr<ID3D12Device6>				mDevice;							/*デバイス*/
	static ComPtr<ID3D12CommandQueue>			mQueue;								/*コマンドキュー*/
	static ComPtr<IDXGISwapChain3>				mSwapChain;							/*スワップチェーン*/
	static ComPtr<ID3D12CommandAllocator>		mAllocator[FreamBufferCount];		/*コマンドアロケーター*/
	static ComPtr<ID3D12GraphicsCommandList>	mCommandList;						/*コマンドリスト*/
	static HANDLE								mFenceEvent;						/*フェンスで使うイベント*/
	static ComPtr<ID3D12Fence>					mFence;								/*フェンス*/
	static UINT64								mFenceValue[FreamBufferCount];		/*フェンスの値*/
	static D3D12_VIEWPORT						mViewport;							/*ビューポート*/
	static D3D12_RECT							mScissor;							/*シザー短形*/
	static uint									mRtvDescriptorSize;					/*レンダーターゲットビューのディスクリプタサイズ*/
	static ComPtr<ID3D12DescriptorHeap>			mRtvHeap;							/*レンダーターゲットのディクリプタヒープ*/
	static ComPtr<ID3D12Resource>				mRenderTargets[FreamBufferCount];	/*レンダーターゲット*/
	static uint									mDsvDescriptorSize;					/*深度ステンシルのディスクリプタサイズ*/
	static ComPtr<ID3D12DescriptorHeap>			mDsvHeap;							/*深度ステンシルのディスクリプタヒープ*/
	static ComPtr<ID3D12Resource>				mDepthStencilBuffer;				/*深度ステンシルバッファ*/
	static ID3D12Resource*						mCurrentRenderTarget;				/*現在のフレームのレンダーターゲットを一時的に保存しておく関数*/

	/*コンスタントバッファ*/
	static Buffer::Constant* mMatrix;

	/*ルートシグニチャ*/
	static RootSignature*						mRootSignature;

	/*パイプライン*/
	static PipeLineState*						mPipelineState;

private:

	static cbool CreateDevice();			/*デバイスを生成*/
	static cbool CreateCommandQueue();		/*コマンドキューを生成*/
	static cbool CreateSwapChain();			/*スワップチェーンを生成*/
	static cbool CreateCommandList();		/*コマンドリストとコマンドアロケーターを生成*/
	static cbool CreateFence();				/*フェンスを生成*/
	static void CreateViewPort();			/*ビューポートを生成*/
	static void CreateScissorRect();		/*シザー短形を生成*/
	static cbool CreateRenderTarget();		/*レンダーターゲットを生成*/
	static cbool CretaeDepthStenccil();		/*深度ステンシルバッファを生成*/
	static void WaitRender();				/*描画完了を待つ処理*/
	static cbool CreateSampler();			/*サンプラーステートを生成*/

public:

	static void MasterInit();
	static void MasterUninit();

	static void Begin();
	static void End();

	static ID3D12Device6* GetDevice();
	static ID3D12GraphicsCommandList* GetCommandList();
	static uint CurrentBackBufferIndex();

	static void SetWorldMatrix(const XMMATRIX* world);
	static void SetViewMatrix(const XMMATRIX* view);
	static void SetProjectionMatrix(const XMMATRIX* projection);
	static void SetWorldViewProjection2D();

};