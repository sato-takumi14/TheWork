
#include"Main.h"

#include "TestScene.h"
#include"StageManager.h"

void TestScene::Init()
{

	Scene::Init();

	SetStageFilePath("test");
	SetName("test");

	{
		int index = 0;
		mLayer.AddLayer(Order{ RendererPath::begindepth,index++,"depth" });
		mLayer.AddLayer(Order{ RendererPath::beginbg,index++,"bg" });
		mLayer.AddLayer(Order{ RendererPath::beginpp,index++,"sky" });
		mLayer.AddLayer(Order{ RendererPath::beginpp,index++,"3D" });
		mLayer.AddLayer(Order{ RendererPath::beginpp,index++,"2D" });
		mLayer.AddLayer(Order{ RendererPath::begin,index++,"post" });
	}

	AddManager(new StageManager(this->GetName()));

}
