#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F09/25
*
********************************************************************************************
* Post
********************************************************************************************/

#include"GameObject.h"

class Post : public GameObject
{
private:

	ID3D11Buffer* mVertexBuffer{};

	ID3D11VertexShader* mVertexShader{};	//���_�V�F�[�_�[�I�u�W�F�N�g
	ID3D11PixelShader* mPixelShader{};		//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ID3D11InputLayout* mVertexLayout{};		//���_���C�A�E�g�I�u�W�F�N�g

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