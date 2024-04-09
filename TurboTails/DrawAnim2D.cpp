
#include"main.h"
#include "DrawAnim2D.h"
#include"GameObject.h"
#include"TextureManager.h"

void DrawAnim2D::Add()
{
	mTime += mAddTime;

	if (mTime >= 1.0f) {

		mInfo.u.x += mInfo.u.z;
		mInfo.u.y += mInfo.u.z;

		mInfo.v.x += mInfo.v.z;
		mInfo.v.y += mInfo.v.z;

		mTime = 0.0f;

	}

}

DrawAnim2D::DrawAnim2D(
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

	auto info = mTarget->GetDrawInfo();
	mInfo.u = info.u;
	mInfo.v = info.v;

	mAddTime = 1.0f / 60;

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

void DrawAnim2D::Draw()
{

	if (!mDraw)return;

	Renderer::SetWorldViewProjection2D();

	SetShader();

	{
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = color(mColor.x, mColor.y, mColor.z, mColor.w);
		Renderer::SetMaterial(material);
	}

	Add();

	if (!mMultiple) {

		auto pos = mTarget->GetPosition();
		auto info = mTarget->GetDrawInfo();

		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = ver3(pos.x - info.wh.x, pos.y - info.wh.y, pos.z + info.wh.z);
		vertex[0].Normal = mNormal;
		vertex[0].Diffuse = mColor;
		vertex[0].TexCoord = ver2(mInfo.u.x, mInfo.v.x);

		vertex[1].Position = ver3(pos.x + info.wh.x, pos.y - info.wh.y, pos.z + info.wh.z);
		vertex[1].Normal = mNormal;
		vertex[1].Diffuse = mColor;
		vertex[1].TexCoord = ver2(mInfo.u.y, mInfo.v.x);

		vertex[2].Position = ver3(pos.x - info.wh.x, pos.y + info.wh.y, pos.z - info.wh.z);
		vertex[2].Normal = mNormal;
		vertex[2].Diffuse = mColor;
		vertex[2].TexCoord = ver2(mInfo.u.x, mInfo.v.y);

		vertex[3].Position = ver3(pos.x + info.wh.x, pos.y + info.wh.y, pos.z - info.wh.z);
		vertex[3].Normal = mNormal;
		vertex[3].Diffuse = mColor;
		vertex[3].TexCoord = ver2(mInfo.u.y, mInfo.v.y);

		Renderer::GetDeviceContext()->Unmap(mVertexBuffer, 0);

		int index{};
		for (auto& data : mTexIDs) {
			// テクスチャ設定
			Renderer::GetDeviceContext()->PSSetShaderResources(index++, 1, TextureManager::GetTexture(data));
		}

		// プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);

	}
	else {

		int index{};
		for (auto& data : mTexIDs) {
			// テクスチャ設定
			Renderer::GetDeviceContext()->PSSetShaderResources(index++, 1, TextureManager::GetTexture(data));
		}

		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		auto info = mTarget->GetDrawInfos();

		mTime += mAddTime;

		for (auto& data : info) {

			if (mTime >= 1.0f) {
				data.AddU();
				data.AddV();
			}

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

	}

}

void DrawAnim2D::Uninit()
{

	DrawInterface::Uninit();

}
