
#include"Main.h"
#include "ActionKey.h"

#include"Input.h"
#include"Scene.h"

unordered_map<string, list<ActionKey::ActionInfo>> ActionKey::mAction{};
unordered_map<string, list<ActionKey::ActionInfoGamePad>> ActionKey::mGamePadAction{};
Scene* ActionKey::mScene{};

void ActionKey::Uninit()
{
    mGamePadAction.clear();
    mAction.clear();
}

void ActionKey::Uninit(crstring scene)
{

    erase_if(mAction, [&](auto& data)->bool {
        for (auto& action : data.second) {
            if (scene == action.scene) {
                if (static_cast<int>(data.second.size()) == 0) {
                    mAction.erase(data.first);
                }
                return true;
            }
        }
        return false;
        });
    erase_if(mGamePadAction, [&](auto& data)->bool {
        for (auto& action : data.second) {
            if (scene == action.scene) {
                if (static_cast<int>(data.second.size()) == 0) {
                    mGamePadAction.erase(data.first);
                }                
                return true;
            }
        }
        return false;
        });

}

void ActionKey::SetScene(Scene* scene)
{
    mScene = scene;
    Uninit();
}

void ActionKey::AddAction(crstring name, function<cbool(const BYTE&)> func, const BYTE& action)
{
    mAction[name].push_back(ActionInfo(func, action, mScene->GetName()));
}

void ActionKey::AddGamePadAction(crstring name, function<cbool(const DWORD&)> func, const DWORD& key)
{
    mGamePadAction[name].push_back(ActionInfoGamePad(func, key,mScene->GetName()));
}

cbool ActionKey::Action(crstring name)
{
    for (auto& data : mAction[name]) { if (data.func(data.key)) return true; }
    for (auto& data : mGamePadAction[name]) { if (data.func(data.key)) return true; }
    return false;
}
