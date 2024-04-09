
#include"Main.h"

#include<algorithm>

#include "UIManager.h"

#include"Scene.h"
#include"ConcentrationLine.h"
#include"JewelUI.h"
#include"File.h"
#include"CreateInstance.h"
#include"Polygon2D.h"
#include"Score.h"
#include"DashGauge.h"
#include"MouseUI.h"
#include"SelectButton.h"

void UIManager::Init()
{
	
	mFilePath += "\\UIManager";

	CreateInstance::AddGameObject<ConcentrationLine>("line");
	CreateInstance::AddGameObject<JewelUI>("JewelUI");
	CreateInstance::AddGameObject<Score>("Score");
	CreateInstance::AddGameObject<DashGauge>("DashGauge");
	CreateInstance::AddGameObject<MouseUI>("MouseUI");
	CreateInstance::AddGameObject<SelectButton>("SelectButton");

	mData = File::JsonLoadTag<list<UI>>("asset\\stage\\ui.json", mScene->GetName());

	for (auto& data : mData) {
		
		mUI[data.name] = mScene->AddGameObject(CreateInstance::CreateObject(data.classname));

	}

	mTag = "UIManager";

#if _DEBUG



#endif // _DEBUG


}

void UIManager::Update()
{



}

void UIManager::Uninit()
{

	mUI.clear();

}
