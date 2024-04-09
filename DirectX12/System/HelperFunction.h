#pragma once

/***********************************************************************************************************
*
*	HelperFunction
*
*																				����ҁF������
*																				������F02/26
*
************************************************************************************************************
* �⏕�֐��Q
************************************************************************************************************/

#include"Main.h"
#include"System\Vector.h"
#include"Interface\Component.h"
#include"Interface\Interface.h"

namespace HelperFunction
{
	
	/*�X�g�����O�n*/
	class String
	{
	public:

		/*string��wstring�ɕϊ�*/
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

		/*�t�@�C�����ʎq�擾�@���C�h������*/
		static inline const wstring FileExtension(const wstring& path) {
			const size_t idx = path.rfind(L".");
			return path.substr(idx + 1, path.length() - idx - 1);
		}

		/*�t�@�C�����ʎq�@�������*/
		static inline cstring FileExtension(crstring path) {
			const size_t idx = path.rfind(".");
			return path.substr(idx + 1, path.length() - idx - 1);
		}

	};

	/*�v�Z�n*/
	namespace Math
	{

		/*�~�����֌W*/
		namespace PI
		{

			/*�~������*/
			constexpr cfloat PI = 3.141592654f;

			/*�x���烉�W�A���ɕϊ�*/
			static inline cfloat Deg2Rad(crfloat rot) { return rot * (PI / 180); }

			/*���W�A������x�ɕϊ�*/
			static inline cfloat Rad2Deg(crfloat rot) { return rot * (180 / PI); }


		}
		
		/*���`���*/
		namespace Lerp
		{

			/*���`��Ԃ�����N���X�̃C���^�[�t�F�[�X*/
			class Lerp : public Component
			{
			protected:

				float mNow{};
				float mAddTime{};
				float mEnd{};

			};

			/*int�^�̐��`���*/
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

			/*float�^�̐��`���*/
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

			/*double�^�̐��`���*/
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

			/*Vector2�^�̐��`���*/
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

			/*Vector3�^�̐��`���*/
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

			/*Vector4�^�̐��`���*/
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

		/*�N�����v*/
		namespace Clamp
		{

			/*int�^*/
			static inline cint Int(int* data, crint max, crint min = 0) {
				if ((*data) > max)(*data) = max;
				else if ((*data) < min)(*data) = min;
				return (*data);
			}

			/*float�^*/
			static inline cfloat Float(float* data, crfloat max, crfloat min = 0.0f) {
				if ((*data) > max)(*data) = max;
				else if ((*data) < min)(*data) = min;
				return (*data);
			}

			/*double�^*/
			static inline cdouble Double(double* data, crdouble max, crdouble min = 0.0) {
				if ((*data) > max)(*data) = max;
				else if ((*data) < min)(*data) = min;
				return (*data);
			}

			/*Vector2�^*/
			static inline cVector2 Vector2(pVector2 data, crfloat max, crfloat min = 0.0f) {
				if ((*data).x > max)(*data).x = max;
				else if ((*data).x < min)(*data).y = min;
				if ((*data).y > max)(*data).y = max;
				else if ((*data).y < min)(*data).y = min;
				return (*data);
			}

			/*Vector3�^*/
			static inline cVector3 Vector3(pVector3 data, crfloat max, crfloat min = 0.0f) {
				if ((*data).x > max)(*data).x = max;
				else if ((*data).x < min)(*data).y = min;
				if ((*data).y > max)(*data).y = max;
				else if ((*data).y < min)(*data).y = min;
				if ((*data).z > max)(*data).z = max;
				else if ((*data).z < min)(*data).z = min;
				return (*data);
			}

			/*Vector4�^*/
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

			/*Color�^*/
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

		/*�ݏ�*/
		namespace Pow
		{

			/*int�^*/
			static inline cint Int(int* f, crint p = 1) {
				for (int i = 0; i < p && 1 < p; i++)(*f) *= (*f);
				return (*f);
			}

			/*float�^*/
			static inline cfloat Float(float* f, crint p = 1) {
				for (int i = 0; i < p && 1 < p; i++)(*f) *= (*f);
				return (*f);
			}

			/*double�^*/
			static inline cdouble Double(double* f, crint p = 1) {
				for (int i = 0; i < p && 1 < p; i++)(*f) *= (*f);
				return (*f);
			}

			/*Vector2�^*/
			static inline cVector2 Vector2(pVector2 f, crint p = 1) {
				for (int i = 0; i < p && 1 < p; i++)(*f) *= (*f);
				return (*f);
			}

			/*Vector3�^*/
			static inline cVector3 Vector3(pVector3 f, crint p = 1) {
				for (int i = 0; i < p && 1 < p; i++)(*f) *= (*f);
				return (*f);
			}

			/*Vector4�^*/
			static inline cVector4 Vector4(pVector4 f, crint p = 1) {
				for (int i = 0; i < p && 1 < p; i++)(*f) *= (*f);
				return (*f);
			}

			/*Color�^*/
			static inline cColor Color(pColor f, crint p = 1) {
				for (int i = 0; i < p && 1 < p; i++)(*f) *= (*f);
				return (*f);
			}

		}

	}

	namespace Function
	{

		/*��x�������s*/
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

		/*���ꂽ�֐��̏��ԂɎ��s*/
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

		/*���݂Ɏ��s*/
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