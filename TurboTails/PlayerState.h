#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：12/24
*
********************************************************************************************
* プレイヤーステート一覧
********************************************************************************************/

#include"State.h"

#include<unordered_map>

class GameObject;
class MeshField;

namespace PlayerState
{
	/*移動系ステート*/
	namespace Move
	{
		//地面にいるときのステート
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

		//ジャンプしているときのステート
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

		//落ちているときのステート
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

		//オブジェクトに乗っているときのステート
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

		//トランポリンではねているときのステート
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

		//エレベーターにつかまっているステート
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

		//ジャンプをしているときのダッシュ状態ステート
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

		//走っているときのダッシュ状態ステート
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

		//バルーンに当たった時状態ステート
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

	/*攻撃系ステート*/
	namespace Attack
	{
		//攻撃中のステート
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

		//攻撃をためているステート
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

		//攻撃をやめた後のステート
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

		//攻撃前のステート
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
