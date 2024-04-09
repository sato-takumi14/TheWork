#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* Audioクラス：おーディをを再生するクラス
********************************************************************************************/

#include"GameObject.h"

#include<string>
#include<thread>
#include<vector>

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

class Audio : public Component
{
private:
	static IXAudio2* mXaudio;
	static IXAudio2MasteringVoice* mMasteringVoice;

	IXAudio2SourceVoice* mSourceVoice{};
	BYTE* mSoundData{};

	int						mLength{};
	int						mPlayLength{};

	IMFSourceReader* mMFSourceReader{};
	IMFMediaType* mMFMediaType{};
	WAVEFORMATEX* mWaveFormat{};
	vector<BYTE> mMediaData;

	string WStringToString(wstring oWString);

public:

	Audio();
	~Audio();

	static void InitMaster();
	static void UninitMaster();

	using Component::Component;

	void Update();
	void Uninit();

	void Load(const char* FileName);
	void Play(bool Loop = false);

	void LoadMP3(wstring file);
	void PlayMP3(bool loop = false);

	void Stop();

};

