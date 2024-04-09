#pragma once

/***********************************************************************************************************
*
*	Time
*
*																				制作者：佐藤匠
*																				制作日：03/01
*
************************************************************************************************************
* 時間を管理するクラス　UnityのTimeと同じ機能を持っている
************************************************************************************************************/

#include<chrono>

#include"Interface\Component.h"

using namespace std::chrono;

class Time : public Component
{
private:

	static system_clock::time_point mStartTime;			/*クラスを初期化をした時間*/
	static system_clock::time_point mNowTime;			/*現在時間*/
	static system_clock::time_point mOldTime;			/*前の更新した時間*/
	static double mDeltaTime;							/*フレーム間の時間*/
	static int mFiexdUpdateFreamCount;					/*GameObjectのFixedUpdateを呼ぶためのフレーム数*/
	static int mCount;									/*現在のフレーム数*/
	static bool mFixedUpdate;							/*FixedUpdateをするか*/

	double mEndTime{};									/*現在時間から何秒後にタイマーをセットするか*/

public:

	/*コンストラクタ*/
	inline Time() {}
	inline Time(crdouble end) { Set(end); }

	/*通常処理*/
	static void MasterInit();
	static void MasterUpdate();
	static void MasterUninit();

	/*現在時間*/
	static cint GetNowTimeSeconds();
	static cfloat GetNowTimeMilliseconds();

	/*現在フレーム開始時間*/
	static cint GetNowFreamTimeSeconds();
	static cfloat GetNowFreamTimeMilliseconds();

	/*前回フレームから現在のフレームの秒数*/
	inline static cfloat GetDeltaTime() { return static_cast<float>(mDeltaTime); }

	/*FixedUpdateができるか*/
	inline static cbool GetFixedUpdate() { return mFixedUpdate; }
	/*FixedUpdateをするためのフレームカウント*/
	inline static void SetFixedUpdateCount(crint count) { mCount = count; }

	/*タイマーセット*/
	void Set(crdouble end);
	cbool GetTimeSecond();
	cbool GetTimeMillisecond();

};