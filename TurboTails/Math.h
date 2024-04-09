#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* Math：線形補間等計算をするためのクラス
********************************************************************************************/

#include"Component.h"

namespace Math
{
	/*線形補間*/
	namespace Lerp
	{
		/*Vector3*/
		class Vector3 : public Component
		{
		private:

			ver3*	mValue{};
			ver3	mStart{};
			ver3	mFinal{};
			float	mNow{};
			float	mAddTime{};
			float	mEnd{};

		public:

			inline Vector3(
				ver3* value,
				const ver3& start,
				const ver3& final,
				crfloat add,
				crfloat end) {
				this->mValue = value;
				this->mStart = start;
				this->mFinal = final;
				this->mNow = 0.0f;
				this->mAddTime = add;
				this->mEnd = end;
			}

			inline void Update()override {

				mNow += mAddTime;
				if (mNow >= mEnd) SetDestroy();
				else {
					float lerp = mNow / mEnd;
					(*mValue) = (this->mStart + ((this->mFinal - this->mStart) * lerp));
				}				

			}

		};

		/*float*/
		class Float : public Component
		{
		private:

			float*	mValue{};
			float	mStart{};
			float	mFinal{};
			float	mNow{};
			float	mAddTime{};
			float	mEnd{};

		public:

			inline Float(
				float* value,
				crfloat start,
				crfloat final,
				crfloat add,
				crfloat end
			) {
				mValue = value;
				mStart = start;
				mFinal = final;
				mNow = 0.0f;
				mAddTime = add;
				mEnd = end;
			}

			inline void Update()override {

				mNow += mAddTime;
				if (mNow >= mEnd) SetDestroy();
				else {
					float lerp = mNow / mEnd;
					(*mValue) = (this->mStart + ((this->mFinal - this->mStart) * lerp));					
				}

			}

		};

	}
}