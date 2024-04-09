#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：09/25
*
********************************************************************************************
* Post
********************************************************************************************/

#include"GameObject.h"

class Post : public GameObject
{
private:

	ID3D11Buffer* mVertexBuffer{};

	ID3D11VertexShader* mVertexShader{};	//頂点シェーダーオブジェクト
	ID3D11PixelShader* mPixelShader{};		//ピクセルシェーダーオブジェクト
	ID3D11InputLayout* mVertexLayout{};		//頂点レイアウトオブジェクト

	uint mTexID = -1;
	float mParam{};

	class Math::Lerp::Float* mLerp{};
	bool mLerpFlag{};
	bool mTitle{};

public:

	void Init()override;
	void Start()override;
	void Update()override;
	void Draw()override;
	void Uninit()override;

};