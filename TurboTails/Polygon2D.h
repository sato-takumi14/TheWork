#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF09/25
*
********************************************************************************************
* Polygon2D
********************************************************************************************/

#include"GameObject.h"

class Polygon2D : public GameObject
{
private:

	DrawInfo mInfo{};
	bool mShadow = false;

	ID3D11Buffer* mVertexBuffer{};
	ID3D11VertexShader* mVertexShader{};
	ID3D11PixelShader* mPixelShader{};
	ID3D11InputLayout* mVertexLayout{};

public:

	Polygon2D();

	Polygon2D(
		crstring file,
		crstring tag,
		crstring vertex,
		crstring pixel,
		const DrawInfo& info,
		crbool anim = false
	);

	void Draw()override;
	void Uninit()override;

	const DrawInfo GetDrawInfo()const override { return mInfo; }

};