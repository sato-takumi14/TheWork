#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：02/16
*
********************************************************************************************
* Bloomクラス：ブルーム処理を行うためのクラス
********************************************************************************************/

#include"GameObject.h"

class Bloom : public GameObject
{
private:

	ID3D11Buffer* mVertexBuffer{};

public:

	void Init()override;
	void Start()override;
	void Update()override;
	void Draw()override;
	void Uninit()override;

};