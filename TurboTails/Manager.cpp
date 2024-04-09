
#include"Main.h"

#include "Manager.h"
#include"File.h"
#include"CreateInstance.h"
#include"Scene.h"
#include"MultiGameObject.h"

void Manager::CreateMultiGameObject(crstring name, cruint state, const list<Transform> transforms)const
{

    if (static_cast<int>(transforms.size()) == 0)return;

    mScene->AddGameObject(
        new MultiGameObject(
            mScene,
            name,
            transforms,
            (MultiGameObject::MultiDrawGameObjectState)(state)
        )
    );

}

void Manager::CreateGameObject(crstring name, const list<Transform> transforms) const
{

    for (auto& data : transforms) {
        auto obj = CreateInstance::CreateObject(name);
        obj->SetTransform(data);
        mScene->AddGameObject(obj);        
        break;
    }

}

void Manager::CreateGameObjects(crstring name, const list<Transform> transforms) const
{

    for (auto& data : transforms) {
        auto obj = CreateInstance::CreateObject(name);
        obj->SetTransform(data);
        mScene->AddGameObject(obj);        
    }

}
