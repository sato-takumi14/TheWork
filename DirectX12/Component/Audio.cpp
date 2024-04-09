#include "Main.h"
#include "Audio.h"

CriUint32 Audio::mUserMemAllocatedSize{};

void Audio::ErrorCallBack(const CriChar8* errid, CriUint32 p1, CriUint32 p2, CriUint32* parray)
{
}

void* Audio::Alloc(void* obj, CriUint32 size)
{

	void* mem = malloc(size + sizeof(CriUint32));
	void* ptr = CRI_NULL;

	if (mem != CRI_NULL) {
		mUserMemAllocatedSize += size;
		ptr = (void*)((CriUintPtr)mem + sizeof(CriUint32));
		*(CriUint32*)mem = size;
	}

	return ptr;

}

void Audio::Free(void* obj, void* buf)
{

	UINT32 size;
	void* mem;

	if (buf != NULL) {
		mem = (void*)((CriUintPtr)buf - sizeof(CriUint32));
		size = *(CriUint32*)mem;
		mUserMemAllocatedSize -= size;
		free(mem);
	}

}

void Audio::InitMaster()
{

	mUserMemAllocatedSize = 0;

	/*エラーコールバック関数の登録*/
	criErr_SetCallback(Audio::ErrorCallBack);

	/*メモリアロケーターの登録*/
	criAtomEx_SetUserAllocator(Audio::Alloc, Audio::Free, NULL);

	/*ライブラリの初期化*/
	criAtomEx_Initialize_WASAPI(NULL, NULL, 0);

}

void Audio::UninitMaster()
{
	/*ライブラリの終了*/
	criAtomEx_Finalize_WASAPI();
}

void Audio::Init()
{
}

void Audio::Update()
{

	if (mLoop) {
		if (criAtomExPlayer_GetStatus(mPlayer) == CRIATOMEXPLAYER_STATUS_PLAYEND) {
			/*キューIDの指定*/
			criAtomExPlayer_SetCueId(mPlayer, mAcbHn, mID);

			/*再生の開始*/
			criAtomExPlayer_Start(mPlayer);
		}
	}

}

void Audio::Uninit()
{

	Stop();

	/*Atomハンドルの破棄*/
	criAtomExPlayer_Destroy(mPlayer);

	/*ボイスプールの破棄*/
	criAtomExVoicePool_Free(mPool);

	/*ACBハンドルの破棄*/
	criAtomExAcb_Release(mAcbHn);

	/*ACFの登録解除*/
	criAtomEx_UnregisterAcf();

	if (mStreaming) {

		/*D-BASの破棄*/
		criAtomDbas_Destroy(mDbasID);

	}

}

void Audio::Load(crstring file, crbool loop)
{

	string acf = "audio\\" + file + ".acf";
	string acb = "audio\\" + file + ".acb";

	/*ACFファイルの読み込みと登録*/
	criAtomEx_RegisterAcfFile(NULL, acf.c_str(), NULL, 0);

	/*ACBファイルを読み込み、ACBハンドルを作成*/
	mAcbHn = criAtomExAcb_LoadAcbFile(NULL, acb.c_str(), NULL, NULL, NULL, 0);

	/*ボイスプールの作成*/
	mPool = criAtomExVoicePool_AllocateStandardVoicePool(NULL, NULL, 0);

	/*プレーヤの作成*/
	mPlayer = criAtomExPlayer_Create(NULL, NULL, 0);

	mLoop = loop;
	mStreaming = false;

}

