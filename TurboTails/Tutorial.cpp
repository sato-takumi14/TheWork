
#include"Main.h"

#include "Tutorial.h"
#include"StageManager.h"

void Tutorial::Init()
{

	Scene::Init();

	SetStageFilePath("tutorial");
	SetName("tutorial");

	/*アクションキー設定*/
	{
		//キーボード
		ActionKey::AddAction("jump", [](const BYTE& key) { return Input::GetKeyTrigger(key); }, VK_SPACE);
		ActionKey::AddAction("attack", [](const BYTE& key) { return Input::GetKeyTrigger(key); }, 'F');
		ActionKey::AddAction("charge", [](const BYTE& key) { return Input::GetKeyPress(key); }, 'F');
		ActionKey::AddAction("dash", [](const BYTE& key) { return Input::GetKeyTrigger(key); }, VK_SHIFT);
		ActionKey::AddAction("forward", [](const BYTE& key) { return Input::GetKeyPress(key); }, 'W');
		ActionKey::AddAction("right", [](const BYTE& key) { return Input::GetKeyPress(key); }, 'D');
		ActionKey::AddAction("left", [](const BYTE& key) { return Input::GetKeyPress(key); }, 'A');
		ActionKey::AddAction("back", [](const BYTE& key) { return Input::GetKeyPress(key); }, 'S');
		ActionKey::AddAction("reset", [](const BYTE& key) { return Input::GetKeyTrigger(key); }, 'R');
		ActionKey::AddAction("NextScene", [](const BYTE& key) { return Input::GetKeyTrigger(key); }, 'F');
		ActionKey::AddAction("select", [](const BYTE& key) { return Input::GetKeyTrigger(key); }, 'F');

		//ゲームパッド
		ActionKey::AddGamePadAction("jump", [](const DWORD& key) { return Input::GamePadTrigger(key); }, XINPUT_GAMEPAD_A);
		ActionKey::AddGamePadAction("attack", [](const DWORD& key) { return Input::GamePadTrigger(key); }, XINPUT_GAMEPAD_X);
		ActionKey::AddGamePadAction("charge", [](const DWORD& key) { return Input::GamePadPress(key); }, XINPUT_GAMEPAD_X);
		ActionKey::AddGamePadAction("dash", [](const DWORD& key) { return Input::GamePadTrigger(key); }, XINPUT_GAMEPAD_LEFT_SHOULDER);

		//マウス
		ActionKey::AddAction("NextScene", [](const BYTE& key) { return Input::IsMouseLeftTriggered(key); }, NULL);
		ActionKey::AddAction("select", [](const BYTE& key) { return Input::IsMouseLeftTriggered(key); }, NULL);

	}

	{
		int index = 0;
		mLayer.AddLayer(Order{ RendererPath::begindepth,index++,"depth" });
		mLayer.AddLayer(Order{ RendererPath::beginbg,index++,"bg" });		
		mLayer.AddLayer(Order{ RendererPath::beginpp,index++,"sky" });
		mLayer.AddLayer(Order{ RendererPath::beginpp,index++,"3D" });
		mLayer.AddLayer(Order{ RendererPath::luminace,index++,"luminance" });
		mLayer.AddLayer(Order{ RendererPath::bloom,index++,"bloom" });
		mLayer.AddLayer(Order{ RendererPath::begin,index++,"post" });
		mLayer.AddLayer(Order{ RendererPath::begin,index++,"2D" });
	}

	{
		AddManager(new StageManager(GetName()));
	}


}
