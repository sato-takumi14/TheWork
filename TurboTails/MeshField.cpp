
#include"main.h"
#include"renderer.h"
#include"MeshField.h"
#include"File.h"
#include"Scene.h"

#include<cstdlib>
#include<ctime>
#include<cmath>
#include<random>

void MeshField::Init()
{

	// 頂点バッファ生成
	{

		mSize = Vector2::one() * 8.0f;
		mWH = Vector2::one();
		mWH.x *= (FieldSizeX / 2) - 1;
		mWH.y *= (FieldSizeZ / 2) - 1;

		FieldHeightData data = File::JsonLoadTag<FieldHeightData>("asset\\stage\\field.json", mScene->GetName());

		int indexX = 0, indexZ = 0;
		for (auto& z : data.height) {

			for (auto& x : z) {

				mMeshVertex[indexZ][indexX].Position =
					ver3((indexZ - mWH.x) * mSize.x, x, (indexX - mWH.y) * -mSize.y);
				mMeshVertex[indexZ][indexX].Normal = Vector3::up();
				mMeshVertex[indexZ][indexX].Diffuse = ver4(1.0f, 1.0f, 1.0f, 1.0f);
				mMeshVertex[indexZ][indexX].TexCoord = ver2(indexZ * 0.5f, indexX * 0.5f);

				indexX++;

			}
			indexX = 0;
			indexZ++;
		}

		//法線ベクトル
		for (int x = 1; x < FieldSizeZ - 1; x++) {

			for (int z = 1; z < FieldSizeX - 1; z++) {

				ver3 vx, vz, vn;
				vx = mMeshVertex[x + 1][z].Position - mMeshVertex[x - 1][z].Position;
				vz = mMeshVertex[x][z - 1].Position - mMeshVertex[x][z + 1].Position;

				D3DXVec3Cross(&vn, &vz, &vx);
				D3DXVec3Normalize(&vn, &vn);
				mMeshVertex[x][z].Normal = vn;

			}

		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * FieldSizeX * FieldSizeZ;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = mMeshVertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &mVertexBuffer);

	}

	// インデックスバッファ生成
	{
		//(22*2) * (20-2)
		unsigned int index[((FieldSizeX + 1) * 2) * (FieldSizeZ - 1) - 2];
		
		int i = 0;
		for (int x = 0; x < FieldSizeX - 1; x++)
		{
			for (int z = 0; z < FieldSizeZ; z++)
			{
				index[i] = x * FieldSizeX + z;
				i++;

				index[i] = (x + 1) * FieldSizeX + z;
				i++;
			}

			if (x == FieldSizeX - 2)
				break;

			index[i] = (x + 1) * FieldSizeX + (FieldSizeZ - 1);
			i++;

			index[i] = (x + 1) * FieldSizeX;
			i++;
		}



		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * (((FieldSizeX + 1) * 2) * (FieldSizeZ - 1) - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &mIndexBuffer);
	}




	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset\\texture\\芝生.jpg",
		NULL,
		NULL,
		&mTexture,
		NULL);
	assert(mTexture);

	Renderer::CreateVertexShader(&mVertexShader, &mVertexLayout, "shader\\DepthVS.cso");
	Renderer::CreatePixelShader(&mPixelShader, "shader\\PcfPS.cso");

	mLayer.emplace_back("3D");
	mTag = "MeshField";
	mClass = "MeshField";

}


void MeshField::Uninit()
{

	mVertexBuffer->Release();
	mIndexBuffer->Release();
	mTexture->Release();

	mVertexLayout->Release();
	mVertexShader->Release();
	mPixelShader->Release();

}


void MeshField::Update()
{

}


void MeshField::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(mVertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(mVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(mPixelShader, NULL, 0);


	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&rot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&trans, 0.0f, 0.0f, 0.0f);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &mTexture);
	auto shadow = Renderer::GetDepthShadowTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &shadow);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->DrawIndexed(((FieldSizeX + 1) * 2) * (FieldSizeZ - 1) - 2, 0, 0);

}

cfloat MeshField::GetHeight(crver3 pos)const
{
	int x, z;

	x = static_cast<int>(pos.x / mSize.x + mWH.x);
	z = static_cast<int>(pos.z / -mSize.y + mWH.y);

	ver3 pos0, pos1, pos2, pos3;

	pos0 = mMeshVertex[x][z].Position;
	pos1 = mMeshVertex[x + 1][z].Position;
	pos2 = mMeshVertex[x][z + 1].Position;
	pos3 = mMeshVertex[x + 1][z + 1].Position;

	ver3 v12, v1p, c;
	v12 = pos2 - pos1;
	v1p = pos - pos1;

	D3DXVec3Cross(&c, &v12, &v1p);

	float py;
	ver3 n;
	if (c.y > 0.0f) {
		//左上ポリゴン
		ver3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);
	}
	else {
		//右下ポリゴン
		ver3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);
	}

	//高さ取得
	py = -((pos.x - pos1.x) * n.x + (pos.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;

}

MeshField::PerlinNoise::PerlinNoise()
{

	mSize = 256;
	mData.resize(mSize);

	int i = 0;
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	for (auto& data : mData) {
		data = std::rand() % mSize;
	}

	random_device rd;
	mt19937 mt(rd());

	shuffle(mData.begin(), mData.end(), mt);

}

MeshField::PerlinNoise::PerlinNoise(crint x, crint z)
{

	mSize = x * z;
	mData.resize(mSize);

	int i = 0;
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	for (auto& data : mData) {
		data = std::rand() % mSize;
	}

	random_device rd;
	mt19937 mt(rd());

	shuffle(mData.begin(), mData.end(), mt);

}

cdouble MeshField::PerlinNoise::Fade(crdouble t)
{
	return static_cast<double>(t * t * t * (t * (t * 6 - 15) + 10));
}

cdouble MeshField::PerlinNoise::Lerp(crdouble t, crdouble a, crdouble b)
{
	return static_cast<double>(a + t * (b - a));
}

cdouble MeshField::PerlinNoise::Grad(crint hash, crdouble x, crdouble y, crdouble z)
{
	int h = hash & 15;
	double u = h < 8 ? x : y;
	double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return static_cast<double>(((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v));
}

cdouble MeshField::PerlinNoise::Noise(double x, double y, double z)
{
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;
	int Z = (int)floor(z) & 255;

	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	double u = Fade(x);
	double v = Fade(y);
	double w = Fade(z);

	int A = mData[X] + Y, AA = mData[A] + Z, AB = mData[A + 1] + Z;
	int B = mData[X + 1] + Y, BA = mData[B] + Z, BB = mData[B + 1] + Z;

	return Lerp(w, Lerp(v, Lerp(u, Grad(mData[AA], x, y, z),
		Grad(mData[BA], x - 1, y, z)),
		Lerp(u, Grad(mData[AB], x, y - 1, z),
			Grad(mData[BB], x - 1, y - 1, z))),
		Lerp(v, Lerp(u, Grad(mData[AA + 1], x, y, z - 1),
			Grad(mData[BA + 1], x - 1, y, z - 1)),
			Lerp(u, Grad(mData[AB + 1], x, y - 1, z - 1),
				Grad(mData[BB + 1], x - 1, y - 1, z - 1))));
}
