#pragma once

/***********************************************************************************************************
*
*	Shader
*
*																				制作者：佐藤匠
*																				制作日：02/24
*
************************************************************************************************************
* シェーダーファイルを読み込むためのクラス
************************************************************************************************************/

#include"Main.h"

class Shader
{
private:

	/*ルートシグネチャ*/
	bool mIsRootSignatureValid = false;					/*ルートシグネチャの生成を成功したかどうか*/
	ComPtr<ID3D12RootSignature> mRootSignature{};		/*ルートシグネチャ*/

	/*パイプライン*/
	bool mIsPipelineValid = false;						/*パイプラインの生成を成功したかどうか*/
	D3D12_GRAPHICS_PIPELINE_STATE_DESC mDesc{};			/*パイプラインステートの設定*/
	ComPtr<ID3D12PipelineState> mPipelineState{};		/*パイプラインステート*/
	ComPtr<ID3DBlob> mVsBlob{};							/*頂点シェーダー*/
	ComPtr<ID3DBlob> mPsBlob{};							/*ピクセルシェーダー*/

public:

	Shader();
	Shader(const wstring& vertex, const wstring& pixel);

	/*ルートシグネチャの生成を成功したかどうかを取得*/
	inline cbool GetIsRootSignatureValid()const { return mIsRootSignatureValid; }
	/*パイプラインの生成を成功したかどうかを取得*/
	inline cbool GetIsPipelineValid()const { return mIsPipelineValid; }
	/*両方成功したかどうか*/
	inline cbool AllValid()const { return mIsRootSignatureValid && mIsPipelineValid; }

	/*ルートシグネチャを取得*/
	inline ID3D12RootSignature* GetRootSignature() { return mRootSignature.Get(); }
	/*頂点シェーダーを生成*/
	void CreateVertexShader(const wstring& file);
	/*ピクセルシェーダーを生成*/
	void CreatePixelShader(const wstring& file);
	/*パイプラインを生成*/
	void Create();

	/*パイプラインを取得*/
	inline ID3D12PipelineState* GetPipelineState() { return mPipelineState.Get(); }

};
