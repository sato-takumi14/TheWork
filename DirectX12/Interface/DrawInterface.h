#pragma once

/***********************************************************************************************************
*
*	DrawInterface
*
*																				����ҁF������
*																				������F03/06
*
************************************************************************************************************
* �`��֌W�̊��N���X���܂Ƃ߂�����
************************************************************************************************************/

#include"System\Renderer.h"
#include"Component\Transform.h"

class GameObjectInterface;

namespace DrawInterface
{

	/*�`����*/
	class Info
	{
	public:

		pTransform target{};
		Vector3 wh;
		Vector3 u;
		Vector3 v;

	public:

		inline Info() : target(nullptr), wh(Vector3::Zero()), u(Vector3::Zero()), v(Vector3::Zero()) {}
		inline Info(pTransform target, crVector3 wh, crVector3 u = Vector3::Up(), crVector3 v = Vector3::Up()) :
			target(target), wh(wh), u(u), v(v) {}

		/*UV�l�𑝉�������*/
		inline void AddUV() {
			u.x += u.z;
			u.y += u.z;
			if (u.x >= 1.0f) {
				v.x += v.z;
				v.y += v.z;
			}
		}

		/*UV�l�����Z�b�g*/
		inline void ResetUV() {
			u.x = 0.0f;
			u.y = u.z;
			v.x = 0.0f;
			v.z = v.z;
		}

		/*wh�̒l�𐶐�����*/
		static cVector3 CreateWH(crfloat wh, crbool z = false) {
			if (z)return cVector3(wh, 0.0f, wh);
			return cVector3(wh, wh, 0.0f);
		}

		/*�V���A���C�Y*/
		template <class DATA>
		void serialize(DATA& data) {
			Transform transform = *target;
			data(CEREAL_NVP(transform), CEREAL_NVP(wh), CEREAL_NVP(u), CEREAL_NVP(v));
			*target = transform;
		}

	};

	class Device
	{
	protected:

		GameObjectInterface* mTarget{};
		bool mDraw{};

	public:

		inline virtual void Draw() = 0;
		inline virtual void Uninit() {}

		inline void Set(crbool set) { mDraw = set; }
		inline cbool GetBool()const { return mDraw; }

	};

}