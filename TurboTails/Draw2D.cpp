
#include"main.h"

#include "Draw2D.h"
#include"GameObject.h"
#include"TextureManager.h"

Draw2D::Draw2D(
	GameObject* target, 
	crstring vertex, 
	crstring pixel, 
	crver3 normal, 
	crver4 color, 
	const list<int> tex,
	crbool set)
{

	LoadShader(vertex, pixel);

	mTarget = target;
	mNormal = normal;
	mColor = color;
	mTexIDs = tex;
	mMultiple = set;

	{

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

	}

}

void Draw2D::Draw()
{

	if (!mDraw)return;

	SetShader();

	Renderer::SetWorldViewProjection2D();

	/*頂点設定*/
	if (!mMultiple) {
		auto pos = mTarget->GetPosition();
		auto info = mTarget->GetDrawInfo();

		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = ver3(pos.x - info.wh.x, pos.y - info.wh.y, pos.z + info.wh.z);
		vertex[0].Normal = mNormal;
		vertex[0].Diffuse = mColor;
		vertex[0].TexCoord = ver2(info.u.x, info.v.x);

		vertex[1].Position = ver3(pos.x + info.wh.x, pos.y - info.wh.y, pos.z + info.wh.z);
		vertex[1].Normal = mNormal;
		vertex[1].Diffuse = mColor;
		vertex[1].TexCoord = ver2(info.u.y, info.v.x);

		vertex[2].Position = ver3(pos.x - info.wh.x, pos.y + info.wh.y, pos.z - info.wh.z);
		vertex[2].Normal = mNormal;
		vertex[2].Diffuse = mColor;
		vertex[2].TexCoord = ver2(info.u.x, info.v.y);

		vertex[3].Position = ver3(pos.x + info.wh.x, pos.y + info.wh.y, pos.z - info.wh.z);
		vertex[3].Normal = mNormal;
		vertex[3].Diffuse = mColor;
		vertex[3].TexCoord = ver2(info.u.y, info.v.y);

		Renderer::GetDeviceContext()->Unmap(mVertexBuffer, 0);

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

		int index{};
		for (auto& data : mTexIDs) {
			// テクスチャ設定
			Renderer::GetDeviceContext()->PSSetShaderResources(index++, 1, TextureManager::GetTexture(data));
		}

		// プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		Renderer::SetATCEnable(true);

		// ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);

		Renderer::SetATCEnable(false);

	}
	else {

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

		int index{};
		for (auto& data : mTexIDs) {
			// テクスチャ設定
			Renderer::GetDeviceContext()->PSSetShaderResources(index++, 1, TextureManager::GetTexture(data));
		}

		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		auto info = mTarget->GetDrawInfos();

		Renderer::SetATCEnable(true);

		for (auto& data : info) {

			D3D11_MAPPED_SUBRESOURCE msr;
			Renderer::GetDeviceContext()->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

			vertex[0].Position = ver3(data.transform.pos.x - data.wh.x, data.transform.pos.y - data.wh.y, data.transform.pos.z + data.wh.z);
			vertex[0].Normal = mNormal;
			vertex[0].Diffuse = mColor;
			vertex[0].TexCoord = ver2(data.u.x, data.v.x);

			vertex[1].Position = ver3(data.transform.pos.x + data.wh.x, data.transform.pos.y - data.wh.y, data.transform.pos.z + data.wh.z);
			vertex[1].Normal = mNormal;
			vertex[1].Diffuse = mColor;
			vertex[1].TexCoord = ver2(data.u.y, data.v.x);

			vertex[2].Position = ver3(data.transform.pos.x - data.wh.x, data.transform.pos.y + data.wh.y, data.transform.pos.z - data.wh.z);
			vertex[2].Normal = mNormal;
			vertex[2].Diffuse = mColor;
			vertex[2].TexCoord = ver2(data.u.x, data.v.y);

			vertex[3].Position = ver3(data.transform.pos.x + data.wh.x, data.transform.pos.y + data.wh.y, data.transform.pos.z - data.wh.z);
			vertex[3].Normal = mNormal;
			vertex[3].Diffuse = mColor;
			vertex[3].TexCoord = ver2(data.u.y, data.v.y);

			Renderer::GetDeviceContext()->Unmap(mVertexBuffer, 0);

			Renderer::GetDeviceContext()->Draw(4, 0);

		}

		Renderer::SetATCEnable(false);

	}	

}

void Draw2D::Uninit()
{
	DrawInterface::Uninit();
	if (mVertexBuffer) {
		mVertexBuffer->Release();
	}
}
