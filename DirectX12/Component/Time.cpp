
#include "Main.h"
#include "Time.h"

system_clock::time_point Time::mStartTime{};
system_clock::time_point Time::mNowTime{};
system_clock::time_point Time::mOldTime{};
double Time::mDeltaTime{};
int Time::mFiexdUpdateFreamCount{};
int Time::mCount{};
bool Time::mFixedUpdate{};

void Time::MasterInit()
{
	mStartTime = system_clock::now();
}

void Time::MasterUpdate()
{
	/*前回の時間を保存*/
	mOldTime = mNowTime;
	/*現在の時間を取得*/
	mNowTime = system_clock::now();
	/*前回の時間と現在の時間の間の時間をセット*/
	mDeltaTime = static_cast<double>(duration_cast<milliseconds>(mNowTime - mOldTime).count()) * 0.01;

	/*FixedUpdateをするかどうか*/
	if (mFiexdUpdateFreamCount == ++mCount) {
		mCount = true;
		mFixedUpdate = true;
	}
	else mFixedUpdate = false;
}

void Time::MasterUninit()
{
	
}

cint Time::GetNowTimeSeconds()
{
	return static_cast<int>(duration_cast<seconds>(system_clock::now() - mStartTime).count());
}

cfloat Time::GetNowTimeMilliseconds()
{
	return static_cast<float>(duration_cast<milliseconds>(system_clock::now() - mStartTime).count());
}

cint Time::GetNowFreamTimeSeconds()
{
	return static_cast<int>(duration_cast<seconds>(mNowTime - mStartTime).count());
}

cfloat Time::GetNowFreamTimeMilliseconds()
{
	return static_cast<float>(duration_cast<milliseconds>(mNowTime - mStartTime).count());
}

void Time::Set(crdouble end)
{
	mEndTime = static_cast<double>(duration_cast<seconds>(system_clock::now().time_since_epoch()).count()) + end;
}

cbool Time::GetTimeSecond()
{
	if (mEndTime <= static_cast<double>(duration_cast<seconds>(system_clock::now().time_since_epoch()).count())) {
		mEndTime = 0.0;
		return true;
	}
	return false;
}

cbool Time::GetTimeMillisecond()
{
	if (mEndTime <= static_cast<double>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count())) {
		mEndTime = 0.0;
		return true;
	}
	return false; return cbool();
}
