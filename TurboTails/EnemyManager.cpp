
#include"Main.h"

#include "EnemyManager.h"

#include"Scene.h"
#include"File.h"
#include"Input.h"
#include"Enemy.h"
#include"CreateInstance.h"
#include"Time.h"

void EnemyManager::Init()
{
	CreateInstance::AddGameObject<Enemy>("Enemy");
}

void EnemyManager::Update()
{



}

void EnemyManager::Uninit()
{
	Manager::Uninit();
}
