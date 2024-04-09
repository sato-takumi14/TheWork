
#include "Main.h"
#include "TestScene.h"

#include"GameObject\Polygon2D.h"
#include"GameObject\Player.h"

void TestScene::Init()
{

	mLayer.AddLayer("3D", 0, []() {});

	AddGameobject(new Player());
	//AddGameobject(new Polygon2D());

}
