#include "Main.h"
#include "CreateInstance.h"

namespace Create
{

	std::unordered_map<string, std::function<GameObject::Interface* ()>> Device::mGameObjects{};
	std::unordered_map<string, std::function<Scene* ()>> Device::mScenes{};

	void Device::MasterInit()
	{
	}

	void Device::MasterUninit()
	{
		Device::GameObjectClear();
		Device::SceneClear();
	}

}
