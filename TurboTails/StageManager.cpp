
#include"Main.h"

#include "StageManager.h"

#include"TargetManager.h"
#include"EnemyManager.h"
#include"UIManager.h"
#include"ItemManager.h"
#include"GimmickManager.h"

#include"Player.h"
#include"MeshField.h"
#include"Post.h"
#include"Skydome.h"
#include"DebugCamera.h"
#include"Load.h"
#include"Luminance.h"
#include"Bloom.h"

#include"Scene.h"
#include"File.h"
#include"CRIAudio.h"

#include"audio\GameBGM_001.h"

void StageManager::Init()
{

	mStageObjectData = File::JsonLoadTag<unordered_map<string, list<Transform>>>("asset\\stage\\stage.json", mStatgeName);	

	{
		mScene->AddGameObject(new Camera(true));
		mScene->AddGameObject(new MeshField());

#if _DEBUG

		mScene->AddGameObject(new DebugCamera());

#endif // _DEBUG

		mScene->AddGameObject(new Player());
		mScene->AddGameObject(new Skydome());
		mScene->AddGameObject(new Post());
		mScene->AddGameObject(new Load());

		if (mScene->GetName() != "title") {
			mScene->AddGameObject(new Luminance());
			mScene->AddGameObject(new Bloom());

			if (mScene->GetName() != "select") {

				mAudio = new CRIAudio();
				mAudio->LoadStreaming(
					"Tutorial",
					"GameBGM_001",
					"GameBGM_001",
					true);
				mAudio->SetVolume(0.1f);

			}
			
		}

	}

	mScene->AddManager(new TargetManager());
	mScene->AddManager(new EnemyManager());
	mScene->AddManager(new UIManager());
	mScene->AddManager(new ItemManager(this));
	mScene->AddManager(new GimmickManager(this));

}

void StageManager::Start()
{			
	if (mAudio)mAudio->Play(CRI_GAMEBGM_001_MAOU_BGM_NEOROCK81);
}

void StageManager::Update()
{
	Manager::Update();
	if (mAudio)mAudio->Update();
}

void StageManager::Uninit()
{

	for (auto& data : mStageObjectData) {
		data.second.clear();
	}

	mStageObjectData.clear();

	if (mAudio) {
		mAudio->Stop();
		mAudio->Uninit();
		delete mAudio;
		mAudio = nullptr;
	}

}

const list<Transform> StageManager::GetTransforminfos(crstring name) const
{
	for (auto& data : mStageObjectData) { if (data.first == name) return data.second; }
	return list<Transform>();
}
