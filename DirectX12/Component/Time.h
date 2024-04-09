#pragma once

/***********************************************************************************************************
*
*	Time
*
*																				����ҁF������
*																				������F03/01
*
************************************************************************************************************
* ���Ԃ��Ǘ�����N���X�@Unity��Time�Ɠ����@�\�������Ă���
************************************************************************************************************/

#include<chrono>

#include"Interface\Component.h"

using namespace std::chrono;

class Time : public Component
{
private:

	static system_clock::time_point mStartTime;			/*�N���X������������������*/
	static system_clock::time_point mNowTime;			/*���ݎ���*/
	static system_clock::time_point mOldTime;			/*�O�̍X�V��������*/
	static double mDeltaTime;							/*�t���[���Ԃ̎���*/
	static int mFiexdUpdateFreamCount;					/*GameObject��FixedUpdate���ĂԂ��߂̃t���[����*/
	static int mCount;									/*���݂̃t���[����*/
	static bool mFixedUpdate;							/*FixedUpdate�����邩*/

	double mEndTime{};									/*���ݎ��Ԃ��牽�b��Ƀ^�C�}�[���Z�b�g���邩*/

public:

	/*�R���X�g���N�^*/
	inline Time() {}
	inline Time(crdouble end) { Set(end); }

	/*�ʏ폈��*/
	static void MasterInit();
	static void MasterUpdate();
	static void MasterUninit();

	/*���ݎ���*/
	static cint GetNowTimeSeconds();
	static cfloat GetNowTimeMilliseconds();

	/*���݃t���[���J�n����*/
	static cint GetNowFreamTimeSeconds();
	static cfloat GetNowFreamTimeMilliseconds();

	/*�O��t���[�����猻�݂̃t���[���̕b��*/
	inline static cfloat GetDeltaTime() { return static_cast<float>(mDeltaTime); }

	/*FixedUpdate���ł��邩*/
	inline static cbool GetFixedUpdate() { return mFixedUpdate; }
	/*FixedUpdate�����邽�߂̃t���[���J�E���g*/
	inline static void SetFixedUpdateCount(crint count) { mCount = count; }

	/*�^�C�}�[�Z�b�g*/
	void Set(crdouble end);
	cbool GetTimeSecond();
	cbool GetTimeMillisecond();

};