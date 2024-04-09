#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F1/17
*
********************************************************************************************
* �g�����X�t�H�[��
********************************************************************************************/

#include"Vector.h"
#include"Math.h"

class Transform
{
private:

	ver3 oldPos;//�X�V�O�̃|�W�V����

	list<Component*> mComponent;

private:

	inline void AddComponent(Component* component) {
		mComponent.push_back(component);
		component->Init();
	}

public:

	ver3 pos;	//�|�W�V����
	ver3 rot;	//��]�p�x
	ver3 scale;	//�X�P�[��
	ver3 local; //���[�J���|�W�V����

public:

	inline Transform() {
		pos = Vector3::zero();
		rot = Vector3::zero();
		scale = Vector3::one();
		local = Vector3::zero();		
	}

	inline Transform(crver3 pos, crver3 rot, crver3 scale)
		: pos(pos), rot(rot), scale(scale) {
		this->local = Vector3::zero();
	}

	inline ~Transform() {
		for (auto& data : mComponent) {
			data->Uninit();
			delete data;
		}
		mComponent.clear();		
	}

	inline void Update() {
		oldPos = pos;
		for (auto& data : mComponent) { data->Update(); }
		mComponent.remove_if([](Component* com) { return com->Destroy(); });
	}

	inline void UndoPosition() { pos = oldPos; }

	inline cver3 GetNormalPos()const {
		ver3 normal;
		D3DXVec3Normalize(&normal, &pos);
		return normal;
	}

	/*�x�N�g���擾�֐��Q*/
	/*�O�x�N�g��*/
	inline cver3 GetForward()const {

		matrix matrix;
		D3DXMatrixRotationYawPitchRoll(&matrix, rot.y, rot.x, rot.z);

		ver3 save{};
		save.x = matrix._31;
		save.y = matrix._32;
		save.z = matrix._33;

		return save;

	}

	/*�E�x�N�g��*/
	inline cver3 GetRight()const {

		matrix matrix;
		D3DXMatrixRotationYawPitchRoll(&matrix, rot.y, rot.x, rot.z);

		ver3 save{};
		save.x = matrix._11;
		save.y = matrix._12;
		save.z = matrix._13;

		return save;

	}

	/*��x�N�g��*/
	inline cver3 GetUp()const {

		matrix matrix;
		D3DXMatrixRotationYawPitchRoll(&matrix, rot.y, rot.x, rot.z);

		ver3 save{};
		save.x = matrix._21;
		save.y = matrix._22;
		save.z = matrix._23;

		return save;

	}

	/*�ړ��n*/
	inline void Translation(crver3 final, crfloat time, crfloat end) { AddComponent(new Math::Lerp::Vector3(&pos, pos, final, time, end)); }

	template <class DATA>
	void serialize(DATA& data) { 
		Vector3 Position(this->pos);
		Vector3 Rotation(this->rot);
		Vector3 Scale(this->scale);
		Vector3 LocalPosition(this->local);
		data(CEREAL_NVP(Position), CEREAL_NVP(Rotation), CEREAL_NVP(Scale), CEREAL_NVP(LocalPosition));
		Position.Set(&this->pos);
		Rotation.Set(&this->rot);
		Scale.Set(&this->scale);
		LocalPosition.Set(&this->local);
	}

};

