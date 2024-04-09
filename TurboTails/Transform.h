#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：1/17
*
********************************************************************************************
* トランスフォーム
********************************************************************************************/

#include"Vector.h"
#include"Math.h"

class Transform
{
private:

	ver3 oldPos;//更新前のポジション

	list<Component*> mComponent;

private:

	inline void AddComponent(Component* component) {
		mComponent.push_back(component);
		component->Init();
	}

public:

	ver3 pos;	//ポジション
	ver3 rot;	//回転角度
	ver3 scale;	//スケール
	ver3 local; //ローカルポジション

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

	/*ベクトル取得関数群*/
	/*前ベクトル*/
	inline cver3 GetForward()const {

		matrix matrix;
		D3DXMatrixRotationYawPitchRoll(&matrix, rot.y, rot.x, rot.z);

		ver3 save{};
		save.x = matrix._31;
		save.y = matrix._32;
		save.z = matrix._33;

		return save;

	}

	/*右ベクトル*/
	inline cver3 GetRight()const {

		matrix matrix;
		D3DXMatrixRotationYawPitchRoll(&matrix, rot.y, rot.x, rot.z);

		ver3 save{};
		save.x = matrix._11;
		save.y = matrix._12;
		save.z = matrix._13;

		return save;

	}

	/*上ベクトル*/
	inline cver3 GetUp()const {

		matrix matrix;
		D3DXMatrixRotationYawPitchRoll(&matrix, rot.y, rot.x, rot.z);

		ver3 save{};
		save.x = matrix._21;
		save.y = matrix._22;
		save.z = matrix._23;

		return save;

	}

	/*移動系*/
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

