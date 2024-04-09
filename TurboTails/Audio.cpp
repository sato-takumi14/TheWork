
#include"Main.h"
#include "Audio.h"

IXAudio2* Audio::mXaudio = NULL;
IXAudio2MasteringVoice* Audio::mMasteringVoice = NULL;

string Audio::WStringToString(wstring oWString)
{
	// wstring → SJIS
	int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
		, -1, (char*)NULL, 0, NULL, NULL);

	// バッファの取得
	CHAR* cpMultiByte = new CHAR[iBufferSize];

	// wstring → SJIS
	WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte
		, iBufferSize, NULL, NULL);

	// stringの生成
	std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

	// バッファの破棄
	delete[] cpMultiByte;

	// 変換結果を返す
	return(oRet);
}

Audio::Audio()
{
}

Audio::~Audio()
{
	
}

void Audio::InitMaster()
{
	// COM初期化
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);

	// XAudio生成
	XAudio2Create(&mXaudio, 0);

	// マスタリングボイス生成
	mXaudio->CreateMasteringVoice(&mMasteringVoice);

}

void Audio::UninitMaster()
{
	MFShutdown();

	mMasteringVoice->DestroyVoice();
	mXaudio->Release();
	CoUninitialize();
}

void Audio::Load(const char* FileName)
{

	// サウンドデータ読込
	WAVEFORMATEX wfx = { 0 };

	{
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;


		hmmio = mmioOpen((LPSTR)FileName, &mmioinfo, MMIO_READ);
		assert(hmmio);

		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
		{
			mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
		}
		else
		{
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}
		mmioAscend(hmmio, &mmckinfo, 0);

		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);



		buflen = datachunkinfo.cksize;
		mSoundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)mSoundData, buflen);


		mLength = readlen;
		mPlayLength = readlen / wfx.nBlockAlign;


		mmioClose(hmmio, 0);
	}


	// サウンドソース生成
	mXaudio->CreateSourceVoice(&mSourceVoice, &wfx);
	assert(mSourceVoice);
}

void Audio::Update()
{

}

void Audio::Uninit()
{

	mSourceVoice->Stop();
	mSourceVoice->DestroyVoice();

	CoTaskMemFree(mWaveFormat);
	if (mMFMediaType) mMFMediaType->Release();
	if (mMFSourceReader) mMFSourceReader->Release();

	delete[] mSoundData;
}

void Audio::Play(bool Loop)
{
	mSourceVoice->Stop();
	mSourceVoice->FlushSourceBuffers();


	// バッファ設定
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = mLength;
	bufinfo.pAudioData = mSoundData;
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = mPlayLength;

	// ループ設定
	if (Loop)
	{
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = mPlayLength;
		bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	mSourceVoice->SubmitSourceBuffer(&bufinfo, NULL);

	// 再生
	mSourceVoice->Start();

}

void Audio::LoadMP3(wstring file)
{
	MFCreateSourceReaderFromURL(file.c_str(), NULL, &mMFSourceReader);

	MFCreateMediaType(&mMFMediaType);
	mMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	mMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	mMFSourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, mMFMediaType);

	mMFMediaType->Release();
	mMFMediaType = nullptr;
	mMFSourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &mMFMediaType);

	MFCreateWaveFormatExFromMFMediaType(mMFMediaType, &mWaveFormat, nullptr);

	while (true) {

		IMFSample* sample{};
		DWORD dwStreamFlags{ 0 };
		mMFSourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwStreamFlags, nullptr, &sample);

		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM) {
			break;
		}

		IMFMediaBuffer* MediaBuffer{};
		sample->ConvertToContiguousBuffer(&MediaBuffer);

		BYTE* buffer{};
		DWORD length{};
		MediaBuffer->Lock(&buffer, nullptr, &length);

		mMediaData.resize(mMediaData.size() + length);
		memcpy(mMediaData.data() + mMediaData.size() - length, buffer, length);

		MediaBuffer->Unlock();

		MediaBuffer->Release();
		sample->Release();

	}

	mXaudio->CreateSourceVoice(&mSourceVoice, mWaveFormat);
}

void Audio::PlayMP3(bool loop)
{
	mSourceVoice->Stop();
	mSourceVoice->FlushSourceBuffers();

	XAUDIO2_BUFFER buffer{ 0 };

	buffer.pAudioData = mMediaData.data();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(mMediaData.size());

	if (loop) {
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	mSourceVoice->SubmitSourceBuffer(&buffer, NULL);

	mSourceVoice->Start(0);
}

void Audio::Stop()
{
	mSourceVoice->Stop();
	mSourceVoice->FlushSourceBuffers();
}
