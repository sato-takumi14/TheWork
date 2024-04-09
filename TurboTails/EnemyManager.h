#pragma once

#include"Manager.h"

class EnemyManager : public Manager
{

public:

	EnemyManager() {}

	void Init()override;
	void Update()override;
	void Uninit()override;

};