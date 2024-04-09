#pragma once

/***********************************************************************************************************
*
*	Transform
*
*																				����ҁF������
*																				������F02/20
*
************************************************************************************************************
* GameObject�̃|�W�V�����E��]�E�傫�����Ǘ�����N���X
************************************************************************************************************/

#include"Interface\Component.h"
#include"System\Vector.h"

class Transform : public Component
{
private:

	Vector3 mOldPos{};		/*�O�t���[���̃|�W�V����*/
	Vector3 mOldLocal{};	/*�O�t���[���̃��[�J���|�W�V����*/
	Vector3 mOldRot{};		/*�O�t���[���̊p�x*/
	Vector3 mOldScale{};	/*�O�t���[���̃X�P�[��*/

public:

	Vector3 pos{};			/*�|�W�V����*/
	Vector3 local{};		/*���[�J���|�W�V����*/
	Vector3 rot{};			/*�p�x*/
	Vector3 scale{};		/*�X�P�[��*/
							
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

	/*�X�V�O�̏���ۑ�*/
	inline void Update()override {

		mOldPos = pos;
		mOldRot = rot;
		mOldScale = scale;
		mOldLocal = local;

	}

	/*�O�t���[���̃|�W�V�����ɖ߂�*/
	inline void UndoPosition() { pos = mOldPos; }
	/*�O�t���[���̊p�x�ɖ߂�*/
	inline void UndoRotation() { rot = mOldRot; }
	/*�O�t���[���̃X�P�[���ɖ߂�*/
	inline void UndoScale() { scale = mOldScale; }
	/*�O�t���[���̃��[�J���|�W�V�����ɖ߂�*/
	inline void UndoLocalPosition() { local = mOldLocal; }

	/*�x�N�g���擾*/
	/*�O�x�N�g��*/
	inline cVector3 GetForward()const {
		XMMATRIX matrix = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
		return Vector3(matrix.r[2].m128_f32[0], matrix.r[2].m128_f32[1], matrix.r[2].m128_f32[2]);
	}

	/*�E�x�N�g��*/
	inline cVector3 GetRight()const {
		matrix matrix = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
		return Vector3(matrix.r[0].m128_f32[0], matrix.r[0].m128_f32[1], matrix.r[0].m128_f32[2]);
	}

	/*��x�N�g��*/
	inline cVector3 GetUp()const {
		matrix matrix = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
		return Vector3(matrix.r[1].m128_f32[0], matrix.r[1].m128_f32[1], matrix.r[1].m128_f32[2]);
	}

	/*�V���A���C�Y*/
	template <class DATA>
	inline void serialize(DATA& data) { data(CEREAL_NVP(pos), CEREAL_NVP(rot), CEREAL_NVP(scale), CEREAL_NVP(local)); }

};

/*�R���X�g�E�R���X�g�Q�ƁE�|�C���^�E�R���X�g�|�C���^*/
using cTransform = const Transform;
using crTransform = cTransform&;
using rTransform = Transform&;
using pTransform = Transform*;
using cpTransform = const pTransform;
