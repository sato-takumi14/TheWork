
#include"Main.h"
#include "AudioManager.h"

#include"CRIAudio.h"
#include"GameObject.h"

AudioManager::AudioManager(GameObject* obj)
{
	mObject = obj;
}

AudioManager::~AudioManager()
{
	Uninit();
}

void AudioManager::Update()
{
}

void AudioManager::Uninit()
{
	mInfo.clear();
}

void AudioManager::Load(
	crstring name,
	crstring file, 
	const signed long& id, 
	const AudioState& state, 
	crbool loop)
{

	bool flag = true;
	for (auto& data : mInfo) { if (data.first == name)flag = false; }

	if (flag) {

		mInfo[name] = AudioInfo(new CRIAudio(), id);

		switch (state) {

		case AudioState::se:
			mInfo[name].audio->Load(file, loop);
			break;

		case AudioState::bgm:
			mInfo[name].audio->LoadStreaming(file, loop);
			break;

		}

		mObject->AddComponent(mInfo[name].audio);

	}

}

void AudioManager::Load(
	crstring name, 
	crstring acf, 
	crstring acb, 
	crstring awb, 
	const signed long& id, 
	const AudioState& state, 
	crbool loop)
{

	bool flag = true;
	for (auto& data : mInfo) { if (data.first == name)flag = false; }

	if (flag) {

		mInfo[name] = AudioInfo(new CRIAudio(), id);

		switch (state) {

		case AudioState::se:
			mInfo[name].audio->Load(acf, acb, loop);
			break;

		case AudioState::bgm:
			mInfo[name].audio->LoadStreaming(acf, acb, awb, loop);
			break;

		}

		mObject->AddComponent(mInfo[name].audio);

	}

}

void AudioManager::Play(crstring name)
{
	
	for (auto& data : mInfo) {

		if (data.first == name) {
			
			data.second.audio->Play(data.second.id);
			break;

		}

	}
}

void AudioManager::BeginPlay(crstring name)
{
	
	for (auto& data : mInfo) {

		if (data.first == name) {

			data.second.audio->BeginPlay(data.second.id);
			break;

		}

	}

}

void AudioManager::Stop(crstring name)
{

	for (auto& data : mInfo) {

		if (data.first == name) {

			data.second.audio->Stop();
			break;

		}

	}

}

void AudioManager::Volume(crstring name, crfloat volume)
{

	for (auto& data : mInfo) {

		if (data.first == name) {

			data.second.audio->SetVolume(volume);
			break;

		}

	}

}

void AudioManager::Volume(crstring name, crfloat volume, const signed long& id)
{

	for (auto& data : mInfo) {

		if (data.first == name) {

			data.second.audio->SetVolume(volume, id);
			break;

		}

	}

}

cbool AudioManager::GetPlay(crstring name)
{
	for (auto& data : mInfo) {
		if (data.first == name) {
			return data.second.audio->GetPlay();
		}
	}
	return false;
}
