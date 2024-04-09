
#include"main.h"

#include "DrawBillboard.h"
#include"GameObject.h"
#include"TextureManager.h"
#include"SceneManager.h"
#include"Scene.h"

DrawBillBoard::DrawBillBoard(
	GameObject* target, 
	crstring vertex, 
	crstring pixel, 
	const list<int> tex, 
	crver4 color,
	crbool set)
{

	LoadShader(vertex, pixel);

	mTarget = target;
	mCamera = SceneManager::GetScene()->GetMainCamera();
	mTexIDs = tex;
	mColor = color;
	mMultiple = set;

	{
		/*頂点設定*/
		ver3 pos = mTarget->GetPosition();

		auto info = mTarget->GetDrawInfo();
		ver3 normal = Vector3::back();

		VERTEX_3D ver[4];

		ver[0].Position = ver3(pos.x - info.wh.x, pos.y + info.wh.y, pos.z + info.wh.z);
		ver[0].Normal = normal;
		ver[0].Diffuse = mColor;
		ver[0].TexCoord = ver2(info.u.x, info.v.x);

		ver[1].Position = ver3(pos.x + info.wh.x, pos.y + info.wh.y, pos.z + info.wh.z);
		ver[1].Normal = normal;
		ver[1].Diffuse = mColor;
		ver[1].TexCoord = ver2(info.u.y, info.v.x);

		ver[2].Position = ver3(pos.x - info.wh.x, pos.y - info.wh.y, pos.z - info.wh.z);
		ver[2].Normal = normal;
		ver[2].Diffuse = mColor;
		ver[2].TexCoord = ver2(info.u.x, info.v.y);

		ver[3].Position = ver3(pos.x + info.wh.x, pos.y - info.wh.y, pos.z - info.wh.z);
		ver[3].Normal = normal;
		ver[3].Diffuse = mColor;
		ver[3].TexCoord = ver2(info.u.y, info.v.y);

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

void DrawBillBoard::Draw()
{

	if (!mDraw)return;

	SetShader();

	{

		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = color(mColor.x, mColor.y, mColor.z, mColor.w);
		Renderer::SetMaterial(material);

	}

	ver3 pos = mTarget->GetPosition();

	if (!mMultiple) {

		{

			auto info = mTarget->GetDrawInfo();
			ver3 normal = Vector3::back();

			/*頂点設定*/
			D3D11_MAPPED_SUBRESOURCE msr;
			Renderer::GetDeviceContext()->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			VERTEX_3D* ver = (VERTEX_3D*)msr.pData;

			ver[0].Position = ver3(pos.x - info.wh.x, pos.y + info.wh.y, pos.z + info.wh.z);
			ver[0].Normal = normal;
			ver[0].Diffuse = mColor;
			ver[0].TexCoord = ver2(info.u.x, info.v.x);

			ver[1].Position = ver3(pos.x + info.wh.x, pos.y + info.wh.y, pos.z + info.wh.z);
			ver[1].Normal = normal;
			ver[1].Diffuse = mColor;
			ver[1].TexCoord = ver2(info.u.y, info.v.x);

			ver[2].Position = ver3(pos.x - info.wh.x, pos.y - info.wh.y, pos.z - info.wh.z);
			ver[2].Normal = normal;
			ver[2].Diffuse = mColor;
			ver[2].TexCoord = ver2(info.u.x, info.v.y);

			ver[3].Position = ver3(pos.x + info.wh.x, pos.y - info.wh.y, pos.z - info.wh.z);
			ver[3].Normal = normal;
			ver[3].Diffuse = mColor;
			ver[3].TexCoord = ver2(info.u.y, info.v.y);

			Renderer::GetDeviceContext()->Unmap(mVertexBuffer, 0);

		}

		{

			D3DXMATRIX view = mCamera->GetMatrix();

			/*ビューの逆行列*/
			D3DXMATRIX invView;
			D3DXMatrixInverse(&invView, NULL, &view);
			invView._41 = 0.0f;
			invView._42 = 0.0f;
			invView._43 = 0.0f;			

			auto scale = mTarget->GetScale();

			/*マトリックス設定*/
			D3DXMATRIX World, Scale, Trans;
			D3DXMatrixScaling(&Scale, scale.x, scale.y, scale.z);
			D3DXMatrixTranslation(&Trans, pos.x, pos.y, pos.z);
			World = Scale * invView * Trans;
			Renderer::SetWorldMatrix(&World);

		}

		{

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

	}
	else {

		auto infos = mTarget->GetDrawInfos();

		int index{};
		for (auto& data : mTexIDs) {
			Renderer::GetDeviceContext()->PSSetShaderResources(index++, 1, TextureManager::GetTexture(data));
		}

		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		D3DXMATRIX view = mCamera->GetMatrix();

		/*ビューの逆行列*/
		D3DXMATRIX invView;
		D3DXMatrixInverse(&invView, NULL, &view);
		invView._41 = 0.0f;
		invView._42 = 0.0f;
		invView._43 = 0.0f;

		for (auto& info : infos) {

			{

				/*頂点設定*/

				ver3 normal = Vector3::back();

				D3D11_MAPPED_SUBRESOURCE msr;
				Renderer::GetDeviceContext()->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
				VERTEX_3D* ver = (VERTEX_3D*)msr.pData;

				ver[0].Position = ver3(pos.x - info.wh.x, pos.y + info.wh.y, pos.z + info.wh.z);
				ver[0].Normal = normal;
				ver[0].Diffuse = mColor;
				ver[0].TexCoord = ver2(info.u.x, info.v.x);

				ver[1].Position = ver3(pos.x + info.wh.x, pos.y + info.wh.y, pos.z + info.wh.z);
				ver[1].Normal = normal;
				ver[1].Diffuse = mColor;
				ver[1].TexCoord = ver2(info.u.y, info.v.x);

				ver[2].Position = ver3(pos.x - info.wh.x, pos.y - info.wh.y, pos.z - info.wh.z);
				ver[2].Normal = normal;
				ver[2].Diffuse = mColor;
				ver[2].TexCoord = ver2(info.u.x, info.v.y);

				ver[3].Position = ver3(pos.x + info.wh.x, pos.y - info.wh.y, pos.z - info.wh.z);
				ver[3].Normal = normal;
				ver[3].Diffuse = mColor;
				ver[3].TexCoord = ver2(info.u.y, info.v.y);

				Renderer::GetDeviceContext()->Unmap(mVertexBuffer, 0);

			}

			{

				/*マトリックス設定*/
				D3DXMATRIX World, Scale, Rot, Trans;
				D3DXMatrixScaling(&Scale, info.transform.scale.x, info.transform.scale.y, info.transform.scale.z);
				D3DXMatrixTranslation(&Trans, info.transform.pos.x, info.transform.pos.y, info.transform.pos.z);
				World = Scale * invView * Trans;
				Renderer::SetWorldMatrix(&World);

			}

			{

				UINT stride = sizeof(VERTEX_3D);
				UINT offset = 0;
				Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

				Renderer::GetDeviceContext()->Draw(4, 0);

			}

		}

	}	

}

void DrawBillBoard::Uninit()
{
	DrawInterface::Uninit();
	if (mVertexBuffer) {
		mVertexBuffer->Release();
	}
}
