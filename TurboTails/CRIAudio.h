#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/17
*
********************************************************************************************
* CRIAudio�N���X�FCRI�~�h���E�F�A���g�p���Ă��[�f�B�����Đ�����N���X
********************************************************************************************/

#include"Component.h"

class CRIAudio : public Component
{
private:

	static void ErrorCallBack(
		const CriChar8* errid,
		CriUint32 p1,
		CriUint32 p2,
		CriUint32* parray
	);

	CriUint32 GetUserMemGetAllocatedMemorySize()const { return mUserMemAllocatedSize; }

	static void* Alloc(void* obj, CriUint32 size);
	static void Free(void* obj, void* buf);

private:

	/*�ʏ�̍Đ��Ɏg�p����ϐ�*/
	CriAtomExPlayerHn mPlayer;
	CriAtomExVoicePoolHn mPool;
	CriAtomExAcbHn mAcbHn;

	/*�X�g���[�~���O�Đ��Ɏg�p����ϐ�*/
	CriAtomExStandardVoicePoolConfig mVoicePoolConfig;
	CriAtomDbasId mDbasID;

	bool mLoop{};
	bool mStreaming{};
	bool mPlay{};

	CriAtomExCueId mID = -1;

	static CriUint32 mUserMemAllocatedSize;

public:

	static void InitMaster();
	static void UninitMaster();

	void Init()override;
	void Update()override;
	void Uninit()override;

	void Load(crstring file, crbool loop = false);
	void LoadStreaming(crstring file, crbool loop = true);

	void Load(crstring acf, crstring acb, crbool loop = false);
	void LoadStreaming(crstring acf, crstring acb, crstring awb, crbool loop = true);

	void Play(const CriAtomExCueId& id);
	void BeginPlay(const CriAtomExCueId& id);
	void Stop();
	void SetVolume(crfloat volume);
	void SetVolume(crfloat volume, const CriAtomExCueId& id);

	cbool GetPlay()const { return mPlay; }

};