void Audio::LoadStreaming(crstring file, crbool loop)
{

	string acf = "audio\\" + file + ".acf";
	string acb = "audio\\" + file + ".acb";
	string awb = "audio\\" + file + ".awb";

	/*D-BASの作成*/
	mDbasID = criAtomDbas_Create(NULL, NULL, 0);

	/*ACFファイルの読み込みと登録*/
	criAtomEx_RegisterAcfFile(NULL, acf.c_str(), NULL, 0);

	/*ACBファイルを読み込み、ACBハンドルを作成*/
	mAcbHn = criAtomExAcb_LoadAcbFile(NULL, acb.c_str(), NULL, awb.c_str(), NULL, 0);

	/*ボイスプールの作成*/
	criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&mVoicePoolConfig);
	mVoicePoolConfig.player_config.streaming_flag = CRI_TRUE;
	mPool = criAtomExVoicePool_AllocateStandardVoicePool(&mVoicePoolConfig, NULL, 0);

	/*プレイヤーの作成*/
	mPlayer = criAtomExPlayer_Create(NULL, NULL, 0);

	mStreaming = true;
	mLoop = loop;

}

void Audio::Load(crstring acf, crstring acb, crbool loop)
{

	string file = "audio\\" + acf + ".acf";

	/*ACFファイルの読み込みと登録*/
	criAtomEx_RegisterAcfFile(NULL, file.c_str(), NULL, 0);

	file = "audio\\" + acb + ".acb";

	/*ACBファイルを読み込み、ACBハンドルを作成*/
	mAcbHn = criAtomExAcb_LoadAcbFile(NULL, file.c_str(), NULL, NULL, NULL, 0);

	/*ボイスプールの作成*/
	mPool = criAtomExVoicePool_AllocateStandardVoicePool(NULL, NULL, 0);

	/*プレーヤの作成*/
	mPlayer = criAtomExPlayer_Create(NULL, NULL, 0);

	mLoop = loop;
	mStreaming = false;

}

void Audio::LoadStreaming(crstring acf, crstring acb, crstring awb, crbool loop)
{

	string file = "audio\\" + acf + ".acf";

	/*D-BASの作成*/
	mDbasID = criAtomDbas_Create(NULL, NULL, 0);

	/*ACFファイルの読み込みと登録*/
	criAtomEx_RegisterAcfFile(NULL, file.c_str(), NULL, 0);

	file = "audio\\" + acb + ".acb";
	string file2 = "audio\\" + awb + ".awb";

	/*ACBファイルを読み込み、ACBハンドルを作成*/
	mAcbHn = criAtomExAcb_LoadAcbFile(NULL, file.c_str(), NULL, file2.c_str(), NULL, 0);

	/*ボイスプールの作成*/
	criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&mVoicePoolConfig);
	mVoicePoolConfig.player_config.streaming_flag = CRI_TRUE;
	mPool = criAtomExVoicePool_AllocateStandardVoicePool(&mVoicePoolConfig, NULL, 0);

	/*プレイヤーの作成*/
	mPlayer = criAtomExPlayer_Create(NULL, NULL, 0);

	mStreaming = true;

}

void Audio::Play(const CriAtomExCueId& id)
{

	mID = id;

	/*キューIDの指定*/
	criAtomExPlayer_SetCueId(mPlayer, mAcbHn, id);

	/*再生の開始*/
	criAtomExPlayer_Start(mPlayer);

	mPlay = true;

}

void Audio::BeginPlay(const CriAtomExCueId& id)
{

	if (!mPlay) {

		mID = id;

		/*キューIDの指定*/
		criAtomExPlayer_SetCueId(mPlayer, mAcbHn, id);

		/*再生の開始*/
		criAtomExPlayer_Start(mPlayer);

		mPlay = true;

	}

}

void Audio::Stop()
{
	criAtomExPlayer_Stop(mPlayer);
	mPlay = false;
}

void Audio::SetVolume(crfloat volume)
{

	/*ボリュームの設定*/
	criAtomExPlayer_SetVolume(mPlayer, volume);

	/* プレーヤーが使用している全てのボイスにパラメーターを反映 */
	criAtomExPlayer_UpdateAll(mPlayer);

}

void Audio::SetVolume(crfloat volume, const CriAtomExCueId& id)
{

	/*ボリュームの設定*/
	criAtomExPlayer_SetVolume(mPlayer, volume);

	/* プレーヤーが使用している全てのボイスにパラメーターを反映 */
	criAtomExPlayer_Update(mPlayer, id);

}
