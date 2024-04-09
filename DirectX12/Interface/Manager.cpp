#include "Main.h"
#include "Manager.h"

#include "Interface\GameObject.h"
#include "Interface\Scene.h"
#include "System\CreateInstance.h"

void Manager::CreateGameObject(crstring name, const std::list<Transform>& transforms) const
{
	for (auto& data : transforms) {
		auto obj = Create::Device::CreateObject(name);
		obj->SetTransform(data);
		mScene->AddGameobject(obj);
	}
}
