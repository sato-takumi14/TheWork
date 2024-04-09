
#include"main.h"

#include "SelectRod.h"

#include"TextureManager.h"
#include"Draw2D.h"
#include"Input.h"

void SelectRod::Init()
{

	list<int> tex{};
	tex.push_back(TextureManager::Load("asset\\texture\\", "SelectRod"));

	mDrawInfo.u = mDrawInfo.v = Vector3::up();

}

void SelectRod::Update()
{

	if (Input::GetKeyTrigger(VK_UP)) {

	}
	else if (Input::GetKeyTrigger(VK_DOWN)) {

	}

}
