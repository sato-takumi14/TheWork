
#include"Main.h"
#include "CRIAudio.h"

CriUint32 CRIAudio::mUserMemAllocatedSize = 0;
static CriUint32 usermem_allocated_size = 0;

void* Alloc(void* obj, CriUint32 size)
{

	void* mem = malloc(size + sizeof(CriUint32));
	void* ptr = CRI_NULL;

	if (mem != CRI_NULL) {
		usermem_allocated_size += size;
		ptr = (void*)((CriUintPtr)mem + sizeof(CriUint32));
		*(CriUint32*)mem = size;
	}

	return ptr;

}

void Free(void* obj, void* buf)
{

	UINT32 size;
	void* mem;

	if (buf != NULL) {
		mem = (void*)((CriUintPtr)buf - sizeof(CriUint32));
		size = *(CriUint32*)mem;
		usermem_allocated_size -= size;
		free(mem);
	}

}

void CRIAudio::ErrorCallBack(const CriChar8* errid, CriUint32 p1, CriUint32 p2, CriUint32* parray)
{

}

void* CRIAudio::Alloc(void* obj, CriUint32 size)
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

void CRIAudio::Free(void* obj, void* buf)
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

void CRIAudio::InitMaster()
{

	mUserMemAllocatedSize = 0;

	/*�G���[�R�[���o�b�N�֐��̓o�^*/
	criErr_SetCallback(CRIAudio::ErrorCallBack);

	/*�������A���P�[�^�[�̓o�^*/
	criAtomEx_SetUserAllocator(Alloc, Free, NULL);

	/*���C�u�����̏�����*/
	criAtomEx_Initialize_WASAPI(NULL, NULL, 0);

}

void CRIAudio::UninitMaster()
{
	/*���C�u�����̏I��*/
	criAtomEx_Finalize_WASAPI();
}

void CRIAudio::Init()
{
	
}

void CRIAudio::Update()
{
	if (mLoop) {
		if (criAtomExPlayer_GetStatus(mPlayer) == CRIATOMEXPLAYER_STATUS_PLAYEND) {
			/*�L���[ID�̎w��*/
			criAtomExPlayer_SetCueId(mPlayer, mAcbHn, mID);

			/*�Đ��̊J�n*/
			criAtomExPlayer_Start(mPlayer);
		}
	}
}

void CRIAudio::Uninit()
{

	Stop();

	/*Atom�n���h���̔j��*/
	criAtomExPlayer_Destroy(mPlayer);

	/*�{�C�X�v�[���̔j��*/
	criAtomExVoicePool_Free(mPool);

	/*ACB�n���h���̔j��*/
	criAtomExAcb_Release(mAcbHn);

	/*ACF�̓o�^����*/
	criAtomEx_UnregisterAcf();

	if (mStreaming) {

		/*D-BAS�̔j��*/
		criAtomDbas_Destroy(mDbasID);

	}

}

void CRIAudio::Load(crstring file, crbool loop)
{

	string acf = "audio\\" + file + ".acf";
	string acb = "audio\\" + file + ".acb";

	/*ACF�t�@�C���̓ǂݍ��݂Ɠo�^*/
	criAtomEx_RegisterAcfFile(NULL, acf.c_str(), NULL, 0);

	/*ACB�t�@�C����ǂݍ��݁AACB�n���h�����쐬*/
	mAcbHn = criAtomExAcb_LoadAcbFile(NULL, acb.c_str(), NULL, NULL, NULL, 0);

	/*�{�C�X�v�[���̍쐬*/
	mPool = criAtomExVoicePool_AllocateStandardVoicePool(NULL, NULL, 0);

	/*�v���[���̍쐬*/
	mPlayer = criAtomExPlayer_Create(NULL, NULL, 0);

	mLoop = loop;
	mStreaming = false;

}

