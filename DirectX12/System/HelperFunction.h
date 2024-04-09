#pragma once

/***********************************************************************************************************
*
*	HelperFunction
*
*																				制作者：佐藤匠
*																				制作日：02/26
*
************************************************************************************************************
* 補助関数群
************************************************************************************************************/

#include"Main.h"
#include"System\Vector.h"
#include"Interface\Component.h"
#include"Interface\Interface.h"

namespace HelperFunction
{
	
	/*ストリング系*/
	class String
	{
	public:

		/*stringをwstringに変換*/
		static inline cwstring GetWideString(crstring str) {

			auto num1 = MultiByteToWideChar(
				CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
				str.c_str(), -1, nullptr, 0);

			wstring wstr;
			wstr.resize(num1);

			auto num2 = MultiByteToWideChar(
				CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
				str.c_str(), -1, &wstr[0], num1);

#if _DEBUG

			assert(num1 == num2);

#endif // _DEBUG

			return wstr;

		}

		/*ファイル識別子取得　ワイド文字版*/
		static inline const wstring FileExtension(const wstring& path) {
			const size_t idx = path.rfind(L".");
			return path.substr(idx + 1, path.length() - idx - 1);
		}

		/*ファイル識別子　文字列版*/
		static inline cstring FileExtension(crstring path) {
			const size_t idx = path.rfind(".");
			return path.substr(idx + 1, path.length() - idx - 1);
		}

	};

	/*計算系*/
	namespace Math
	{

		/*円周率関係*/
		namespace PI
		{

			/*円周率π*/
			constexpr cfloat PI = 3.141592654f;

			/*度からラジアンに変換*/
			static inline cfloat Deg2Rad(crfloat rot) { return rot * (PI / 180); }

			/*ラジアンから度に変換*/
			static inline cfloat Rad2Deg(crfloat rot) { return rot * (180 / PI); }


		}
		
		/*線形補間*/
		namespace Lerp
		{

			/*線形補間をするクラスのインターフェース*/
			class Lerp : public Component
			{
			protected:

				float mNow{};
				float mAddTime{};
				float mEnd{};

			};

			/*int型の線形補間*/
			class Int : public Lerp, public Interface
			{
			private:

				int* mValue{};
				int mStart{};
				int mFinal{};

			public:

				inline Int(
					int* value,
					crint start,
					crint final,
					crfloat add,
					crfloat end
				) : mValue(value), mStart(start), mFinal(final) {
					this->mAddTime = add;
					this->mEnd = end;
					this->mNow = 0.0f;
				}

				inline void Update()override {

					mNow += mAddTime;
					if (mNow >= mEnd)SetDestroy();
					else {
						float lerp = mNow / mEnd;
						(*mValue) = this->mStart + static_cast<int>(((this->mFinal - this->mStart) * lerp));
					}

				}

			};

			/*float型の線形補間*/
			class Float : public Lerp, public Interface
			{
			private:

				float* mValue{};
				float	mStart{};
				float	mFinal{};

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

			/*double型の線形補間*/
			class Double : public Lerp, public Interface
			{
			private:

				double* mValue{};
				double	mStart{};
				double	mFinal{};

			public:

