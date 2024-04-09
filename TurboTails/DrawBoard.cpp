
#include"main.h"

#include "DrawBoard.h"
#include"GameObject.h"
#include"TextureManager.h"

DrawBoard::DrawBoard(
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
	mMultiple = set;

	{

		VERTEX_3D ver[4];

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

	mTexIDs = tex;

}

void DrawBoard::Draw()
{

	if (!mDraw)return;

	SetShader();

	{

		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		//material.TextureEnable = true;
		material.Diffuse = color(mColor.x, mColor.y, mColor.z, mColor.w);
		Renderer::SetMaterial(material);

	}
	
	if (!mMultiple) {

		ver3 pos = mTarget->GetPosition();

		{

			auto info = mTarget->GetDrawInfo();

			//頂点設定
			D3D11_MAPPED_SUBRESOURCE msr;
			Renderer::GetDeviceContext()->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			VERTEX_3D* ver = (VERTEX_3D*)msr.pData;

			ver[0].Position = ver3(pos.x - info.wh.x, pos.y + info.wh.y, pos.z + info.wh.z);
			ver[0].Normal = mNormal;
			ver[0].Diffuse = mColor;
			ver[0].TexCoord = ver2(info.u.x, info.v.x);

			ver[1].Position = ver3(pos.x + info.wh.x, pos.y + info.wh.y, pos.z + info.wh.z);
			ver[1].Normal = mNormal;
			ver[1].Diffuse = mColor;
			ver[1].TexCoord = ver2(info.u.y, info.v.x);

			ver[2].Position = ver3(pos.x - info.wh.x, pos.y - info.wh.y, pos.z - info.wh.z);
			ver[2].Normal = mNormal;
			ver[2].Diffuse = mColor;
			ver[2].TexCoord = ver2(info.u.x, info.v.y);

			ver[3].Position = ver3(pos.x + info.wh.x, pos.y - info.wh.y, pos.z - info.wh.z);
			ver[3].Normal = mNormal;
			ver[3].Diffuse = mColor;
			ver[3].TexCoord = ver2(info.u.y, info.v.y);

			Renderer::GetDeviceContext()->Unmap(mVertexBuffer, 0);

		}

		{

			auto scale = mTarget->GetScale();
			auto rot = mTarget->GetRotation();

			//マトリックス設定
			D3DXMATRIX World, Scale, Rot, Trans;
			D3DXMatrixScaling(&Scale, scale.x, scale.y, scale.z);
			D3DXMatrixRotationYawPitchRoll(&Rot, rot.y, rot.x, rot.z);
			D3DXMatrixTranslation(&Trans, pos.x, pos.y, pos.z);
			World = Scale * Rot * Trans;
			Renderer::SetWorldMatrix(&World);


		}

		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

		int index{};
		for (auto& data : mTexIDs) {
			Renderer::GetDeviceContext()->PSSetShaderResources(index++, 1, TextureManager::GetTexture(data));
		}

		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		Renderer::SetATCEnable(true);

		Renderer::GetDeviceContext()->Draw(4, 0);

		Renderer::SetATCEnable(false);

	}
	else {

		auto infos = mTarget->GetDrawInfos();

		int index{};
		for (auto& data : mTexIDs) {
			Renderer::GetDeviceContext()->PSSetShaderResources(index++, 1, TextureManager::GetTexture(data));
		}

		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		{

			Renderer::SetATCEnable(true);

			for (auto& info : infos) {

				D3D11_MAPPED_SUBRESOURCE msr;
				Renderer::GetDeviceContext()->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
				VERTEX_3D* ver = (VERTEX_3D*)msr.pData;

				ver[0].Position = ver3(info.transform.pos.x - info.wh.x, info.transform.pos.y + info.wh.y, info.transform.pos.z + info.wh.z);
				ver[0].Normal = mNormal;
				ver[0].Diffuse = mColor;
				ver[0].TexCoord = ver2(info.u.x, info.v.x);

				ver[1].Position = ver3(info.transform.pos.x + info.wh.x, info.transform.pos.y + info.wh.y, info.transform.pos.z + info.wh.z);
				ver[1].Normal = mNormal;
				ver[1].Diffuse = mColor;
				ver[1].TexCoord = ver2(info.u.y, info.v.x);

				ver[2].Position = ver3(info.transform.pos.x - info.wh.x, info.transform.pos.y - info.wh.y, info.transform.pos.z - info.wh.z);
				ver[2].Normal = mNormal;
				ver[2].Diffuse = mColor;
				ver[2].TexCoord = ver2(info.u.x, info.v.y);

				ver[3].Position = ver3(info.transform.pos.x + info.wh.x, info.transform.pos.y - info.wh.y, info.transform.pos.z - info.wh.z);
				ver[3].Normal = mNormal;
				ver[3].Diffuse = mColor;
				ver[3].TexCoord = ver2(info.u.y, info.v.y);

				Renderer::GetDeviceContext()->Unmap(mVertexBuffer, 0);

			}

			const int size = static_cast<int>(infos.size());

			Renderer::GetDeviceContext()->DrawInstanced(4, size, 0, 0);

			Renderer::SetATCEnable(false);

		}

	}

}

void DrawBoard::Uninit()
{
	if (mVertexBuffer) {
		mVertexBuffer->Release();
	}
	mTexIDs.clear();
	DrawInterface::Uninit();
}
