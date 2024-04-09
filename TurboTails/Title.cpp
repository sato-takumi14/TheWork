
#include"Main.h"

#include "Title.h"

#include"TestScene.h"

#include"Input.h"
#include"Camera.h"
#include"Polygon2D.h"
#include"SceneChangeObject.h"
#include"Post.h"
#include"LoadScene.h"
#include"DebugCamera.h"
#include"Skydome.h"
#include"TitleLogo.h"
#include"MeshField.h"
#include"TitlePlayer.h"

void Title::Init()
{

	Scene::Init();

	SetStageFilePath("title");
	SetName("title");

	/*アクションキー設定*/
	{
		/*キーボード*/
		ActionKey::AddAction("NextScene", [](const BYTE& key) { return Input::GetKeyTrigger(key); }, VK_SPACE);

		/*ゲームパッド*/

		/*マウス*/
		ActionKey::AddAction("NextScene", [](const BYTE& key) { return Input::IsMouseLeftTriggered(key); }, NULL);

	}

	{
		int index = 0;
		mLayer.AddLayer(Order{ RendererPath::begindepth,index++,"depth" });
		mLayer.AddLayer(Order{ RendererPath::beginbg,index++,"bg" });
		mLayer.AddLayer(Order{ RendererPath::beginpp,index++,"sky" });
		mLayer.AddLayer(Order{ RendererPath::beginpp,index++,"3D" });
		mLayer.AddLayer(Order{ RendererPath::beginpp,index++,"2D" });
		mLayer.AddLayer(Order{ RendererPath::begin,index++,"post" });
	}

	AddGameObject(new Camera(true));
	AddGameObject(new Skydome());
	AddGameObject(new TitleLogo());
	AddGameObject(new MeshField());
	AddGameObject(new TitlePlayer());

	AddGameObject(new Load("StageSelect"));
	AddGameObject(new Post());

}

void Title::Update()
{
	Scene::Update();
}
