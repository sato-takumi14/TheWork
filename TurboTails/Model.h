#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* Model�N���X�Fobj�t�@�C���̃��f����ǂݍ��ރN���X
********************************************************************************************/

#include"Renderer.h"
#include"Component.h"

// �}�e���A���\����
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView* Texture;

};

// �`��T�u�Z�b�g�\����
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};

// ���f���\����
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