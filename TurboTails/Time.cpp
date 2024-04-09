
#include"Main.h"

#include "Time.h"

system_clock::time_point Time::mStartTime{};
system_clock::time_point Time::mNowFreamTime{};
system_clock::time_point Time::mOldFreamTime{};
float Time::mDeltaTime{};
int Time::mFixedUpdateFreamCount{};
int Time::mCount{};
bool Time::mFixedUpdate{};

void Time::MasterInit()
{
	mStartTime = system_clock::now();
}

void Time::MasterUpdate()
{
	//�O��̃t���[�����̎��Ԃ�ۑ�
	mOldFreamTime = mNowFreamTime;
	//���݂̎��Ԃ��擾
	mNowFreamTime = system_clock::now();
	//�O��t���[���ƌ��݃t���[���Ԃ̎��Ԃ��Z�b�g
	mDeltaTime = static_cast<float>(duration_cast<milliseconds>(mNowFreamTime - mOldFreamTime).count()) * 0.01f;

	//FixdUpdate�����邩�ǂ���
	mCount++;
	if (mFixedUpdateFreamCount == mCount) {
		mCount = 0;
		mFixedUpdate = true;
	}
	else mFixedUpdate = false;
}

void Time::Init()
{
}

void Time::Update()
{
}

void Time::Set(crdouble end)
{
	mEndTime = static_cast<double>(duration_cast<seconds>(system_clock::now().time_since_epoch()).count()) + end;
}

const bool Time::GetTimeSecond()
{
	if (mEndTime <= static_cast<double>(duration_cast<seconds>(system_clock::now().time_since_epoch()).count())) {
		mEndTime = 0.0f;
		return true;
	}

	return false;
}

const bool Time::GetTimeMillSecond()
{
	if (mEndTime <= static_cast<double>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count())) {
		mEndTime = 0.0f;
		return true;
	}

	return false;
}

const int Time::GetNowTimeSeconds()
{
	return static_cast<int>(duration_cast<seconds>(system_clock::now() - mStartTime).count());
}

const float Time::GetNowTimeMilliseconds()
{
	return static_cast<float>(duration_cast<milliseconds>(system_clock::now() - mStartTime).count());
}

const int Time::GetNowFreamTimeSeconds()
{
	return static_cast<int>(duration_cast<seconds>(mNowFreamTime - mStartTime).count());
}

const float Time::GetNowFreamTimeMilliseconds()
{
	return static_cast<float>(duration_cast<milliseconds>(mNowFreamTime - mStartTime).count());
}

const float Time::GetDeltaTime()
{
	return (mDeltaTime);
}

const bool Time::GetFixedUpdate()
{
	return mFixedUpdate;
}

void Time::SetFixedUpdateCount(const int count)
{
	mFixedUpdateFreamCount = count;
}
