#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* Enemy�N���X�F�G�l�~�[�̃C���X�^���X�N���X
********************************************************************************************/

#include"GameObject.h"

class Enemy : public GameObject
{
private:



public:
	
	void Init()override;
	void Start()override;
	void Update()override;

	void OnCollisionEnter(crint id)override;
	void OnCollisionExit(crint id)override;
	void OnCollisionStay(crint id)override;

	cver3 GetOffset()const override { return Vector3::up() * 3.0f; }

};