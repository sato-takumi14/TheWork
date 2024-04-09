#pragma once

/***********************************************************************************************************
*
*	Player
*
*																				制作者：佐藤匠
*																				制作日：02/28
*
************************************************************************************************************
* プレイヤーを実装するクラス
************************************************************************************************************/

#include"Interface\GameObject.h"
#include"System\Buffer.h"

#include"System\AssimpLoader.h"

class Player : public GameObject::ThreeD
{
private:

	Assimp::Model* mModel{};
	class Shader* mShader{};

public:

	void Init()override;
	void Update()override;
	void Draw()override;
	void Uninit()override;

};