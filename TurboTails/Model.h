#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* Modelクラス：objファイルのモデルを読み込むクラス
********************************************************************************************/

#include"Renderer.h"
#include"Component.h"

// マテリアル構造体
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView* Texture;

};

// 描画サブセット構造体
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};

// モデル構造体
struct MODEL
{
	VERTEX_3D* VertexArray;
	unsigned int	VertexNum;

	unsigned int* IndexArray;
	unsigned int	IndexNum;

	SUBSET* SubsetArray;
	unsigned int	SubsetNum;
};

class Model : public Component
{
private:

	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;

	SUBSET* mSubsetArray;
	unsigned int	mSubsetNum;

	uint mNumDraw = 0;
	uint mNumVertex = 0;

	void LoadObj(const char* FileName, MODEL* Model);
	void LoadMaterial(const char* FileName, MODEL_MATERIAL** MaterialArray, unsigned int* MaterialNum);

public:

	Model() {}
	Model(crstring file) { Load(file.c_str()); }
	Model(crstring file, crint num);

	void Init(const char* FileName);
	void Draw()override;
	void Draw(cruint num);
	void NumDraw();
	void Uninit()override;

	void Load(const char* FileName);
	void Load(const char* file, const char* tex);
	void Unload();

	void Set(crint set)override { mNumDraw = set; }

};