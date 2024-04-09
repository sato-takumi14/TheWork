
#include"main.h"

#include "SceneChangeObject.h"

#include"Scene.h"
#include"SceneManager.h"
#include"Input.h"
#include"CreateInstance.h"
#include"ActionKey.h"

void SceneChangeObject::SetNextScene() 
{
	SceneManager::SetNextSceneLoad(CreateInstance::CreateScene(mScene));
}

void SceneChangeObject::SetNextLoadingScene() 
{
	SceneManager::SetLoadingNextScene(CreateInstance::CreateScene(mScene));
}

void SceneChangeObject::Next()
{
	
}

void SceneChangeObject::Update()
{

	if (mUpdate) {

		if (!mKey) {

			if (Input::GetKeyTrigger(mBottonInt)) {

				mNextSceneFunc();

			}

		}
		else {

			if (Input::GetKeyPress(mBottonKey)) {

				mNextSceneFunc();

			}

		}

	}

	if (ActionKey::Action(mActionKey)) {

		if (mConditionsFunc()) {

			mNextSceneFunc();

		}

	}

}
