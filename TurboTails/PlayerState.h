#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F12/24
*
********************************************************************************************
* �v���C���[�X�e�[�g�ꗗ
********************************************************************************************/

#include"State.h"

#include<unordered_map>

class GameObject;
class MeshField;

namespace PlayerState
{
	/*�ړ��n�X�e�[�g*/
	namespace Move
	{
		//�n�ʂɂ���Ƃ��̃X�e�[�g
		class Ground : public State
		{
		private:

			MeshField* mField{};

		public:
			Ground(GameObject* target, MeshField* field);
			void Update()override;
			State* NextState()const override;
			void Set(GameObject* target, MeshField* field) {
				mTarget = target;
				mField = field;
			}
		};

		//�W�����v���Ă���Ƃ��̃X�e�[�g
		class Jump : public State
		{
		private:

			MeshField* mField{};
			ver3 mVel{};
			bool mDash{};

		public:
			Jump(GameObject* target, MeshField* field);
			void Update()override;
			State* NextState()const override;
			void Set(GameObject* target, MeshField* field) {
				mTarget = target;
				mField = field;
			}
			cint GetNowState()const override { return (cint)(PlayerStateMove::jump); }
			void Set(crbool set)override { mDash = set; }
		};

		//�����Ă���Ƃ��̃X�e�[�g
		class Fall : public State
		{
		private:

			MeshField* mField;
			ver3 mVel{};

		public:
			Fall(GameObject* target, MeshField* field);
			void Update()override;
			State* NextState()const override;
			void Set(GameObject* target, MeshField* field) {
				mTarget = target;
				mField = field;
			}
			cint GetNowState()const override { return (cint)(PlayerStateMove::fall); }
		};

		//�I�u�W�F�N�g�ɏ���Ă���Ƃ��̃X�e�[�g
		class RideScaffold : public State
		{
		private:

			//PlayerStateMove mNext = PlayerStateMove::none;
			MeshField* mField;

		public:
			RideScaffold(GameObject* target, MeshField* field);
			void Update()override;
			State* NextState()const override;
			void Set(GameObject* target, MeshField* field) {
				mTarget = target;
				mField = field;
			}
			cint GetNowState()const override { return (cint)(PlayerStateMove::ride); }
		};

		//�g�����|�����ł͂˂Ă���Ƃ��̃X�e�[�g
		class TrampolineState : public State
		{
		private:

			MeshField* mField;
			ver3 mVel{};
			int mTimeCount{};
			int mStopCount{};
			float mSpeed{};

		public:
			TrampolineState(GameObject* target, MeshField* field);
			void Update()override;
			State* NextState()const override;
			void Set(GameObject* target, MeshField* field) {
				mTarget = target;
				mField = field;
			}
			cint GetNowState()const { return (cint)(PlayerStateMove::trampoline); }
		};

		//�G���x�[�^�[�ɂ��܂��Ă���X�e�[�g
		class ElevatorState : public State
		{
		private:

			MeshField* mField;
			GameObject* mElevator{};
			ver3 mVel{};

		public:
			ElevatorState(GameObject* target, MeshField* field);
			void Update()override;
			State* NextState()const override;
			void Set(GameObject* target, MeshField* field) {
				mTarget = target;
				mField = field;
			}
		};

		//�W�����v�����Ă���Ƃ��̃_�b�V����ԃX�e�[�g
		class FlyDash : public State
		{
		private:

			MeshField* mField{};
			int mTime{};
			ver3 mAddForce{};

		public:

			FlyDash(GameObject* target, MeshField* field);
			~FlyDash();
			void Update()override;
			State* NextState()const override;

		};

		//�����Ă���Ƃ��̃_�b�V����ԃX�e�[�g
		class RunDash : public State
		{
		private:

			MeshField* mField{};
			float mSpeed{};
			bool mRide{};

		public:

			RunDash(GameObject* target, MeshField* field);
			void Update()override;
			State* NextState()const override;
			void Set(crbool set)override { mRide = set; }

		};

		//�o���[���ɓ�����������ԃX�e�[�g
		class BalloonState : public State
		{
		private:

			MeshField* mField{};
			int mTime{};

		public:

			BalloonState(GameObject* target, MeshField* field);
			void Update()override;
			State* NextState()const override;

		};

		class None : public State
		{
		public:
			None(GameObject* target, MeshField* filed) {}
		};
	}

	/*�U���n�X�e�[�g*/
	namespace Attack
	{
		//�U�����̃X�e�[�g
		class Assault : public State
		{
		private:

			PARAMETER mParameter{};
			GameObject* mAssaultTarget{};
			ver3 mVel{};
			ver3 mOffset{};
			bool mStop{};

			void SetRotation(crver3 vel);

		public:

			Assault(GameObject* target, const PARAMETER& param);
			~Assault();

			void Update()override;
			State* NextState()const override;
			cbool GetBool()const override { return true; }
			void SetParameter()override;

		};

		//�U�������߂Ă���X�e�[�g
		class Charge : public State
		{
		private:

			PARAMETER mParam{};

		public:

			Charge(GameObject* target, const PARAMETER& param);
			void Update()override;
			State* NextState()const override;
			void SetParameter()override;

		};

		//�U������߂���̃X�e�[�g
		class Deceleration : public State
		{
		private:

			PARAMETER mParam{};
			bool mReCharge{};

		public:

			Deceleration(GameObject* target, const PARAMETER& param);
			void Update()override;
			State* NextState()const override;
			void SetParameter()override;

		};

		//�U���O�̃X�e�[�g
		class PlayerAttackNormal : public State
		{
		private:



		public:

			PlayerAttackNormal(GameObject* target);
			void Update()override;
			State* NextState()const override;
			void SetParameter()override;
			cint GetNowState()const override { return 1; }

		};
	}

}
