#include "Main.h"
#include "Player.h"
#include"System\Shader.h"
#include"System\Renderer.h"
#include"System\Debug.h"

void Player::Init()
{

	mShader = new Shader();
	mShader->CreateVertexShader(L"Shader\\UnlitTextureVS.cso");
	mShader->CreatePixelShader(L"Shader\\UnlitTexturePS.cso");
	mShader->Create();
	assert(mShader->AllValid());

	mModel = new Assimp::Model(mShader);
	mModel->Load("asset\\model\\TurboTails.fbx");

	mLayer.push_back("3D");

}

void Player::Update()
{

}

void Player::Draw()
{

	auto commandlist = Renderer::GetCommandList();

	commandlist->SetGraphicsRootSignature(mShader->GetRootSignature());
	commandlist->SetPipelineState(mShader->GetPipelineState());
	auto world = DirectX::XMMatrixIdentity();
	Renderer::SetWorldMatrix(&world);

	mModel->Draw();
}

void Player::Uninit()
{

	if (mModel) {
		mModel->Unload();
		delete mModel;
		mModel = nullptr;
	}
	if (mShader) {
		delete mShader;
		mShader = nullptr;
	}

}
