#pragma once

/***********************************************************************************************************
*
*	Camera
*
*																				����ҁF������
*																				������F02/19
*
************************************************************************************************************
* �J�����N���X
************************************************************************************************************/

#include"Interface\Interface.h"
#include"Interface\GameObject.h"

class Camera : public GameObject::ThreeD, public Interface
{
private:

	GameObject::Interface* mTarget{};
	matrix mView{};
	matrix mProjection{};

public:

	Camera(crbool main = false);

	void Init()override;
	void Start()override;
	void Update()override;
	void Draw()override;
	void Uninit()override;

	void SetObject(GameObject::Interface* obj)override { mTarget = obj; }

};