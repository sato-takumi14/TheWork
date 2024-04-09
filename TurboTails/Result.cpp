
#include"Main.h"

#include "Result.h"
#include"Polygon2D.h"
#include"Post.h"
#include"Skydome.h"

void Result::Init()
{

	Scene::Init();

	{
		int index = 0;
		mLayer.AddLayer(Order{ RendererPath::beginbg,index++,"bg" });
		mLayer.AddLayer(Order{ RendererPath::beginbg,index++,"sky" });
		mLayer.AddLayer(Order{ RendererPath::beginpp,index++,"2D" });
		mLayer.AddLayer(Order{ RendererPath::begin,index++,"post" });
	}

	{
		AddGameObject(new Camera(true));
		AddGameObject(new Skydome());
		AddGameObject(new Post());
	}

}
