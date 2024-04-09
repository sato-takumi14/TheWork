#pragma once

/***********************************************************************************************************
*
*	Transform
*
*																				制作者：佐藤匠
*																				制作日：02/20
*
************************************************************************************************************
* GameObjectのポジション・回転・大きさを管理するクラス
************************************************************************************************************/

#include"Interface\Component.h"
#include"System\Vector.h"

class Transform : public Component
{
private:

	Vector3 mOldPos{};		/*前フレームのポジション*/
	Vector3 mOldLocal{};	/*前フレームのローカルポジション*/
	Vector3 mOldRot{};		/*前フレームの角度*/
	Vector3 mOldScale{};	/*前フレームのスケール*/

public:

	Vector3 pos{};			/*ポジション*/
	Vector3 local{};		/*ローカルポジション*/
	Vector3 rot{};			/*角度*/
	Vector3 scale{};		/*スケール*/
							
public:

	inline Transform() {
		pos = Vector3::Zero();
		rot = Vector3::Zero();
		scale = Vector3::One();
		local = Vector3::Zero();
	}
	inline Transform(
		const Vector3& pos,
		const Vector3& rot,
		const Vector3& scale,
		const Vector3& local) :
		pos(pos), rot(rot), scale(scale), local(local) {}
	inline Transform(const Transform& transform) { *this = transform; }

	/*更新前の情報を保存*/
	inline void Update()override {

		mOldPos = pos;
		mOldRot = rot;
		mOldScale = scale;
		mOldLocal = local;

	}

	/*前フレームのポジションに戻す*/
	inline void UndoPosition() { pos = mOldPos; }
	/*前フレームの角度に戻す*/
	inline void UndoRotation() { rot = mOldRot; }
	/*前フレームのスケールに戻す*/
	inline void UndoScale() { scale = mOldScale; }
	/*前フレームのローカルポジションに戻す*/
	inline void UndoLocalPosition() { local = mOldLocal; }

	/*ベクトル取得*/
	/*前ベクトル*/
	inline cVector3 GetForward()const {
		XMMATRIX matrix = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
		return Vector3(matrix.r[2].m128_f32[0], matrix.r[2].m128_f32[1], matrix.r[2].m128_f32[2]);
	}

	/*右ベクトル*/
	inline cVector3 GetRight()const {
		matrix matrix = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
		return Vector3(matrix.r[0].m128_f32[0], matrix.r[0].m128_f32[1], matrix.r[0].m128_f32[2]);
	}

	/*上ベクトル*/
	inline cVector3 GetUp()const {
		matrix matrix = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
		return Vector3(matrix.r[1].m128_f32[0], matrix.r[1].m128_f32[1], matrix.r[1].m128_f32[2]);
	}

	/*シリアライズ*/
	template <class DATA>
	inline void serialize(DATA& data) { data(CEREAL_NVP(pos), CEREAL_NVP(rot), CEREAL_NVP(scale), CEREAL_NVP(local)); }

};

/*コンスト・コンスト参照・ポインタ・コンストポインタ*/
using cTransform = const Transform;
using crTransform = cTransform&;
using rTransform = Transform&;
using pTransform = Transform*;
using cpTransform = const pTransform;