				inline Double(
					double* value,
					crdouble start,
					crdouble final,
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

			/*Vector2型の線形補間*/
			class Float2 : public Lerp, public Interface
			{
			private:

				pVector2 mValue{};
				Vector2	mStart{};
				Vector2	mFinal{};

			public:

				inline Float2(
					pVector2 value,
					crVector2 start,
					crVector2 final,
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

			/*Vector3型の線形補間*/
			class Float3 : public Lerp, public Interface
			{
			private:

				pVector3 mValue{};
				Vector3	mStart{};
				Vector3	mFinal{};

			public:

				inline Float3(
					pVector3 value,
					crVector3 start,
					crVector3 final,
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

			/*Vector4型の線形補間*/
			class Float4 : public Lerp, public Interface
			{
			private:

				Vector4* mValue{};
				Vector4	mStart{};
				Vector4	mFinal{};

			public:

				inline Float4(
					Vector4* value,
					crVector4 start,
					crVector4 final,
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

		/*クランプ*/
		namespace Clamp
		{

			/*int型*/
			static inline cint Int(int* data, crint max, crint min = 0) {
				if ((*data) > max)(*data) = max;
				else if ((*data) < min)(*data) = min;
				return (*data);
			}

			/*float型*/
			static inline cfloat Float(float* data, crfloat max, crfloat min = 0.0f) {
				if ((*data) > max)(*data) = max;
				else if ((*data) < min)(*data) = min;
				return (*data);
			}

			/*double型*/
			static inline cdouble Double(double* data, crdouble max, crdouble min = 0.0) {
				if ((*data) > max)(*data) = max;
				else if ((*data) < min)(*data) = min;
				return (*data);
			}

			/*Vector2型*/
			static inline cVector2 Vector2(pVector2 data, crfloat max, crfloat min = 0.0f) {
				if ((*data).x > max)(*data).x = max;
				else if ((*data).x < min)(*data).y = min;
				if ((*data).y > max)(*data).y = max;
				else if ((*data).y < min)(*data).y = min;
				return (*data);
			}

			/*Vector3型*/
			static inline cVector3 Vector3(pVector3 data, crfloat max, crfloat min = 0.0f) {
				if ((*data).x > max)(*data).x = max;
				else if ((*data).x < min)(*data).y = min;
				if ((*data).y > max)(*data).y = max;
				else if ((*data).y < min)(*data).y = min;
				if ((*data).z > max)(*data).z = max;
				else if ((*data).z < min)(*data).z = min;
				return (*data);
			}

			/*Vector4型*/
			static inline cVector4 Vector4(pVector4 data, crfloat max, crfloat min = 0.0f) {
				if ((*data).x > max)(*data).x = max;
				else if ((*data).x < min)(*data).y = min;
				if ((*data).y > max)(*data).y = max;
				else if ((*data).y < min)(*data).y = min;
				if ((*data).z > max)(*data).z = max;
				else if ((*data).z < min)(*data).z = min;
				if ((*data).w > max)(*data).w = max;
				else if ((*data).w < min)(*data).w = min;
				return (*data);
			}

			/*Color型*/
			static inline cColor Color(pColor data, crfloat max, crfloat min = 0.0f) {
				if ((*data).r > max)(*data).r = max;
				else if ((*data).r < min)(*data).r = min;
				if ((*data).g > max)(*data).g = max;
				else if ((*data).g < min)(*data).g = min;
				if ((*data).b > max)(*data).b = max;
				else if ((*data).b < min)(*data).b = min;
				if ((*data).a > max)(*data).a = max;
				else if ((*data).a < min)(*data).a = min;
				return (*data);
			}

		}

		/*累乗*/
		namespace Pow
		{

			/*int型*/
			static inline cint Int(int* f, crint p = 1) {
				for (int i = 0; i < p && 1 < p; i++)(*f) *= (*f);
				return (*f);
			}

			/*float型*/
			static inline cfloat Float(float* f, crint p = 1) {
				for (int i = 0; i < p && 1 < p; i++)(*f) *= (*f);
				return (*f);
			}

			/*double型*/
			static inline cdouble Double(double* f, crint p = 1) {
				for (int i = 0; i < p && 1 < p; i++)(*f) *= (*f);
				return (*f);
			}

			/*Vector2型*/
			static inline cVector2 Vector2(pVector2 f, crint p = 1) {
				for (int i = 0; i < p && 1 < p; i++)(*f) *= (*f);
				return (*f);
			}

			/*Vector3型*/
			static inline cVector3 Vector3(pVector3 f, crint p = 1) {
				for (int i = 0; i < p && 1 < p; i++)(*f) *= (*f);
				return (*f);
			}

			/*Vector4型*/
			static inline cVector4 Vector4(pVector4 f, crint p = 1) {
				for (int i = 0; i < p && 1 < p; i++)(*f) *= (*f);
				return (*f);
			}

			/*Color型*/
			static inline cColor Color(pColor f, crint p = 1) {
				for (int i = 0; i < p && 1 < p; i++)(*f) *= (*f);
				return (*f);
			}

		}

	}

	namespace Function
	{

		/*一度だけ実行*/
		class DoOnce : public Component
		{
		private:

			std::function<void()> mFunc{};
			bool mExecution{};

		public:

			inline DoOnce(const std::function<void()>& func) :
				mFunc(func), mExecution(false) {}

			inline void Update()override {

				if (mExecution)return;

				mFunc();
				SetDestroy();
				mExecution = true;

			}

		};

		/*入れた関数の順番に実行*/
		class Sequence : public Component
		{
		private:

			std::unordered_map<int, std::function<void()>> mFunc{};
			int mIndex = 0;

		public:

			inline Sequence(const std::unordered_map<int, std::function<void()>>& func) : mFunc(func) {}

			inline void Update()override {

				mFunc[mIndex++]();
				if (mIndex == mFunc.size())mIndex = 0;

			}

		};

		/*交互に実行*/
		class FlipFlop : public Component
		{
		private:

			std::unordered_map<bool, std::function<void()>> mFunc{};
			bool mSwitch = false;

		public:

			inline FlipFlop(const std::unordered_map<bool, std::function<void()>>& func) : mFunc(func) {}

			inline void Update()override {

				mFunc[mSwitch]();
				mSwitch = not mSwitch;

			}

		};

	}

}