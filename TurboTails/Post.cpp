
#include"main.h"

#include "Post.h"
#include"TextureManager.h"
#include"Scene.h"
#include"State.h"

void Post::Init()
{

	mLayer.push_back("post");

	mClass = "Post";

	mParam = 0.0f;
	mLerpFlag = false;

}

void Post::Start()
{
	if (mScene->GetName() == "load" ||
		mScene->GetName() == "title")mTitle = true;
	else mTitle = false;

	{
		/*頂点設定*/
		VERTEX_3D vertex[4];

		vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

		vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		vertex[2].Position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


		// 頂点バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &mVertexBuffer);

	}

	Renderer::CreateVertexShader(&mVertexShader, &mVertexLayout, "shader\\MergeVS.cso");
	Renderer::CreatePixelShader(&mPixelShader, "shader\\MergePS.cso");

}

void Post::Update()
{

	GameObject::Update();

	if (mScene->GetName() != "title") {

		GameObject* player = mScene->GetGameObjectTag("Player");
		if (player->GetStateManager()->GetState("move")->GetName() == "Dash") {
			if (!mLerpFlag) {
				mLerpFlag = true;
				mLerp = new Math::Lerp::Float(&mParam, mParam, 0.05f, 0.002f, 1.0f);
				AddComponent(mLerp);
			}
		}
		else {
			mParam = 0.0f;
			if (mLerpFlag) {
				mLerp->SetDestroy();
				mLerpFlag = false;
			}
		}

	}

}

void Post::Draw()
{

	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.Diffuse.r = mParam;
	Renderer::SetParameter(param);

	auto device = Renderer::GetDeviceContext();

	device->IASetInputLayout(mVertexLayout);
	device->VSSetShader(mVertexShader, NULL, 0);
	device->PSSetShader(mPixelShader, NULL, 0);

	Renderer::SetWorldViewProjection2D();

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	device->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	auto tex = Renderer::GetPPTexture();
	device->PSSetShaderResources(0, 1, &tex);
	device->PSSetShaderResources(1, 1, Renderer::GetLuminaceTexture());
	for (int i = 0; i < 4; i++) {
		device->PSSetShaderResources(2 + i, 1, Renderer::GetBloomTexture(i));
	}

	device->PSSetShaderResources(6, 1, TextureManager::GetTexture(mTexID));

	device->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	device->Draw(4, 0);

}

void Post::Uninit()
{

	GameObject::Uninit();

	if (mVertexBuffer) {
		mVertexBuffer->Release();
	}
	if (mVertexShader) {
		mVertexShader->Release();
	}
	if (mPixelShader) {
		mPixelShader->Release();
	}
	if (mVertexLayout) {
		mVertexLayout->Release();
	}

}
