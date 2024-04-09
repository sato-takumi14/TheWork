#pragma once

/***********************************************************************************************************
*
*	Audio
*
*																				制作者：佐藤匠
*																				制作日：03/06
*
************************************************************************************************************
* CRIミドルウェアを使用してサウンドを鳴らすクラス
************************************************************************************************************/

#include"Interface\Interface.h"
#include"Interface\Component.h"

/*CRI SDKヘッダー*/
#include <cri_le_xpt.h>

/*CRI ADX2ヘッダー*/
#include <cri_le_atom_ex.h>
#include <cri_le_atom_wasapi.h>

#pragma comment (lib,"cri_ware_pcx64_le_import.lib")

class Audio : public Interface, public Component
{
private:

	/*通常の再生に使用する変数*/
	CriAtomExPlayerHn mPlayer;
	CriAtomExVoicePoolHn mPool;
	CriAtomExAcbHn mAcbHn;

	/*ストリーミング再生に使用する変数*/
	CriAtomExStandardVoicePoolConfig mVoicePoolConfig;
	CriAtomDbasId mDbasID;

	bool mLoop{};
	bool mStreaming{};
	bool mPlay{};

	CriAtomExCueId mID = -1;

	static CriUint32 mUserMemAllocatedSize;

private:

	static void ErrorCallBack(
		const CriChar8* errid,
		CriUint32 p1,
		CriUint32 p2,
		CriUint32* parray
	);
	static void* Alloc(void* obj, CriUint32 size);
	static void Free(void* obj, void* buf);

public:

	/*ライブラリの初期化など一度だけ実行する*/
	static void InitMaster();
	static void UninitMaster();

	/*通常処理*/
	void Init()override;
	void Update()override;
	void Uninit()override;

	/*ロード*/
	void Load(crstring file, crbool loop = false);
	void LoadStreaming(crstring file, crbool loop = true);

	void Load(crstring acf, crstring acb, crbool loop = false);
	void LoadStreaming(crstring acf, crstring acb, crstring awb, crbool loop = true);

	/*プレイ*/
	void Play(const CriAtomExCueId& id);
	void BeginPlay(const CriAtomExCueId& id);
	/*ストップ*/
	void Stop();
	/*ボリュームを設定*/
	void SetVolume(crfloat volume);
	void SetVolume(crfloat volume, const CriAtomExCueId& id);

	/*現在再生しているか*/
	inline cbool GetPlay()const { return mPlay; }

};