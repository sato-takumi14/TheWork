
#include "Main.h"
#include "Polygon2D.h"

#include"System\Renderer.h"
#include"System\Shader.h"

void Polygon2D::Init()
{

	Vertex3D vertex[4];

	vertex[0].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertex[0].Normal = Vector3::Zero();
	vertex[0].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertex[1].Normal = Vector3::Zero();
	vertex[1].Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertex[2].Normal = Vector3::Zero();
	vertex[2].Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertex[3].Normal = Vector3::Zero();
	vertex[3].Diffuse = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	mLayer.push_back("2D");

	mShader = new Shader();
	mShader->CreateVertexShader(L"Shader\\UnlitTextureVS.cso");
	mShader->CreatePixelShader(L"Shader\\UnlitTexturePS.cso");
	mShader->Create();
	assert(mShader->AllValid());

	mVertex = new Buffer::Vertex(sizeof(Vertex3D) * std::size(vertex), sizeof(Vertex3D), vertex);

	mTexture = new Texture::Texture();
	mTexture->Load("asset\\texture\\ロゴ.png");
	mDescriptorHeap = new Texture::DescriptorHeap();
	mHandle = mDescriptorHeap->Register(mTexture);

	/*インデックスバッファの生成*/
	/*uint32_t index[] = { 0,1,2,0,2,3 };
	mIndex = new Buffer::Index(sizeof(uint32_t) * size(index), index);*/

}

void Polygon2D::Start()
{
}

void Polygon2D::Update()
{
	//mTransform.rot.x += 0.01f;
}

void Polygon2D::Draw()
{

	auto commandlist = Renderer::GetCommandList();

	/*commandlist->SetGraphicsRootSignature(mRootSignature->GetRootSignature());
	commandlist->SetPipelineState(mPipeline->GetPipleLineState());*/
	commandlist->SetGraphicsRootSignature(mShader->GetRootSignature());
	commandlist->SetPipelineState(mShader->GetPipelineState());
	auto world = DirectX::XMMatrixRotationX(mTransform.rot.x);
	//auto world = DirectX::XMMatrixIdentity();
	Renderer::SetWorldMatrix(&world);

	commandlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	commandlist->IASetVertexBuffers(0, 1, mVertex->GetView());
	//commandlist->IASetIndexBuffer(mIndex->GetView());

	auto heap = mDescriptorHeap->GetHeap();
	commandlist->SetDescriptorHeaps(1, &heap);
	commandlist->SetGraphicsRootDescriptorTable(1, mHandle->mHandleGPU);

	commandlist->DrawInstanced(4, 1, 0, 0);
	//commandlist->DrawIndexedInstanced(6, 1, 0, 0, 0);

}

void Polygon2D::Uninit()
{
	if (mShader) {
		delete mShader;
		mShader = nullptr;
	}
	if (mVertex) {
		delete mVertex;
		mVertex = nullptr;
	}
	if (mIndex) {
		delete mIndex;
		mIndex = nullptr;
	}
	if (mTexture) {
		delete mTexture;
		mTexture = nullptr;
	}
	if (mDescriptorHeap) {
		delete mDescriptorHeap;
		mDescriptorHeap = nullptr;
	}
}
