#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF09/25
*
********************************************************************************************
* MeshField
********************************************************************************************/

#include"GameObject.h"

cint FieldSizeX = 81;
cint FieldSizeZ = 81;

class MeshField : public GameObject
{
private:

	typedef struct FieldHeightData {

		list<list<float>> height{};

		FieldHeightData() : height(list<list<float>>()) {}

		template <class DATA>
		void serialize(DATA& data) { data(CEREAL_NVP(height)); }

	}FieldHeightData;

	class PerlinNoise
	{
	private:

		vector<int> mData;
		int mSize{};

	public:

		PerlinNoise();
		PerlinNoise(crint x, crint z);

		cdouble Fade(crdouble t);
		cdouble Lerp(crdouble t, crdouble a, crdouble b);
		cdouble Grad(crint hash, crdouble x, crdouble y, crdouble z);
		cdouble Noise(double x, double y, double z);

	};

private:

	ID3D11Buffer* mVertexBuffer{};
	ID3D11Buffer* mIndexBuffer{};
	ID3D11ShaderResourceView* mTexture{};
	ID3D11VertexShader* mVertexShader{};
	ID3D11PixelShader* mPixelShader{};
	ID3D11InputLayout* mVertexLayout{};

	VERTEX_3D mMeshVertex[FieldSizeX][FieldSizeZ]{};

	ver2 mSize{};
	ver2 mWH{};

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

	cfloat GetHeight(crver3 pos = Vector3::zero())const override;

};