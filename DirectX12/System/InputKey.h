#pragma once

/***********************************************************************************************************
*
*	InputKey
*
*																				����ҁF������
*																				������F02/29
*
************************************************************************************************************
* ���͂����Ƃ��̖��O�Őݒ肷��
************************************************************************************************************/


class InputKey
{
private:

	/*�L�[�{�[�h�E�}�E�X*/
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

	/*�Q�[���p�b�h*/
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

	static std::unordered_map<string, std::list<InputKeyboardInfo>> mKeyboard;	/*�L�[�{�[�h�E�}�E�X*/
	static std::unordered_map<string, std::list<InputGamepadInfo>> mGamepad;	/*�Q�[���p�b�h*/
	static class Scene* mScene;													/*���݂̃V�[��*/

public:

	static void MasterInit();
	static void MasterUninit();
	static void Uninit();

	inline static void SetScene(Scene* scene) { mScene = scene; }
	
	/*�A�N�V�����̒ǉ�*/
	static void AddAction(
		crstring name, 
		std::function<cbool(const BYTE&)> func,
		const BYTE& key);
	/*�Q�[���p�b�h�A�N�V�����̒ǉ�*/
	static void AddGamepadAction(
		crstring name, 
		std::function<cbool(const DWORD&)> func,
		const DWORD& key);

	/*���O��t�����A�N�V����������Ă��邩*/
	static cbool Action(crstring name);

};