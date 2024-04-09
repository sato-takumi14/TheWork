#pragma once

/***********************************************************************************************************
*
*	Polygon2D
*
*																				����ҁF������
*																				������F02/24
*
************************************************************************************************************
* 2D�|���S����`�悷��N���X
************************************************************************************************************/

#include"Interface\GameObject.h"
#include"Interface\Interface.h"
#include"System\Texture.h"
#include"System\Buffer.h"

class Polygon2D : public GameObject::TwoD, public Interface
{
private:

	/*class PipeLineState* mPipeline{};
	class RootSignature* mRootSignature{};*/

	class Shader* mShader{};

	Buffer::Vertex* mVertex{};
	Buffer::Index* mIndex{};

	Texture::Texture* mTexture{};
	Texture::DescriptorHeap* mDescriptorHeap{};
	Texture::DescriptorHandle* mHandle{};

public:

	void Init()override;
	void Start()override;
	void Update()override;
	void Draw()override;
	void Uninit()override;

};