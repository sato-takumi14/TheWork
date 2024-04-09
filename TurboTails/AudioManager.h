#pragma once

#include"Manager.h"

enum class AudioState
{
	none,
	se,
	bgm,
};

class CRIAudio;
class GameObject;

class AudioManager : public Manager
{
private:

	typedef struct AudioInfo
	{
	public:

		CRIAudio* audio;
		signed long id;

	public:

		AudioInfo() {}
		AudioInfo(CRIAudio* audio, const signed long& id) : audio(audio), id(id) {}

	}AudioInfo;

private:

	GameObject* mObject{};
	unordered_map<string, AudioInfo> mInfo{};

public:

	AudioManager(GameObject* obj);
	~AudioManager();

	void Update()override;
	void Uninit()override;

	void Load(
		crstring name,
		crstring file, 
		const signed long& id, 
		const AudioState& state, 
		crbool loop = false);

	void Load(
		crstring name,
		crstring acf,
		crstring acb,
		crstring awb,
		const signed long& id,
		const AudioState& state,
		crbool loop
	);

	void Play(crstring name);
	void BeginPlay(crstring name);
	void Stop(crstring name);
	void Volume(crstring name, crfloat volume);
	void Volume(crstring name, crfloat volume, const signed long& id);

	cbool GetPlay(crstring name);

};