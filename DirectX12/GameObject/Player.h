#pragma once

/***********************************************************************************************************
*
*	Player
*
*																				����ҁF������
*																				������F02/28
*
************************************************************************************************************
* �v���C���[����������N���X
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