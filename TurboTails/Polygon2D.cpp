
#include"main.h"

#include "Polygon2D.h"

#include"Draw2D.h"
#include"DrawAnim2D.h"

#include"TextureManager.h"

Polygon2D::Polygon2D()
{

	mShadow = true;

	mTransform.pos = Vector3::windowcenter();

	VERTEX_3D ver[4];
	/*頂点設定*/
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = ver;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &mVertexBuffer);

	mLayer.push_back("2D");

	mInfo.wh = Vector3::windowcenter() * 0.5f;
	mInfo.u = mInfo.v = Vector3::up();

	Renderer::CreateVertexShader(&mVertexShader, &mVertexLayout, "shader\\UnlitTextureVS.cso");
	Renderer::CreatePixelShader(&mPixelShader, "shader\\UnlitTexturePS.cso");

}

Polygon2D::Polygon2D(
	crstring file, 
	crstring tag, 
	crstring vertex, 
	crstring pixel, 
	const DrawInfo& info,
	crbool anim)
{
	
	mInfo = info;
	mTransform.pos = mInfo.transform.pos;

	list<int> tex{};
	tex.push_back(TextureManager::Load(file, tag));

	mDraw = new Draw2D(
		this,
		vertex,
		pixel,
		Vector3::zero(),
		Vector4::one(),
		tex
	);

	mLayer.push_back("2D");
	mTag = tag;

}

void Polygon2D::Draw()
{
	if (mShadow) {

		Renderer::SetWorldViewProjection2D();

		Renderer::GetDeviceContext()->IASetInputLayout(mVertexLayout);
		Renderer::GetDeviceContext()->VSSetShader(mVertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(mPixelShader, NULL, 0);

		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = ver3(mTransform.pos.x - mInfo.wh.x, mTransform.pos.y - mInfo.wh.y, mTransform.pos.z + mInfo.wh.z);
		vertex[0].Normal = Vector3::back();
		vertex[0].Diffuse = Vector4::one();
		vertex[0].TexCoord = ver2(mInfo.u.x, mInfo.v.x);

		vertex[1].Position = ver3(mTransform.pos.x + mInfo.wh.x, mTransform.pos.y - mInfo.wh.y, mTransform.pos.z + mInfo.wh.z);
		vertex[1].Normal = Vector3::back();
		vertex[1].Diffuse = Vector4::one();
		vertex[1].TexCoord = ver2(mInfo.u.y, mInfo.v.x);

		vertex[2].Position = ver3(mTransform.pos.x - mInfo.wh.x, mTransform.pos.y + mInfo.wh.y, mTransform.pos.z - mInfo.wh.z);
		vertex[2].Normal = Vector3::back();
		vertex[2].Diffuse = Vector4::one();
		vertex[2].TexCoord = ver2(mInfo.u.x, mInfo.v.y);

		vertex[3].Position = ver3(mTransform.pos.x + mInfo.wh.x, mTransform.pos.y + mInfo.wh.y, mTransform.pos.z - mInfo.wh.z);
		vertex[3].Normal = Vector3::back();
		vertex[3].Diffuse = Vector4::one();
		vertex[3].TexCoord = ver2(mInfo.u.y, mInfo.v.y);

		Renderer::GetDeviceContext()->Unmap(mVertexBuffer, 0);

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

		auto shadow = Renderer::GetDepthShadowTexture();
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &shadow);

		// プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		Renderer::SetATCEnable(false);

		// ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);

		Renderer::SetATCEnable(false);

	}
	else GameObject::Draw();
}

void Polygon2D::Uninit()
{
	if (mShadow) {
		mVertexBuffer->Release();
		mVertexShader->Release();
		mPixelShader->Release();
		mVertexLayout->Release();
	}
}
