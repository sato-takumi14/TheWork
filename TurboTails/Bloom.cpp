#include "main.h"
#include "Bloom.h"
#include"Shader.h"

void Bloom::Init()
{

	mLayer.push_back("bloom");

}

void Bloom::Start()
{

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

	Shader::AddShader("Bloom,", "BloomVS.cso", "BloomPS.cso");

}

void Bloom::Update()
{



}

void Bloom::Draw()
{

	auto device = Renderer::GetDeviceContext();

	Renderer::SetWorldViewProjection2D();

	for (int i = 0; i < 4; i++) {

		Shader::SetShader("Bloom");

		Renderer::BeginBloom(i);
		Renderer::SetBloomViewport(i);

		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		device->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

		device->PSSetShaderResources(0, 1, Renderer::GetLuminaceTexture());

		device->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		device->Draw(4, 0);

	}

}

void Bloom::Uninit()
{
}