void CRIAudio::LoadStreaming(crstring file, crbool loop)
{

	string acf = "audio\\" + file + ".acf";
	string acb = "audio\\" + file + ".acb";
	string awb = "audio\\" + file + ".awb";

	/*D-BAS�̍쐬*/
	mDbasID = criAtomDbas_Create(NULL, NULL, 0);

	/*ACF�t�@�C���̓ǂݍ��݂Ɠo�^*/
	criAtomEx_RegisterAcfFile(NULL, acf.c_str(), NULL, 0);

	/*ACB�t�@�C����ǂݍ��݁AACB�n���h�����쐬*/
	mAcbHn = criAtomExAcb_LoadAcbFile(NULL, acb.c_str(), NULL, awb.c_str(), NULL, 0);

	/*�{�C�X�v�[���̍쐬*/
	criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&mVoicePoolConfig);
	mVoicePoolConfig.player_config.streaming_flag = CRI_TRUE;
	mPool = criAtomExVoicePool_AllocateStandardVoicePool(&mVoicePoolConfig, NULL, 0);

	/*�v���C���[�̍쐬*/
	mPlayer = criAtomExPlayer_Create(NULL, NULL, 0);

	mStreaming = true;
	mLoop = loop;

}

void CRIAudio::Load(crstring acf, crstring acb, crbool loop)
{

	string file = "audio\\" + acf + ".acf";

	/*ACF�t�@�C���̓ǂݍ��݂Ɠo�^*/
	criAtomEx_RegisterAcfFile(NULL, file.c_str(), NULL, 0);

	file = "audio\\" + acb + ".acb";

	/*ACB�t�@�C����ǂݍ��݁AACB�n���h�����쐬*/
	mAcbHn = criAtomExAcb_LoadAcbFile(NULL, file.c_str(), NULL, NULL, NULL, 0);

	/*�{�C�X�v�[���̍쐬*/
	mPool = criAtomExVoicePool_AllocateStandardVoicePool(NULL, NULL, 0);

	/*�v���[���̍쐬*/
	mPlayer = criAtomExPlayer_Create(NULL, NULL, 0);

	mLoop = loop;
	mStreaming = false;

}

void CRIAudio::LoadStreaming(crstring acf, crstring acb, crstring awb, crbool loop)
{

	string file = "audio\\" + acf + ".acf";

	/*D-BAS�̍쐬*/
	mDbasID = criAtomDbas_Create(NULL, NULL, 0);

	/*ACF�t�@�C���̓ǂݍ��݂Ɠo�^*/
	criAtomEx_RegisterAcfFile(NULL, file.c_str(), NULL, 0);

	file = "audio\\" + acb + ".acb";
	string file2 = "audio\\" + awb + ".awb";

	/*ACB�t�@�C����ǂݍ��݁AACB�n���h�����쐬*/
	mAcbHn = criAtomExAcb_LoadAcbFile(NULL, file.c_str(), NULL, file2.c_str(), NULL, 0);

	/*�{�C�X�v�[���̍쐬*/
	criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&mVoicePoolConfig);
	mVoicePoolConfig.player_config.streaming_flag = CRI_TRUE;
	mPool = criAtomExVoicePool_AllocateStandardVoicePool(&mVoicePoolConfig, NULL, 0);

	/*�v���C���[�̍쐬*/
	mPlayer = criAtomExPlayer_Create(NULL, NULL, 0);

	mStreaming = true;

}

void CRIAudio::Play(const CriAtomExCueId& id)
{

	mID = id;

	/*�L���[ID�̎w��*/
	criAtomExPlayer_SetCueId(mPlayer, mAcbHn, id);

	/*�Đ��̊J�n*/
	criAtomExPlayer_Start(mPlayer);

	mPlay = true;

}

void CRIAudio::BeginPlay(const CriAtomExCueId& id)
{

	if (!mPlay) {

		mID = id;

		/*�L���[ID�̎w��*/
		criAtomExPlayer_SetCueId(mPlayer, mAcbHn, id);

		/*�Đ��̊J�n*/
		criAtomExPlayer_Start(mPlayer);

		mPlay = true;

	}

}

void CRIAudio::Stop()
{
	criAtomExPlayer_Stop(mPlayer);
	mPlay = false;
}

void CRIAudio::SetVolume(crfloat volume)
{

	/*�{�����[���̐ݒ�*/
	criAtomExPlayer_SetVolume(mPlayer, volume);

	/* �v���[���[���g�p���Ă���S�Ẵ{�C�X�Ƀp�����[�^�[�𔽉f */
	criAtomExPlayer_UpdateAll(mPlayer);

}

void CRIAudio::SetVolume(crfloat volume, const CriAtomExCueId& id)
{

	/*�{�����[���̐ݒ�*/
	criAtomExPlayer_SetVolume(mPlayer, volume);

	/* �v���[���[���g�p���Ă���S�Ẵ{�C�X�Ƀp�����[�^�[�𔽉f */
	criAtomExPlayer_Update(mPlayer, id);

}
