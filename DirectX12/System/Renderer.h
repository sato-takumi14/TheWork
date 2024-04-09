#pragma once

/***********************************************************************************************************
*
*	Renderer
*
*																				����ҁF������
*																				������F02/19
*
************************************************************************************************************
* �����_���O�ɕK�v�ȋ@�\���W�߂��N���X
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

	Vector3 Position;		/*�|�W�V����*/
	Vector3 Normal;			/*�@��*/
	Vector4 Diffuse;		/*���_�F*/
	Vector3 Tangent;		/*�ڋ��*/
	Vector2 TexCoord;		/*UV�l*/

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

	bool mIsValid = false;							/*�����̐����������ǂ���*/
	D3D12_GRAPHICS_PIPELINE_STATE_DESC mDesc{};		/*�p�C�v���C���X�e�[�g�̐ݒ�*/
	ComPtr<ID3D12PipelineState> mPipeLineState{};	/*�p�C�v���C���X�e�[�g*/
	ComPtr<ID3DBlob> mVsBlob{};						/*���_�V�F�[�_�[*/
	ComPtr<ID3DBlob> mPsBlob{};						/*�s�N�Z���V�F�[�_�[*/

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

	/*�`��֘A*/
	static uint									mCurrentBackBufferIndex;
	static ComPtr<ID3D12Device6>				mDevice;							/*�f�o�C�X*/
	static ComPtr<ID3D12CommandQueue>			mQueue;								/*�R�}���h�L���[*/
	static ComPtr<IDXGISwapChain3>				mSwapChain;							/*�X���b�v�`�F�[��*/
	static ComPtr<ID3D12CommandAllocator>		mAllocator[FreamBufferCount];		/*�R�}���h�A���P�[�^�[*/
	static ComPtr<ID3D12GraphicsCommandList>	mCommandList;						/*�R�}���h���X�g*/
	static HANDLE								mFenceEvent;						/*�t�F���X�Ŏg���C�x���g*/
	static ComPtr<ID3D12Fence>					mFence;								/*�t�F���X*/
	static UINT64								mFenceValue[FreamBufferCount];		/*�t�F���X�̒l*/
	static D3D12_VIEWPORT						mViewport;							/*�r���[�|�[�g*/
	static D3D12_RECT							mScissor;							/*�V�U�[�Z�`*/
	static uint									mRtvDescriptorSize;					/*�����_�[�^�[�Q�b�g�r���[�̃f�B�X�N���v�^�T�C�Y*/
	static ComPtr<ID3D12DescriptorHeap>			mRtvHeap;							/*�����_�[�^�[�Q�b�g�̃f�B�N���v�^�q�[�v*/
	static ComPtr<ID3D12Resource>				mRenderTargets[FreamBufferCount];	/*�����_�[�^�[�Q�b�g*/
	static uint									mDsvDescriptorSize;					/*�[�x�X�e���V���̃f�B�X�N���v�^�T�C�Y*/
	static ComPtr<ID3D12DescriptorHeap>			mDsvHeap;							/*�[�x�X�e���V���̃f�B�X�N���v�^�q�[�v*/
	static ComPtr<ID3D12Resource>				mDepthStencilBuffer;				/*�[�x�X�e���V���o�b�t�@*/
	static ID3D12Resource*						mCurrentRenderTarget;				/*���݂̃t���[���̃����_�[�^�[�Q�b�g���ꎞ�I�ɕۑ����Ă����֐�*/

	/*�R���X�^���g�o�b�t�@*/
	static Buffer::Constant* mMatrix;

	/*���[�g�V�O�j�`��*/
	static RootSignature*						mRootSignature;

	/*�p�C�v���C��*/
	static PipeLineState*						mPipelineState;

private:

	static cbool CreateDevice();			/*�f�o�C�X�𐶐�*/
	static cbool CreateCommandQueue();		/*�R�}���h�L���[�𐶐�*/
	static cbool CreateSwapChain();			/*�X���b�v�`�F�[���𐶐�*/
	static cbool CreateCommandList();		/*�R�}���h���X�g�ƃR�}���h�A���P�[�^�[�𐶐�*/
	static cbool CreateFence();				/*�t�F���X�𐶐�*/
	static void CreateViewPort();			/*�r���[�|�[�g�𐶐�*/
	static void CreateScissorRect();		/*�V�U�[�Z�`�𐶐�*/
	static cbool CreateRenderTarget();		/*�����_�[�^�[�Q�b�g�𐶐�*/
	static cbool CretaeDepthStenccil();		/*�[�x�X�e���V���o�b�t�@�𐶐�*/
	static void WaitRender();				/*�`�抮����҂���*/
	static cbool CreateSampler();			/*�T���v���[�X�e�[�g�𐶐�*/

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