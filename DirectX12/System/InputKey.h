#pragma once

/***********************************************************************************************************
*
*	InputKey
*
*																				制作者：佐藤匠
*																				制作日：02/29
*
************************************************************************************************************
* 入力したときの名前で設定する
************************************************************************************************************/


class InputKey
{
private:

	/*キーボード・マウス*/
	typedef struct InputKeyboardInfo {

	public:

		std::function<cbool(const BYTE&)> input{};
		BYTE key;
		string scene;

	public:

		InputKeyboardInfo(
			std::function<cbool(const BYTE&)>& input,
			const BYTE& key,
			crstring scene) :
			input(input), key(key), scene(scene) {}

	}InputKeyboardInfo;

	/*ゲームパッド*/
	typedef struct InputGamepadInfo {

	public:

		std::function<cbool(const DWORD&)> input;
		DWORD key;
		string scene;

	public:

		InputGamepadInfo(
			std::function<cbool(const DWORD&)>& input,
			const DWORD& key,
			crstring scene) :
			input(input), key(key), scene(scene) {}

	}InputGamepadInfo;

private:

	static std::unordered_map<string, std::list<InputKeyboardInfo>> mKeyboard;	/*キーボード・マウス*/
	static std::unordered_map<string, std::list<InputGamepadInfo>> mGamepad;	/*ゲームパッド*/
	static class Scene* mScene;													/*現在のシーン*/

public:

	static void MasterInit();
	static void MasterUninit();
	static void Uninit();

	inline static void SetScene(Scene* scene) { mScene = scene; }
	
	/*アクションの追加*/
	static void AddAction(
		crstring name, 
		std::function<cbool(const BYTE&)> func,
		const BYTE& key);
	/*ゲームパッドアクションの追加*/
	static void AddGamepadAction(
		crstring name, 
		std::function<cbool(const DWORD&)> func,
		const DWORD& key);

	/*名前を付けたアクションがされているか*/
	static cbool Action(crstring name);

};