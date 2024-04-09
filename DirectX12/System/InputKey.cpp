#include "Main.h"
#include "InputKey.h"

#include"Interface\Scene.h"

std::unordered_map<string, std::list<InputKey::InputKeyboardInfo>> InputKey::mKeyboard{};
std::unordered_map<string, std::list<InputKey::InputGamepadInfo>> InputKey::mGamepad{};
Scene* InputKey::mScene{};

void InputKey::MasterInit()
{
}

void InputKey::MasterUninit()
{
	
	mKeyboard.clear();
	mGamepad.clear();

}

void InputKey::Uninit()
{

	for (auto& key : mKeyboard) {
		key.second.remove_if([&](InputKeyboardInfo input) { return mScene->GetName() == input.scene; });
	}

	for (auto& key : mGamepad) {
		key.second.remove_if([&](InputGamepadInfo input) { return mScene->GetName() == input.scene; });
	}

}

void InputKey::AddAction(
	crstring name, 
	std::function<cbool(const BYTE&)> func,
	const BYTE& key)
{
	mKeyboard[name].push_back(InputKeyboardInfo(func, key, mScene->GetName()));
}

void InputKey::AddGamepadAction(
	crstring name, 
	std::function<cbool(const DWORD&)> func,
	const DWORD& key)
{
	mGamepad[name].push_back(InputGamepadInfo(func, key, mScene->GetName()));
}

cbool InputKey::Action(crstring name)
{
	
	for (auto& data : mKeyboard[name]) { if (data.input(data.key))return true; }
	for (auto& data : mGamepad[name]) { if (data.input(data.key))return true; }

	return false;

}
