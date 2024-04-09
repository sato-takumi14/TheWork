#pragma once

/***********************************************************************************************************
*
*	Audio
*
*																				����ҁF������
*																				������F03/06
*
************************************************************************************************************
* CRI�~�h���E�F�A���g�p���ăT�E���h��炷�N���X
************************************************************************************************************/

#include"Interface\Interface.h"
#include"Interface\Component.h"

/*CRI SDK�w�b�_�[*/
#include <cri_le_xpt.h>

/*CRI ADX2�w�b�_�[*/
#include <cri_le_atom_ex.h>
#include <cri_le_atom_wasapi.h>

#pragma comment (lib,"cri_ware_pcx64_le_import.lib")

class Audio : public Interface, public Component
{
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

	/*���C�u�����̏������Ȃǈ�x�������s����*/
	static void InitMaster();
	static void UninitMaster();

	/*�ʏ폈��*/
	void Init()override;
	void Update()override;
	void Uninit()override;

	/*���[�h*/
	void Load(crstring file, crbool loop = false);
	void LoadStreaming(crstring file, crbool loop = true);

	void Load(crstring acf, crstring acb, crbool loop = false);
	void LoadStreaming(crstring acf, crstring acb, crstring awb, crbool loop = true);

	/*�v���C*/
	void Play(const CriAtomExCueId& id);
	void BeginPlay(const CriAtomExCueId& id);
	/*�X�g�b�v*/
	void Stop();
	/*�{�����[����ݒ�*/
	void SetVolume(crfloat volume);
	void SetVolume(crfloat volume, const CriAtomExCueId& id);

	/*���ݍĐ����Ă��邩*/
	inline cbool GetPlay()const { return mPlay; }

};