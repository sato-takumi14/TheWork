#pragma once
/******************************************************************************************************
*
*	[Time.h]
*																		����ҁF������
*																		�X�V���F7/28
*
*******************************************************************************************************
* Time�N���X�F��{�I��Unity�̃^�C���Ɠ�����ȋ@�\����������悤�ɍ쐬
*******************************************************************************************************/

#include"Component.h"
#include<unordered_map>
#include<chrono>

using namespace std::chrono;

class Time : public Component
{
private:

	static system_clock::time_point mStartTime;		/*�X�^�[�g����*/
	static system_clock::time_point mNowFreamTime;	/*���ݎ���*/
	static system_clock::time_point mOldFreamTime;	/*�O�̃t���[���̎���*/
	static float mDeltaTime;						/*�t���[���Ԃ̕b��*/
	static int mFixedUpdateFreamCount;				/*GameObject��FixdUpdate���ĂԂ��߂̃t���[����*/
	static int mCount;								/*���݂̃t���[����*/
	static bool mFixedUpdate;						/*FixdUpdate���ł��邩*/

	double mEndTime{};								/*���ݎ��Ԃ��牽�b��Ƀ^�C�}�[���Z�b�g���邩*/

public:

	Time() {}
	Time(const double& end) { mEndTime = static_cast<double>(duration_cast<seconds>(system_clock::now().time_since_epoch()).count()) + end; }

	/*Audio�N���X�Ɠ���*/
	static void MasterInit();
	static void MasterUpdate();

	/*���ݎ���*/
	static const int GetNowTimeSeconds();					//�b
	static const float GetNowTimeMilliseconds();			//�~���b

	/*���݃t���[���J�n����*/
	static const int GetNowFreamTimeSeconds();				//�b
	static const float GetNowFreamTimeMilliseconds();		//�~���b

	/*�O��t���[�����猻�݃t���[���̕b��*/
	static const float GetDeltaTime();						//�f���^�^�C��

	static const bool GetFixedUpdate();						//FixdUpdate���ł��邩
	static void SetFixedUpdateCount(const int count);		//FixdUpdate�����邽�߂̃t���[���J�E���g�Z�b�g

	/*�ꉞ*/
	void Init()override;
	void Update()override;

	void Set(crdouble end);									//�����ɉ��b��Ƀ^�C�}�[�Q�b�g���邩
	const bool GetTimeSecond();								//�^�C�}�[�擾�F�b
	const bool GetTimeMillSecond();							//�^�C�}�[�擾�F�~���b

};