#pragma once
/******************************************************************************************************
*
*	[Time.h]
*																		制作者：佐藤匠
*																		更新日：7/28
*
*******************************************************************************************************
* Timeクラス：基本的にUnityのタイムと同じよな機能を持たせるように作成
*******************************************************************************************************/

#include"Component.h"
#include<unordered_map>
#include<chrono>

using namespace std::chrono;

class Time : public Component
{
private:

	static system_clock::time_point mStartTime;		/*スタート時間*/
	static system_clock::time_point mNowFreamTime;	/*現在時間*/
	static system_clock::time_point mOldFreamTime;	/*前のフレームの時間*/
	static float mDeltaTime;						/*フレーム間の秒数*/
	static int mFixedUpdateFreamCount;				/*GameObjectのFixdUpdateを呼ぶためのフレーム数*/
	static int mCount;								/*現在のフレーム数*/
	static bool mFixedUpdate;						/*FixdUpdateができるか*/

	double mEndTime{};								/*現在時間から何秒後にタイマーをセットするか*/

public:

	Time() {}
	Time(const double& end) { mEndTime = static_cast<double>(duration_cast<seconds>(system_clock::now().time_since_epoch()).count()) + end; }

	/*Audioクラスと同じ*/
	static void MasterInit();
	static void MasterUpdate();

	/*現在時間*/
	static const int GetNowTimeSeconds();					//秒
	static const float GetNowTimeMilliseconds();			//ミリ秒

	/*現在フレーム開始時間*/
	static const int GetNowFreamTimeSeconds();				//秒
	static const float GetNowFreamTimeMilliseconds();		//ミリ秒

	/*前回フレームから現在フレームの秒数*/
	static const float GetDeltaTime();						//デルタタイム

	static const bool GetFixedUpdate();						//FixdUpdateができるか
	static void SetFixedUpdateCount(const int count);		//FixdUpdateをするためのフレームカウントセット

	/*一応*/
	void Init()override;
	void Update()override;

	void Set(crdouble end);									//引数に何秒後にタイマーゲットするか
	const bool GetTimeSecond();								//タイマー取得：秒
	const bool GetTimeMillSecond();							//タイマー取得：ミリ秒

};